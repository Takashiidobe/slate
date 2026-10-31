extern int helper_value(void);

int value(void) {
    return VARIANT + helper_value();
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}error: compile commands have different arguments for {{.*}}tests/fixtures.select/no_outputs.c and no output paths to choose a build target from
// CHECK-NEXT: exit: 1
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
