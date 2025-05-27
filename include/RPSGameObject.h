#ifndef RPS_H
#define RPS_H

#include "collider.h"
#include "gameObject.h"
#include "unit.h"
#include "environment.h"
#include "iconFactory.h"

enum RPSType { ROCK, PAPER, SCISSORS, NONE};

class RPSGameObject : public GameObject, public ICollider {

public:
  RPSGameObject(RPSType, int, int, char*, bool);
  void update(int) override;
  void onCollision(ICollider *other) override;
  bool intersect(ICollider *other) override;
  void setControl();
  void unsetControl();
  bool getControl();
  void setIcon();

  RPSType getType() const;
  void setType(RPSType type);

private:
  bool control;
  RPSType _type;
};
#endif