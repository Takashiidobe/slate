
void capture(void *);
void test_alloca(int n) {
  capture(_alloca(n));
}

void test_alloca_with_align(int n) {
  capture(__builtin_alloca_with_align(n, 64));
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
// DEFAULT-NEXT:     fn %0 @capture(%5 <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @_alloca(%6 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %1 @test_alloca(%2 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%0, call<ptr<void>, signature=fn(u32) -> ptr<void>>(%7, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%2))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @__builtin_alloca_with_align(%8 <unnamed>: u32, %9 <unnamed>: u32) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @test_alloca_with_align(%4 n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%0, call<ptr<void>, signature=fn(u32, u32) -> ptr<void>>(%10, reinterpret<u32, reason=arg, fits=unknown>(read<i32>(%4)), reinterpret<u32, reason=arg, fits=always>(const<i32>(64))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
