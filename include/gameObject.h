#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include "unit.h"
#include "icon.h"
class GameObject{

public:

    Position getPosition() const;
    Icon getIcon() const;
    virtual void update(int);
protected:

    Position _pos;
    Icon _icon;
};

#endif
