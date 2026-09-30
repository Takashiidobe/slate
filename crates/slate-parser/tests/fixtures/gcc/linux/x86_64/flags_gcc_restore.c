// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions -ftrapv -fwrapv -fno-wrapv

void operations(void) {
    1 + (2 + 3);
    1U + 2U;
    1.0f + 2.0f;
    1.0L + 2.0L;
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: add<i32, overflow=trap>(const<i32>(1), add<i32, overflow=trap>(const<i32>(2), const<i32>(3)))
// CHECK-NEXT: add<u32, overflow=wrap>(const<u32>(1), const<u32>(2))
// CHECK-NEXT: add<f32, rounding=nearest_even, exceptions=observable, contract=fast>(const<f32>(1.0), const<f32>(2.0))
// CHECK-NEXT: add<f80, rounding=nearest_even, exceptions=observable, contract=fast>(const<f80>(1), const<f80>(2))
// SLATE-FILECHECK-END CHECK
