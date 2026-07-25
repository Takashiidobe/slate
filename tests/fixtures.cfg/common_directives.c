#define DIRECTIVE_VALUE 40
#undef DIRECTIVE_VALUE
#define DIRECTIVE_VALUE 41
#include "common_directives.h"
#import "imported_directive.h"
#ident "slate ident"
#sccs "slate sccs"
#

int main(void) {
  struct included_pair pair = {DIRECTIVE_VALUE, INCLUDED_VALUE};
  return pair.left + pair.right + IMPORTED_VALUE == 46 ? 0 : 1;
}
