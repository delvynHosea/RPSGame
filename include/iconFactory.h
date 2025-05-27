// TODO 
// Finish the declaration of IconFactory that
// 1. offers some methods to create an Icon.
// 2. encapsulates the constructor from the caller. 
#ifndef ICONFACTORY_H
#define ICONFACTORY_H

#include "icon.h"

class iconFactory {
public:
  static Icon createRockIcon(Color, char*);
  static Icon createPaperIcon(Color, char*);
  static Icon createScissorIcon(Color, char*);
};

#endif