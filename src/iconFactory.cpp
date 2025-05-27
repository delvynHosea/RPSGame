// TODO implementation
#include "iconFactory.h"
#include <string.h>
Icon iconFactory::createRockIcon(Color color, char* str) {
  char str2[3] = "R ";
  if(strcmp(str, "[R") == 0) {
    str2[0] = 'R';
    str2[1] = ']';
  }
  return {{{color, str}, {color, str2}}};
}

Icon iconFactory::createPaperIcon(Color color, char* str) {
  return {{{color, " P"}, {color, "P "}}};
}

Icon iconFactory::createScissorIcon(Color color, char* str) {
  return {{{color, " S"}, {color, "S "}}};
}