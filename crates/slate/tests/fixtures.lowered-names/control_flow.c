int next_value(void);

#ifdef EARLY
int early_jump(int n) {
    int total = 0;
again:
    total += next_value();
    if (total < n) {
        goto again;
    }
    return total;
}
#endif

int later_jump(int n) {
    int total = 0;
    switch (n) {
    case 1:
        total = next_value();
        break;
    default:
        total = 2;
    }
    if (total) {
        goto done;
    }
    {
        int buffer[n];
        buffer[0] = total;
        total = buffer[0];
    }
done:
    return total;
}

int *file_values = (int[]){1, 2, 3};

int later_compound(void) {
    return file_values[1];
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}feature early
// CHECK-NEXT: + extern "C-unwind" fn early_jump(n: i32) -> i32
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
