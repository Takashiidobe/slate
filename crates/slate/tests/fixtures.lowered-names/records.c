#ifdef EARLY
typedef struct {
    int first;
} early_record;
early_record early_value;
#endif

typedef struct {
    int second;
} later_record;
later_record later_value;

int read_later(void) {
    later_record local = later_value;
    return local.second;
}

int same_name_a(void) {
    struct scoped {
        int a;
    } value = {1};
    return value.a;
}

int same_name_b(void) {
    struct scoped {
        long b;
    } value = {2};
    return (int)value.b;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}feature early
// CHECK-NEXT: + struct __SlateRecord_records_c_4
// CHECK-NEXT: + static mut early_value: __SlateRecord_records_c_4 = unsafe { std::mem::zeroed() };
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
