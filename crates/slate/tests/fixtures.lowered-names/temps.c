int next_value(void);

#ifdef EARLY
int early(int a, int b) {
    int total = next_value() + a * next_value() + b;
    while (total > 10) {
        total -= next_value();
    }
    return total;
}
#endif

int later(int a) {
    int total = next_value() + a * next_value();
    for (int i = 0; i < a; i++) {
        if (total > 100) {
            break;
        }
        total += next_value() * i;
    }
    return total;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}feature early
// CHECK-NEXT: + extern "C-unwind" fn early(a: i32, b: i32) -> i32
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
