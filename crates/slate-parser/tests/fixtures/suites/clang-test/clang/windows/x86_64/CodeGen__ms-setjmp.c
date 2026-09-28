typedef char jmp_buf[1];

#ifdef DECLARE_SETJMP
int _setjmp(jmp_buf env);
int _setjmpex(jmp_buf env);
#endif

jmp_buf jb;

int test_setjmp(void) {
  return _setjmp(jb);


}

int test_setjmpex(void) {
  return _setjmpex(jb);

}

// SLATE-FILECHECK-DEFINES DEFAULT -DDECLARE_SETJMP
// SLATE-FILECHECK-STD DEFAULT gnu17

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
// DEFAULT-NEXT:     type @type0 jmp_buf = array<i8, 1>;
// DEFAULT-NEXT:     global %5 jb: array<i8, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @_setjmp(%8 env: ptr<i8> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @_setjmpex(%9 env: ptr<i8> [array=1]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @test_setjmp() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<i8>) -> i32>(%2, array_decay<ptr<i8>, length=Some(1)>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @test_setjmpex() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<i8>) -> i32>(%4, array_decay<ptr<i8>, length=Some(1)>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
