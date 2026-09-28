typedef int unused_typedef;
struct unused_tag { int x; };
int declared_only(void);
extern int declared_object;
static int unused_static(void) { return 1; }
inline int unused_inline(void) { return 2; }
static int unused_static_object;

int tentative_object;
extern int initialized_extern = 3;
int external_definition(void) { return 4; }
