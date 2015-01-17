# Makefile for creating PM programs

TOP = d:/iv/src
TOPI = $(TOP)/interviews
TOPSTDI = $(TOPI)/Std
CDEBUGFLAGS = -Od -Zi -k
CXXFLAGS = !Q !C +zf127 +zl30000
CFLAGS = $(CXXFLAGS) -c -Alfu -Gt1 -G2s -D__cplusplus
LDDEBUGFLAGS = /CO
LDFLAGS = $(LDDEBUGFLAGS) /SE:256 /align:16
INCLUDES = -I$(TOPI)/pm -I$(TOPSTDI) -I$(TOPI) -I$(TOP)
IVLIB = d:\iv\src\libInterViews\intervw.lib
LIBS = os2 intervw lCxxCe llibce
PROGNAME = squares
DEBUGDIR = cvp
OBJS = glue.obj main.obj metaview.obj sframe.obj squares.obj view.obj

RM = del
CP = copy
CC = ccxx
LD = link

all: $(PROGNAME)

$(PROGNAME): $(PROGNAME).exe

$(PROGNAME).exe: $(OBJS) intervw.lib
	$(LD) $(LDFLAGS) $(OBJS),$(PROGNAME),$(PROGNAME),$(LIBS),$(PROGNAME)

intervw.lib: $(IVLIB)
	copy $(IVLIB) intervw.lib
	lib intervw.lib -glue.obj;

glue.obj: glue.cxx
	@echo "compiling glue.cxx"
	-$(RM) $@
	$(CC) $(CFLAGS) $(INCLUDES) glue.cxx

main.obj: main.cxx
	@echo "compiling main.cxx"
	-$(RM) $@
	$(CC) $(CFLAGS) $(INCLUDES) main.cxx

metaview.obj: metaview.cxx
	@echo "compiling metaview.cxx"
	-$(RM) $@
	$(CC) $(CFLAGS) $(INCLUDES) metaview.cxx

sframe.obj: sframe.cxx
	@echo "compiling sframe.cxx"
	-$(RM) $@
	$(CC) $(CFLAGS) $(INCLUDES) sframe.cxx

squares.obj: squares.cxx
	@echo "compiling squares.cxx"
	-$(RM) $@
	$(CC) $(CFLAGS) $(INCLUDES) squares.cxx

view.obj: view.cxx
	@echo "compiling view.cxx"
	-$(RM) $@
	$(CC) $(CFLAGS) $(INCLUDES) view.cxx

clean:
	-$(RM) *.obj
	-$(RM) *.exe
