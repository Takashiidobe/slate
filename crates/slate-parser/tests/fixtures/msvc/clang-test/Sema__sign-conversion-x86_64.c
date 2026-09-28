
// PR9345: make a subgroup of -Wconversion for signedness changes

void test(int x) {
  unsigned t0 = x; // expected-warning {{implicit conversion changes signedness}}
  unsigned t1 = (t0 == 5 ? x : 0); // expected-warning {{operand of ? changes signedness}}

  // Clang has special treatment for left shift of literal '1'.
  // Make sure there is no diagnostics.
  long t2 = 1LL << x;
}

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT -Wsign-conversion

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
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
// DEFAULT-NEXT:     fn %0 @test(%1 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 t0: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=unknown>(read<i32>(%1));
// DEFAULT-NEXT:         let %3 t1: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=unknown>(conditional<i32>(eq<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(5))), read<i32>(%1), const<i32>(0)));
// DEFAULT-NEXT:         let %4 t2: i32 [storage=automatic] = truncate<i32, reason=assign, fits=unknown>(shl<i64, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i64>(1), read<i32>(%1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
