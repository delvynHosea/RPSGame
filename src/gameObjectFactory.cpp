#include "gameObjectFactory.h"
#include "RPSGameObject.h"

RPSGameObject* gameObjectFactory::createRock(int x, int y, char* str, bool control) {
  return new RPSGameObject(ROCK, x, y, str, control);
}

RPSGameObject* gameObjectFactory::createPaper(int x, int y) {
  return new RPSGameObject(PAPER, x, y, "P", false);
}

RPSGameObject* gameObjectFactory::createScissor(int x, int y) {
  return new RPSGameObject(SCISSORS, x, y, "S", false);
}