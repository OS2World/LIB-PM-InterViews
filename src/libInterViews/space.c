/*
 * Copyright (c) 1987, 1988, 1989 Stanford University
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Stanford not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  Stanford makes no representations about
 * the suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * STANFORD DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
 * IN NO EVENT SHALL STANFORD BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/****

 Copyright (C) 1990 Texas Instruments Incorporated.

 Permission is granted to any individual or institution to use, copy, modify,
 and distribute this software, provided that this complete copyright and
 permission notice is maintained, intact, in all copies and supporting
 documentation.

 Texas Instruments Incorporated provides this software "as is" without express
 or implied warranty.

****/

/*
 * Object spaces are objects that manage a collection of other objects.
 */

//
// rl:4/10 - typecast to (fd_set *) for Std
// rl:4/10 - added SignalIgnore def in <IV/space.h> (was in Std/signal.h)
//


#include <InterViews/catalog.h>
#include <InterViews/connection.h>
#include <InterViews/space.h>
#include <InterViews/packet.h>
#include <InterViews/spaceman.h>
#include <InterViews/stub.h>
#include <InterViews/tagtable.h>
#include <bstring.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(DOS) || defined(MSDOS)
#include <time.h>
#else
#include <os/ipc.h>
#include <sys/time.h>
#endif


/*
 * Helper class for object spaces.
 * A messenger handles requests for a particular client.
 */

typedef enum { ready, waiting, finished } MessengerState;

class Messenger {
private:
    friend class ObjectSpace;

    Messenger(ObjectSpace*, Connection*);
    ~Messenger();

    MessengerState Run();
    Connection* GetClient () { return client; }
    void SetPending () { pending = true; }

    ObjectSpace* space;
    Connection* client;
    boolean pending;	/* buffer contains at least one complete packet */
    char* buffer;	/* buffer of incoming packets */
    long buflen;		/* length of buffer */
    long cur;		/* offset in buffer of current packet */
    long count;		/* number of bytes left in buffer */
    Messenger* next;	/* in dispatcher's list */
    Messenger* prev;

    MessengerState ReadData();
    boolean ProcessMessage();
    void GrowBuffer(long);
};

/*
 * Create a space without registering it with the space manager.
 */

ObjectSpace::ObjectSpace () {
    Init();
    name = nil;
    manager = nil;
}

/*
 * Create a space and register it with the space manager under
 * the given name.
 */

ObjectSpace::ObjectSpace (const char* str) {
    Init();
    name = strrchr(str, '/');
    if (name == nil) {
	name = str;
    } else {
	++name;
    }
    manager = new SpaceManager;
    local = new Connection;
    manager->Register(str, local, remote);
    Listen(local);
}

ObjectSpace::~ObjectSpace () {
    delete dictionary;
    delete table;
}

/*
 * Initialize object space information.
 */

void ObjectSpace::Init () {
    dictionary = new Catalog(4096);
    table = new ObjectTable(4096);
    local = nil;
    remote = nil;
    channels = 0;
    maxchannel = 0;
    active.head = nil;
    inactive.head = nil;
    streams = nil;
    /*
     * Ignore SIGPIPE's because they can be caused by writing to a client
     * after the client exits.
     */
    signal(SIGPIPE, SignalIgnore);
}

/*
 * Add a descriptor to the select mask.
 */

void ObjectSpace::Attach (register long ch) {
    channels |= (1 << ch);
    if (ch > maxchannel) {
	maxchannel = ch;
    }
}

/*
 * Remove a descriptor from the select mask.
 * This is a bit of a pain because we need to adjust
 * maxchannels in case the removed descriptor was the highest one
 * in the mask.
 */

void ObjectSpace::Detach (long ch) {
    channels &= ~(1 << ch);
    if (channels == 0) {
	maxchannel = 0;
    } else {
	while ((channels & (1 << maxchannel)) == 0) {
	    --maxchannel;
	}
    }
}

/*
 * Start listening to the given connection, assuming it is not nil.
 */

void ObjectSpace::Listen (Connection* c) {
    if (c != nil) {
	Attach(c->Descriptor());
    }
}

/*
 * Check to see if the given mask indicates that a connection
 * has data pending.
 */

boolean ObjectSpace::Ready (long mask, Connection* c) {
    register long d;

    d = (1 << c->Descriptor());
    if ((mask & d) != 0) {
	mask &= ~d;
	return true;
    }
    return false;
}

/*
 * Check to see if the given mask contains the service connection.
 * If so, add a new client.
 */

void ObjectSpace::CheckServer (long mask, Connection* c) {
    if (c != nil && Ready(mask, c)) {
	Connection* nc = c->AcceptClient();
	Listen(nc);
	AddClient(nc);
    }
}

/*
 * Start listening to the service connections.
 */

void ObjectSpace::StartServer (Connection* here, Connection* there) {
    Listen(here);
    Listen(there);
    local = here;
    remote = there;
}

/*
 * Add a messenger to the given queue.
 */

void ObjectSpace::Add (register Messenger* m, register MQueue& q) {
    m->next = nil;
    if (q.head == nil) {
	m->prev = nil;
	q.head = m;
	q.tail = m;
    } else {
	m->prev = q.tail;
	q.tail->next = m;
	q.tail = m;
    }
}

/*
 * Remove a messenger from the given queue.
 */

void ObjectSpace::Remove (register Messenger* m, MQueue& q) {
    if (m->prev == nil) {
	q.head = m->next;
    } else {
	m->prev->next = m->next;
    }
    if (m->next == nil) {
	q.tail = m->prev;
    } else {
	m->next->prev = m->prev;
    }
}

/*
 * Disconnect from a given messenger.
 */

void ObjectSpace::CloseDown (Messenger* m) {
    Connection* c = m->GetClient();
    Detach(c->Descriptor());
    table->RemoveAll(c);
    delete m;
}

/*
 * Default way to add a client is to create a messenger for it.
 * Spaces need to redefine this if they want to treat clients as streams.
 */

void ObjectSpace::AddClient (Connection* c) {
    Add(new Messenger(this, c), inactive);
}

/*
 * Basic message-handling dispatch loop.
 */

void ObjectSpace::Dispatch () {
    register long nready;
    register Messenger* m;
    register Stream* s;
    long rdmask;

    m = active.head;
    if (m == nil) {
	do {
	    rdmask = channels;
	    nready = select((int)(maxchannel+1), (fd_set *)&rdmask, 0, 0, nil);  // rl:4/10
	    if (nready < 0 && errno == EBADF) {
		CheckClients();
	    }
	} while (nready < 0);
	CheckServer(rdmask, local);
	CheckServer(rdmask, remote);
	if (rdmask != 0) {
	    for (m = inactive.head; m != nil; m = m->next) {
		if (Ready(rdmask, m->GetClient())) {
		    m->SetPending();
		    Remove(m, inactive);
		    Add(m, active);
		}
	    }
	    for (s = streams; s != nil; s = s->next) {
		if ((rdmask & s->mask) != 0) {
		    s->object->ChannelReady(s->channel);
		}
	    }
	}
    } else {
	Remove(m, active);
	switch (m->Run()) {
	    case ready:
		Add(m, active);
		break;
	    case waiting:
		Add(m, inactive);
		break;
	    case finished:
		CloseDown(m);
		break;
	}
    }
}

/*
 * Poll the inactive clients to see if any have disappeared.
 * This shouldn't really be necessary; we only do it when
 * we get a bad file error from select in Dispatch.
 */

void ObjectSpace::CheckClients () {
    register Messenger* m;
    register Messenger* next;
    register long d;
    long rdmask;
    struct timeval poll;

    poll.tv_sec = 0;
    poll.tv_usec = 0;
    for (m = inactive.head; m != nil; m = next) {
	next = m->next;
	d = m->GetClient()->Descriptor();
	rdmask = 1 << d;
	if (select((int)(d+1), (fd_set *)&rdmask, 0, 0, &poll) < 0) { // rl:4/10
	    Remove(m, inactive);
	    CloseDown(m);
	}
    }
}

/*
 * Default way to handle messages to the space itself.
 */

void ObjectSpace::Message (Connection* c, ObjectTag, long op, void* msg, long) {
    register objectspace_Msg* m;
    ObjectStub* s;

    m = (objectspace_Msg*)msg;
    switch (op) {
	case objectspace_Find:
	    if (dictionary->Find(s, m->name)) {
		table->Add(c, m->tag, s);
	    }
	    break;
	case objectspace_Clone:
	    if (dictionary->Find(s, m->name)) {
		table->Add(c, m->tag, s->Clone());
	    }
	    break;
	case objectspace_Destroy:
	    if (dictionary->Find(s, m->name)) {
		dictionary->UnRegister(m->name);
		Unref(s);
	    }
	    break;
	default:
	    /* ignore op */;
    }
}

/*
 * Return the stub associated with a given tag.
 */

ObjectStub* ObjectSpace::Map (Connection* client, ObjectTag t) {
    return table->Find(client, t);
}

/*
 * Deliver a message to the appropriate stub.  A zero tag
 * by convention means a message to the object space itself.
 */

void ObjectSpace::Deliver (
    Connection* client, ObjectTag t, long op, void* msg, long n
) {
    if (t == 0) {
	Message(client, t, op, msg, n);
    } else {
	ObjectStub* s = Map(client, t);
	if (s == nil) {
	    /* bad target tag -- what should we do? */
	} else {
	    s->Message(client, t, op, msg, n);
	}
    }
}

/*
 * Set up an additional channel to receive data on.  This channel
 * is treated as a stream--no packet interpretation is performed.
 * Note that AddChannel does not start listening to the channel--
 * this must be done by Attach.
 */

void ObjectSpace::AddChannel (long c, ObjectStub* o) {
    register Stream* s;

    s = new Stream;
    s->channel = c;
    s->mask = (1 << c);
    s->object = o;
    s->next = streams;
    streams = s;
}

/*
 * Remove a channel and stop listening to it.
 */

void ObjectSpace::RemoveChannel (long c) {
    register Stream* s, * prev;

    prev = nil;
    for (s = streams; s != nil; s = s->next) {
	if (s->channel == c) {
	    if (prev == nil) {
		streams = s->next;
	    } else {
		prev->next = s->next;
	    }
	    Detach(c);
	    delete s;
	    break;
	}
	prev = s;
    }
}

/*
 * Change path in space manager.
 */

void ObjectSpace::UsePath (const char* newpath) {
    manager->UsePath(newpath);
}

/*
 * Create connection to an object space.
 * Block if wait is true.
 */

Connection* ObjectSpace::Find (const char* spacename, boolean wait) {
    return manager->Find(spacename, wait);
}

/*
 * What to do if we get some message data that is intolerable.
 */

static void BadMessage (long index) {
    fprintf(stderr, "invalid buffer index %ld\n", index);
    exit(1);
}

/*
 * Initial and maximum allowed buffer sizes for a client.
 * A client's buffer grows when an incoming packet is larger than
 * the packet size.
 */

static const int initlen = 256;
static const int maxlen = 8192;

Messenger::Messenger (ObjectSpace* s, Connection* c) {
    space = s;
    client = c;
    pending = false;
    buffer = new char[initlen];
    buflen = initlen;
    cur = 0;
    count = 0;
    next = nil;
    prev = nil;
}

Messenger::~Messenger () {
    delete client;
    delete buffer;
}

/*
 * We have reached the head of the active queue, so try to read a message
 * into our buffer and split it into packets.
 */

MessengerState Messenger::Run () {
    if (ProcessMessage()) {
	return ready;
    } else if (pending) {
	return ReadData();
    } else {
	return waiting;
    }
}

/*
 * Read as much data as we can into the buffer.
 */

MessengerState Messenger::ReadData () {
    register long n, last;

    last = cur + count;
    if (last < 0 || last >= buflen) {
	BadMessage(last);
    }
    n = client->Read(&buffer[last], (int)(buflen - last));
    if (n > 0) {
	count += n;
	pending = false;
	return ready;
    } else {
	if (n < 0) {
	    /* error reading data -- ignore for now */
	}
	return finished;
    }
}

/*
 * Try to process a packet in the buffer.  If there isn't a complete one,
 * return false.  Otherwise handle the packet and return true.
 */

boolean Messenger::ProcessMessage () {
    boolean b;
    register Packet* h;
    register long len, hlen;

    if (cur < 0 || cur >= buflen) {
	BadMessage(cur);
    }
    if (count < sizeof(Packet)) {
	/* incomplete packet header in buffer */
	b = false;
    } else {
	h = (Packet*) &buffer[cur];
	hlen = WordAlign(h->length);
	len = sizeof(Packet) + hlen;
	if (hlen > maxlen) {
	    /* bad length, skip over header */
	    b = true;
	    cur += sizeof(Packet);
	    count -= sizeof(Packet);
	    if (count == 0) {
		cur = 0;
	    }
	} else if (count < len) {
	    /* incomplete packet */
	    b = false;
	    if (len > buflen) {
		GrowBuffer(len);
	    }
	} else {
	    b = true;
	    cur += sizeof(Packet);
	    space->Deliver(client, h->tag, h->op, &buffer[cur], h->length);
	    count -= len;
	    if (count == 0) {
		cur = 0;
	    } else {
		cur += hlen;
	    }
	}
    }
    if (!b && cur != 0) {
	/* move incomplete message to beginning of buffer */
	bcopy(&buffer[cur], &buffer[0], (int)count);
	cur = 0;
    }
    return b;
}

/*
 * Grow the buffer to handle a larger message.
 */

void Messenger::GrowBuffer (long len) {
    char* overflow;

    overflow = new char[len];
    bcopy(&buffer[cur], &overflow[0], (int)count);
    cur = 0;
    delete buffer;
    buffer = overflow;
    buflen = len;
}
