int wide(void) {
    int value = 0;
#ifdef F0
    value += 0;
#else
    value -= 0;
#endif
#ifdef F1
    value += 1;
#else
    value -= 1;
#endif
#ifdef F2
    value += 2;
#else
    value -= 2;
#endif
#ifdef F3
    value += 3;
#else
    value -= 3;
#endif
#ifdef F4
    value += 4;
#else
    value -= 4;
#endif
#ifdef F5
    value += 5;
#else
    value -= 5;
#endif
#ifdef F6
    value += 6;
#else
    value -= 6;
#endif
#ifdef F7
    value += 7;
#else
    value -= 7;
#endif
#ifdef F8
    value += 8;
#else
    value -= 8;
#endif
#ifdef F9
    value += 9;
#else
    value -= 9;
#endif
#ifdef F10
    value += 10;
#else
    value -= 10;
#endif
#ifdef F11
    value += 11;
#else
    value -= 11;
#endif
#ifdef F12
    value += 12;
#else
    value -= 12;
#endif
#ifdef F13
    value += 13;
#else
    value -= 13;
#endif
#ifdef F14
    value += 14;
#else
    value -= 14;
#endif
#ifdef F15
    value += 15;
#else
    value -= 15;
#endif
#ifdef F16
    value += 16;
#else
    value -= 16;
#endif
#ifdef F17
    value += 17;
#else
    value -= 17;
#endif
#ifdef F18
    value += 18;
#else
    value -= 18;
#endif
#ifdef F19
    value += 19;
#else
    value -= 19;
#endif
#ifdef F20
    value += 20;
#else
    value -= 20;
#endif
#ifdef F21
    value += 21;
#else
    value -= 21;
#endif
#ifdef F22
    value += 22;
#else
    value -= 22;
#endif
#ifdef F23
    value += 23;
#else
    value -= 23;
#endif
#ifdef F24
    value += 24;
#else
    value -= 24;
#endif
#ifdef F25
    value += 25;
#else
    value -= 25;
#endif
#ifdef F26
    value += 26;
#else
    value -= 26;
#endif
#ifdef F27
    value += 27;
#else
    value -= 27;
#endif
#ifdef F28
    value += 28;
#else
    value -= 28;
#endif
#ifdef F29
    value += 29;
#else
    value -= 29;
#endif
    return value;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}target linux-x86_64: 2 compiles from 467 preprocessor runs, naive 1073741824
// CHECK-NEXT:   wide.c: 2 compiles, 30 relevant, 64 elements, 467 runs
// CHECK-NEXT:     []
// CHECK-NEXT:     [f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,f27,f28,f29]
// CHECK-NEXT: total: 2 compiles across 1 targets
// CHECK-NEXT: exit: 0
// CHECK-NEXT: exhaustive: error: cannot enumerate 30 matrix features, the limit is 16
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
