int a1, a2 __attribute__((vector_size(16)));
int __attribute__((vector_size(16))) b1, b2;
int d1 __attribute__((vector_size(16))), d2;
int e1, e2 __attribute__((mode(QI)));
int f1, __attribute__((mode(QI))) f2;
int __attribute__((mode(QI))) g1, g2;
int (__attribute__((mode(QI))) i1), i2;
int l1 __attribute__((vector_size(16))) __attribute__((mode(DI))), l2;
typedef int T1, T2 __attribute__((mode(HI)));
struct fields {
  int j1, j2 __attribute__((mode(QI)));
  int k1 __attribute__((vector_size(8))), k2;
};

int locals(void) {
  int x, y __attribute__((mode(QI)));
  static int s1 __attribute__((mode(HI))), s2;
  return sizeof x + sizeof y + sizeof s1 + sizeof s2;
}

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES IR

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 T1 = i32;
// IR-NEXT:     type @type1 T2 = i16;
// IR-NEXT:     type @type2 fields = struct {
// IR-NEXT:         field0 j1: i32;
// IR-NEXT:         field1 j2: i8;
// IR-NEXT:         field2 k1: vector<i32, 2>;
// IR-NEXT:         field3 k2: i32;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 4, 8, 16]];
// IR-NEXT:     global %0 a1: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %1 a2: vector<i32, 4> [storage=static] [linkage=external];
// IR-NEXT:     global %2 b1: vector<i32, 4> [storage=static] [linkage=external];
// IR-NEXT:     global %3 b2: vector<i32, 4> [storage=static] [linkage=external];
// IR-NEXT:     global %4 d1: vector<i32, 4> [storage=static] [linkage=external];
// IR-NEXT:     global %5 d2: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %6 e1: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %7 e2: i8 [storage=static] [linkage=external];
// IR-NEXT:     global %8 f1: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %9 f2: i8 [storage=static] [linkage=external];
// IR-NEXT:     global %10 g1: i8 [storage=static] [linkage=external];
// IR-NEXT:     global %11 g2: i8 [storage=static] [linkage=external];
// IR-NEXT:     global %12 i1: i8 [storage=static] [linkage=external];
// IR-NEXT:     global %13 i2: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %14 l1: vector<i64, 2> [storage=static] [linkage=external];
// IR-NEXT:     global %15 l2: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %22 s1: i16 [storage=static] [linkage=internal];
// IR-NEXT:     global %23 s2: i32 [storage=static] [linkage=internal];
// IR-NEXT:     fn %19 @locals() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %20 x: i32 [storage=automatic];
// IR-NEXT:         let %21 y: i8 [storage=automatic];
// IR-NEXT:         return reinterpret<i32>(truncate<u32>(add<u64>(add<u64>(add<u64>(const<u64>(4), const<u64>(1)), const<u64>(2)), const<u64>(4))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
