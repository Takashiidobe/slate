// reduced testcase, compile with -O2. Also, with --disable-checking
// gcc produces wrong code.

void abort(void);
int  i;

void g(void) { i = 1; }

void f(int a, int b) {
  int c = 0;
  if (a == 0)
    c = 1;
  if (c)
    return;
  if (c == 1)
    c = 0;
  if (b == 0)
    c = 1;
  if (c)
    g();
}

int main(void) {
  f(1, 0);
  if (i != 1)
    abort();
  return 0;
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
// DEFAULT-NEXT:     global %1 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%1, const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f(%4 a: i32, %5 b: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 c: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             return;
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%6), const<i32>(1))
// DEFAULT-NEXT:             write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%3, const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%1), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
