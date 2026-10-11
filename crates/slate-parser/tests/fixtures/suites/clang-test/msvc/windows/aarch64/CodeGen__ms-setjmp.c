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

// SLATE-FILECHECK-DEFINES CFG0 -DDECLARE_SETJMP
// SLATE-FILECHECK-STD CFG0 c17
// SLATE-FILECHECK-DEFINES CFG1
// SLATE-FILECHECK-STD CFG1 c17
// SLATE-FILECHECK-PREFIX-ARGS CFG1 -Wno-implicit-function-declaration

// SLATE-FILECHECK-BEGIN CFG0
// CFG0: module {
// CFG0-NEXT:     target "aarch64-pc-windows-msvc" {
// CFG0-NEXT:         endian = little;
// CFG0-NEXT:         pointer [size=8, align=8];
// CFG0-NEXT:         stack_alignment = 16;
// CFG0-NEXT:         long_double = f64;
// CFG0-NEXT:         storage bool [size=1, align=1];
// CFG0-NEXT:         storage i8, u8 [size=1, align=1];
// CFG0-NEXT:         storage i16, u16 [size=2, align=2];
// CFG0-NEXT:         storage i32, u32 [size=4, align=4];
// CFG0-NEXT:         storage i64, u64 [size=8, align=8];
// CFG0-NEXT:         storage i128, u128 [size=16, align=16];
// CFG0-NEXT:         storage bf16 [size=2, align=2];
// CFG0-NEXT:         storage f16 [size=2, align=2];
// CFG0-NEXT:         storage f32 [size=4, align=4];
// CFG0-NEXT:         storage f64 [size=8, align=8];
// CFG0-NEXT:         storage f128 [size=16, align=16];
// CFG0-NEXT:         storage d32 [size=4, align=4];
// CFG0-NEXT:         storage d64 [size=8, align=8];
// CFG0-NEXT:         storage d128 [size=16, align=16];
// CFG0-NEXT:     }
// CFG0-NEXT:     type @type[[TYPE_jmp_buf:[0-9]+]] jmp_buf = array<i8, 1>;
// CFG0-NEXT:     global %[[VALUE_jb:[0-9]+]] jb: array<i8, 1> [storage=static] [linkage=external];
// CFG0-NEXT:     fn %[[VALUE__setjmp:[0-9]+]] @_setjmp(%[[VALUE_env:[0-9]+]] env: ptr<i8> [array=1]) -> i32 [linkage=external];
// CFG0-NEXT:     fn %[[VALUE__setjmpex:[0-9]+]] @_setjmpex(%[[VALUE_env_2:[0-9]+]] env: ptr<i8> [array=1]) -> i32 [linkage=external];
// CFG0-NEXT:     fn %[[VALUE_test_setjmp:[0-9]+]] @test_setjmp() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CFG0-NEXT:         return call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE__setjmp]], array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_jb]]));
// CFG0-NEXT:     }
// CFG0-NEXT:     fn %[[VALUE_test_setjmpex:[0-9]+]] @test_setjmpex() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CFG0-NEXT:         return call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE__setjmpex]], array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_jb]]));
// CFG0-NEXT:     }
// CFG0-NEXT: }
// SLATE-FILECHECK-END CFG0
// SLATE-FILECHECK-BEGIN CFG1
// CFG1: module {
// CFG1-NEXT:     target "aarch64-pc-windows-msvc" {
// CFG1-NEXT:         endian = little;
// CFG1-NEXT:         pointer [size=8, align=8];
// CFG1-NEXT:         stack_alignment = 16;
// CFG1-NEXT:         long_double = f64;
// CFG1-NEXT:         storage bool [size=1, align=1];
// CFG1-NEXT:         storage i8, u8 [size=1, align=1];
// CFG1-NEXT:         storage i16, u16 [size=2, align=2];
// CFG1-NEXT:         storage i32, u32 [size=4, align=4];
// CFG1-NEXT:         storage i64, u64 [size=8, align=8];
// CFG1-NEXT:         storage i128, u128 [size=16, align=16];
// CFG1-NEXT:         storage bf16 [size=2, align=2];
// CFG1-NEXT:         storage f16 [size=2, align=2];
// CFG1-NEXT:         storage f32 [size=4, align=4];
// CFG1-NEXT:         storage f64 [size=8, align=8];
// CFG1-NEXT:         storage f128 [size=16, align=16];
// CFG1-NEXT:         storage d32 [size=4, align=4];
// CFG1-NEXT:         storage d64 [size=8, align=8];
// CFG1-NEXT:         storage d128 [size=16, align=16];
// CFG1-NEXT:     }
// CFG1-NEXT:     type @type[[TYPE_jmp_buf:[0-9]+]] jmp_buf = array<i8, 1>;
// CFG1-NEXT:     global %[[VALUE_jb:[0-9]+]] jb: array<i8, 1> [storage=static] [linkage=external];
// CFG1-NEXT:     fn %[[VALUE__setjmp:[0-9]+]] @_setjmp(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i8>) -> i32 [linkage=external];
// CFG1-NEXT:     fn %[[VALUE_test_setjmp:[0-9]+]] @test_setjmp() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CFG1-NEXT:         return call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE__setjmp]], array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_jb]]));
// CFG1-NEXT:     }
// CFG1-NEXT:     fn %[[VALUE__setjmpex:[0-9]+]] @_setjmpex(%[[VALUE1:[0-9]+]] <unnamed>: ptr<i8>) -> i32 [linkage=external];
// CFG1-NEXT:     fn %[[VALUE_test_setjmpex:[0-9]+]] @test_setjmpex() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// CFG1-NEXT:         return call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE__setjmpex]], array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_jb]]));
// CFG1-NEXT:     }
// CFG1-NEXT: }
// SLATE-FILECHECK-END CFG1
