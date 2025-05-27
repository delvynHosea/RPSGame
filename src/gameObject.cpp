#include "gameObject.h"
#include "environment.h"

Position GameObject::getPosition() const {
  return _pos;
}

Icon GameObject::getIcon() const {
  return _icon;
}

void GameObject::update(int mvmt) {
  if(mvmt == 1 && _pos.e2 > 0) _pos.e2--;
  if(mvmt == 2 && _pos.e2 < GAME_WINDOW_HEIGHT - 1) _pos.e2++;
  if(mvmt == 3 && _pos.e1 > 0) _pos.e1--;
  if(mvmt == 4 && _pos.e1 < GAME_WINDOW_WIDTH - 1) _pos.e1++;
}