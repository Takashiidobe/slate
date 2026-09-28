// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions

void expressions(void) {
    1, 2;
    _Generic(1LL, long: 0, long long: 1);
    _Generic(1 < 2, int: 1, _Bool: 0);
    sizeof(1 < 2);
    (1, 1 < 2) && 3;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: sequence<i32>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: const<i32>(1)
// CHECK-NEXT: const<i32>(1)
// CHECK-NEXT: const<u64>(4)
// CHECK-NEXT: logical_and<bool>(ne<i32>(sequence<i32>(const<i32>(1), lt<i32>(const<i32>(1), const<i32>(2))), const<i32>(0)), ne<i32>(const<i32>(3), const<i32>(0)))
// SLATE-FILECHECK-END CHECK
