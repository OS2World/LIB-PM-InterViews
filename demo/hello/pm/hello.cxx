#include <InterViews/world.h>
#include <InterViews/message.h>

void main (int argc, char *argv[]) {
  World* world = new World("hello", argc, argv);
  world->Insert(new Message("Hello world"));
  world->Run();
}
