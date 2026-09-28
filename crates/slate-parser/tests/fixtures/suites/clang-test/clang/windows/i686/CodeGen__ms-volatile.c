struct foo {
  volatile int x;
};
struct bar {
  int x;
};
typedef _Complex float __declspec(align(8)) baz;

#pragma pack(push)
#pragma pack(1)
struct qux {
   volatile int f;
};
#pragma pack(pop)

void test1(struct foo *p, struct foo *q) {
  *p = *q;
}
void test2(volatile int *p, volatile int *q) {
  *p = *q;
}
void test3(struct foo *p, struct foo *q) {
  p->x = q->x;
}
void test4(volatile struct foo *p, volatile struct foo *q) {
  p->x = q->x;
}
void test5(volatile struct foo *p, volatile struct foo *q) {
  *p = *q;
}
void test6(struct bar *p, struct bar *q) {
  *p = *q;
}
void test7(volatile struct bar *p, volatile struct bar *q) {
  *p = *q;
}
void test8(volatile double *p, volatile double *q) {
  *p = *q;
}
void test9(volatile baz *p, baz *q) {
  *p = *q;
}
void test10(volatile long long *p, volatile long long *q) {
  *p = *q;
}
void test11(volatile float *p, volatile float *q) {
  *p = *q;
}
int test12(struct qux *p) {
  return p->f;
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 x: volatile i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 bar = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type2 baz = complex<f32>;
// DEFAULT-NEXT:     type @type3 qux = struct {
// DEFAULT-NEXT:         field0 f: volatile i32;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0]];
// DEFAULT-NEXT:     fn %4 @test1(%5 p: ptr<@type0>, %6 q: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type0>(deref(read<ptr<@type0>>(%5)), copy<@type0, reason=assign>(read<@type0>(deref(read<ptr<@type0>>(%6)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test2(%8 p: ptr<volatile i32>, %9 q: ptr<volatile i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, volatile>(deref(read<ptr<volatile i32>>(%8)), read<i32, volatile>(deref(read<ptr<volatile i32>>(%9))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @test3(%11 p: ptr<@type0>, %12 q: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, volatile>(field0(deref(read<ptr<@type0>>(%11))), read<i32, volatile>(field0(deref(read<ptr<@type0>>(%12)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @test4(%14 p: ptr<volatile @type0>, %15 q: ptr<volatile @type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, volatile>(field0(deref(read<ptr<volatile @type0>>(%14))), read<i32, volatile>(field0(deref(read<ptr<volatile @type0>>(%15)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @test5(%17 p: ptr<volatile @type0>, %18 q: ptr<volatile @type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type0, volatile>(deref(read<ptr<volatile @type0>>(%17)), copy<@type0, reason=assign>(read<@type0, volatile>(deref(read<ptr<volatile @type0>>(%18)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @test6(%20 p: ptr<@type1>, %21 q: ptr<@type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type1>(deref(read<ptr<@type1>>(%20)), copy<@type1, reason=assign>(read<@type1>(deref(read<ptr<@type1>>(%21)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @test7(%23 p: ptr<volatile @type1>, %24 q: ptr<volatile @type1>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<@type1, volatile>(deref(read<ptr<volatile @type1>>(%23)), copy<@type1, reason=assign>(read<@type1, volatile>(deref(read<ptr<volatile @type1>>(%24)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @test8(%26 p: ptr<volatile f64>, %27 q: ptr<volatile f64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f64, volatile>(deref(read<ptr<volatile f64>>(%26)), read<f64, volatile>(deref(read<ptr<volatile f64>>(%27))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @test9(%29 p: ptr<volatile complex<f32>>, %30 q: ptr<complex<f32>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<complex<f32>, volatile>(deref(read<ptr<volatile complex<f32>>>(%29)), read<complex<f32>>(deref(read<ptr<complex<f32>>>(%30))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @test10(%32 p: ptr<volatile i64>, %33 q: ptr<volatile i64>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64, volatile>(deref(read<ptr<volatile i64>>(%32)), read<i64, volatile>(deref(read<ptr<volatile i64>>(%33))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %34 @test11(%35 p: ptr<volatile f32>, %36 q: ptr<volatile f32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<f32, volatile>(deref(read<ptr<volatile f32>>(%35)), read<f32, volatile>(deref(read<ptr<volatile f32>>(%36))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @test12(%38 p: ptr<@type3>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32, volatile>(field0(deref(read<ptr<@type3>>(%38))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
