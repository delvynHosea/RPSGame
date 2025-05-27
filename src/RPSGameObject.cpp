#include "RPSGameObject.h"
#include "gameObjectFactory.h"
#include "gameObject.h"
#include <string.h>
#include <iostream>

RPSGameObject::RPSGameObject(RPSType type, int x, int y, char* str, bool set) {
  setType(type);

  _pos = {x, y};
  (set) ? setControl() : unsetControl();
  if(strcmp(str, "P") == 0) {
    _icon = iconFactory::createPaperIcon(CYAN, str);
  } else if(strcmp(str, "S") == 0) {
    _icon = iconFactory::createScissorIcon(GREEN, "S");
  } else {
    _icon = (set) ? iconFactory::createRockIcon(RED, "[R") : iconFactory::createRockIcon(RED, str);
  }
}

void RPSGameObject::setType(RPSType type) {
  _type = type;
  setIcon();
}

bool RPSGameObject::getControl() {
  return control;
}

RPSType RPSGameObject::getType() const {
  return _type;
}

void RPSGameObject::setControl() {
  control = true;
}

void RPSGameObject::unsetControl() {
  control = false;
}

void RPSGameObject::setIcon() {
  if(control && getType() == ROCK) {
    _icon = iconFactory::createRockIcon(RED, "[R");
  } else if(!control && getType() == ROCK){
    _icon = iconFactory::createRockIcon(RED, " R");
  } else if(getType() == PAPER) {
    _icon = iconFactory::createPaperIcon(CYAN, " P");
  } else if(getType() == SCISSORS) {
    _icon = iconFactory::createScissorIcon(GREEN, " S");
  }
}

void RPSGameObject::update(int mvmt) {
  if(mvmt == 1 && _pos.e2 > 0) _pos.e2--;
  if(mvmt == 2 && _pos.e2 < GAME_WINDOW_HEIGHT - 1) _pos.e2++;
  if(mvmt == 3 && _pos.e1 > 0) _pos.e1--;
  if(mvmt == 4 && _pos.e1 < GAME_WINDOW_WIDTH - 2) _pos.e1++;
}

bool RPSGameObject::intersect(ICollider *other) {
  RPSGameObject* newOther = dynamic_cast<RPSGameObject*>(other);
  // if(abs(newOther->getPosition().e1 - this->getPosition().e1) <= 1 && abs(newOther->getPosition().y() - this->getPosition().y()) <= 1) return true;
  if(abs(newOther->getPosition().e1 - this->getPosition().e1) - 1 <= 1 && newOther->getPosition().y() == this->getPosition().y()) return true;
  else if(newOther->getPosition().e1 == this->getPosition().e1 && abs(newOther->getPosition().e2 - this->getPosition().e2) == 1) return true;
  return false;
}

void RPSGameObject::onCollision(ICollider *other) {
  RPSGameObject* newOther = dynamic_cast<RPSGameObject*>(other);
  switch(newOther->getType()) {
      case ROCK:
        if(this->getType() == SCISSORS) {
          this->setType(ROCK);
        } else if(this->getType() == PAPER) {
          newOther->setType(PAPER);
          if(newOther->getControl()) newOther->unsetControl();
        }
        return;

      case PAPER:
        if(this->getType() == SCISSORS) {
          newOther->setType(SCISSORS);
        } else if(this->getType() == ROCK) {
          this->setType(PAPER);
          this->unsetControl();
        }
        return;

      case SCISSORS:
        if(this->getType() == ROCK) {
          newOther->setType(SCISSORS);
        } else if(this->getType() == PAPER) {
          this->setType(SCISSORS);
        }
        return;
    }
}