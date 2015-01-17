#include <InterViews/world.h>
#include <InterViews/message.h>

#if !(defined(DOS) || defined(MSDOS))
extern int _Xdebug;
#endif

void main () {
  World* world = new World();
  world->Insert(new Message("Hello world"));
  world->Run();
}
