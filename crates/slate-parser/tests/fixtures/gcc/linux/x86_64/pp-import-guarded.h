#ifndef PP_IMPORT_GUARDED_H
#define PP_IMPORT_GUARDED_H
#ifdef IMPORT_BUG
#error guarded header re-entered after #import
#endif
struct guarded { int value; };
#endif
