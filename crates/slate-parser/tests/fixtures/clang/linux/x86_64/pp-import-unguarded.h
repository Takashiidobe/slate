#ifdef IMPORT_BUG
#error unguarded header re-entered after #import
#endif
#define IMPORT_BUG
struct unguarded { int value; };
