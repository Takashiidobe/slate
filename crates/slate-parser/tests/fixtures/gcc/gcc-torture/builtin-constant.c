/* PR optimization/8423.  */

void abort(void);
void exit(int);

#define btest(x) __builtin_constant_p(x) ? "1" : "0"

#ifdef __OPTIMIZE__
void foo(char *i) {
  if (*i == '0')
    abort();
}
#else
void foo(char *i) {}
#endif

int main(void) {
  int size = sizeof(int);
  foo(btest(size));
  foo(btest(size));
  exit(0);
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
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
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %7 .str7: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([48, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([49, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([48, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%6 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 i: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 size: i32 [storage=automatic] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%2, conditional<ptr<i8>>(ne<i32>(const<i32>(0), const<i32>(0)), array_decay<ptr<i8>, length=Some(2)>(%7), array_decay<ptr<i8>, length=Some(2)>(%8)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>) -> void>(%2, conditional<ptr<i8>>(ne<i32>(const<i32>(0), const<i32>(0)), array_decay<ptr<i8>, length=Some(2)>(%9), array_decay<ptr<i8>, length=Some(2)>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
