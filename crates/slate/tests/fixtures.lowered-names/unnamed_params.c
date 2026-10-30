#ifdef EARLY
int external(int fd, char *name);
int unnamed_define(int, char *);
#else
int external(int, char *);
int unnamed_define(int descriptor, char *);
#endif

int use_external(void) {
    return external(1, 0) + unnamed_define(2, 0);
}

int unnamed_define(int a, char *b) {
    return a + (b != 0);
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}feature early
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
