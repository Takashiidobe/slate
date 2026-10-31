// select-args: --build-target missing
extern int helper_value(void);

int value(void) {
    return VARIANT + helper_value();
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}error: unknown build target missing, the compile commands have: example, shared, static
// CHECK-NEXT: exit: 1
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
