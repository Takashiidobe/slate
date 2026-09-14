// SLATE-FILECHECK-DEFINES CHECK
// SLATE-FILECHECK-ARGS --dump-ir-expressions --show-spans

#define NUMBER 42
#define ADD(x) (x + NUMBER)
void spans(void) {
    1 + (2 + 3);
    ADD(7);
}

// SLATE-FILECHECK-BEGIN CHECK
// CHECK: add<i32, overflow=undefined>(const<i32>(1) [spelling=3:70+1, expansion=3:70+1], add<i32, overflow=undefined>(const<i32>(2) [spelling=3:75+1, expansion=3:75+1], const<i32>(3) [spelling=3:79+1, expansion=3:79+1]) [spelling=3:75+5, expansion=3:75+5]) [spelling=3:70+11, expansion=3:70+11]
// CHECK-NEXT: add<i32, overflow=undefined>(const<i32>(7) [spelling=3:91+1, expansion=3:91+1], const<i32>(42) [spelling=3:16+2, expansion=3:87+3]) [spelling=3:16+2, expansion=3:87+3]
// SLATE-FILECHECK-END CHECK
