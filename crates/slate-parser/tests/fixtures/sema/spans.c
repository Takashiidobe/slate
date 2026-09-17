// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions --show-spans

#define NUMBER 42
#define ADD(x) (x + NUMBER)
void spans(void) {
    1 + (2 + 3);
    ADD(7);
    !NUMBER;
    (bool)NUMBER;
    (double)(NUMBER < 1);
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: add<i32, overflow=ub>(const<i32>(1) [spelling=3:70+1, expansion=3:70+1], add<i32, overflow=ub>(const<i32>(2) [spelling=3:75+1, expansion=3:75+1], const<i32>(3) [spelling=3:79+1, expansion=3:79+1]) [spelling=3:75+5, expansion=3:75+5]) [spelling=3:70+11, expansion=3:70+11]
// CHECK-NEXT: add<i32, overflow=ub>(const<i32>(7) [spelling=3:91+1, expansion=3:91+1], const<i32>(42) [spelling=3:16+2, expansion=3:87+3]) [spelling=3:16+2, expansion=3:87+3]
// CHECK-NEXT: not<bool>(ne<i32>(const<i32>(42) [spelling=3:16+2, expansion=3:100+6], const<i32>(0) [spelling=3:16+2, expansion=3:100+6]) [spelling=3:16+2, expansion=3:100+6]) [spelling=3:16+2, expansion=3:99+7]
// CHECK-NEXT: ne<i32, reason=explicit>(const<i32>(42) [spelling=3:16+2, expansion=3:118+6], const<i32>(0) [spelling=3:16+2, expansion=3:118+6]) [spelling=3:16+2, expansion=3:112+12]
// CHECK-NEXT: int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=explicit>(lt<i32>(const<i32>(42) [spelling=3:16+2, expansion=3:139+6], const<i32>(1) [spelling=3:148+1, expansion=3:148+1]) [spelling=3:16+133, expansion=3:139+10]) [spelling=3:16+133, expansion=3:139+10]) [spelling=3:130+20, expansion=3:130+20]
// SLATE-FILECHECK-END CHECK
