#ifdef EARLY
int early_static(void) {
    static int count;
    return ++count;
}
#endif

int later_static(void) {
    static int count;
    return ++count;
}

int other_static(void) {
    static int count;
    return count--;
}

int shadow_count;

int shadowing(void) {
    int result = shadow_count;
    {
        int shadow_count = 3;
        result += shadow_count;
    }
    return result;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}feature early
// CHECK-NEXT: + extern "C-unwind" fn early_static() -> i32
// CHECK-NEXT: + static mut count__3: i32 = unsafe { std::mem::zeroed() };
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
