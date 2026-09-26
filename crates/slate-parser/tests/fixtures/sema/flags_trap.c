// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -ftrapv

void operations(void) {
    1 + (2 + 3);
    1U + 2U;
    1.0f + 2.0f;
    1.0L + 2.0L;
    1 * 2;
    1 / 2;
    1 % 2;
    1 << 2;
    1 & 2;
    -1;
    -1U;
    ~1;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: add<i32, overflow=trap>(const<i32>(1), add<i32, overflow=trap>(const<i32>(2), const<i32>(3)))
// CHECK-NEXT: add<u32, overflow=wrap>(const<u32>(1), const<u32>(2))
// CHECK-NEXT: add<f32, rounding=nearest_even, exceptions=ignore, contract=on>(const<f32>(1.0), const<f32>(2.0))
// CHECK-NEXT: add<f80, rounding=nearest_even, exceptions=ignore, contract=on>(const<f80>(1), const<f80>(2))
// CHECK-NEXT: mul<i32, overflow=trap>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: rem<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: and<i32>(const<i32>(1), const<i32>(2))
// CHECK-NEXT: neg<i32, overflow=trap>(const<i32>(1))
// CHECK-NEXT: neg<u32, overflow=wrap>(const<u32>(1))
// CHECK-NEXT: not<i32>(const<i32>(1))
// SLATE-FILECHECK-END CHECK
