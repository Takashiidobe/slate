extern int helper_value(void);

int value(void) {
    return VARIANT + helper_value();
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}error: build target objs compiles {{.*}}tests/fixtures.select/same_directory.c with different arguments
// CHECK-NEXT: exit: 1
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
