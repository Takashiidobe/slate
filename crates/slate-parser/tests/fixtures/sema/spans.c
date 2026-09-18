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
// CHECK: add<i32, overflow=ub>(const<i32>(1) [spelling=[[#FILE0:]]:70+1, expansion={{[0-9]+}}:70+1], add<i32, overflow=ub>(const<i32>(2) [spelling={{[0-9]+}}:75+1, expansion={{[0-9]+}}:75+1], const<i32>(3) [spelling={{[0-9]+}}:79+1, expansion={{[0-9]+}}:79+1]) [spelling={{[0-9]+}}:75+5, expansion={{[0-9]+}}:75+5]) [spelling={{[0-9]+}}:70+11, expansion={{[0-9]+}}:70+11]
// CHECK-NEXT: add<i32, overflow=ub>(const<i32>(7) [spelling=[[#FILE0]]:91+1, expansion=[[#FILE0]]:91+1], const<i32>(42) [spelling=[[#FILE0]]:16+2, expansion=[[#FILE0]]:87+3]) [spelling=[[#FILE0]]:16+2, expansion=[[#FILE0]]:87+3]
// CHECK-NEXT: not<bool>(ne<i32>(const<i32>(42) [spelling=[[#FILE0]]:16+2, expansion=[[#FILE0]]:100+6], const<i32>(0) [spelling=[[#FILE0]]:16+2, expansion=[[#FILE0]]:100+6]) [spelling=[[#FILE0]]:16+2, expansion=[[#FILE0]]:100+6]) [spelling=[[#FILE0]]:16+2, expansion=[[#FILE0]]:99+7]
// CHECK-NEXT: ne<i32, reason=explicit>(const<i32>(42) [spelling=[[#FILE0]]:16+2, expansion=[[#FILE0]]:118+6], const<i32>(0) [spelling=[[#FILE0]]:16+2, expansion=[[#FILE0]]:118+6]) [spelling=[[#FILE0]]:16+2, expansion=[[#FILE0]]:112+12]
// CHECK-NEXT: int_to_float<f64, reason=explicit, exact=true, rounding=nearest_even, exceptions=ignore>(from_bool<i32, reason=explicit>(lt<i32>(const<i32>(42) [spelling=[[#FILE0]]:16+2, expansion=[[#FILE0]]:139+6], const<i32>(1) [spelling=[[#FILE0]]:148+1, expansion=[[#FILE0]]:148+1]) [spelling=[[#FILE0]]:16+133, expansion=[[#FILE0]]:139+10]) [spelling=[[#FILE0]]:16+133, expansion=[[#FILE0]]:139+10]) [spelling=[[#FILE0]]:130+20, expansion=[[#FILE0]]:130+20]
// SLATE-FILECHECK-END CHECK
