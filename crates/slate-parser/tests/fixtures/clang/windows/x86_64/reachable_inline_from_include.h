typedef int fn_t(void);
fn_t typedef_declared;

inline int plain_inline(void) { return 1; }
extern inline int extern_inline(void) { return 2; }
inline int redeclared_inline(void) { return 3; }
int redeclared_inline(void);
int predeclared_inline(void);
inline int predeclared_inline(void) { return 4; }
__attribute__((gnu_inline)) inline int gnu_plain_inline(void) { return 5; }
__attribute__((gnu_inline)) extern inline int gnu_extern_inline(void) { return 6; }
__attribute__((dllexport)) inline int exported_inline(void) { return 7; }
static inline int static_inline(void) { return 8; }
