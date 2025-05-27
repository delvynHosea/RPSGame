// TODO 
// Finish the declaration of GameObjectFactory that
// 1. offers some methods to create a GameObject.
// 2. encapsulates the constructor from the caller. 

#ifndef GAMEOBJECTFACTORY_H
#define GAMEOBJECTFACTORY_H

#include "RPSgameObject.h"

class gameObjectFactory {
public: 
  static RPSGameObject* createRock(int, int, char*, bool);
  static RPSGameObject* createPaper(int, int);
  static RPSGameObject* createScissor(int, int);
};

#endif