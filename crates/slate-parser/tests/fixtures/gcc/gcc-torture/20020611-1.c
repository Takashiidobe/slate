/* PR target/6997.  Missing (set_attr "cc" "none") in sleu pattern in
   cris.md.  Testcase from hp@axis.com.  */

void abort(void);
void exit(int);

int          p;
int          k;
unsigned int n;

void x() {
  unsigned int h;

  h = n <= 30;
  if (h)
    p = 1;
  else
    p = 0;

  if (h)
    k = 1;
  else
    k = 0;
}

unsigned int n = 30;

int main(void) {
  x();
  if (p != 1 || k != 1)
    abort();
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
// DEFAULT-NEXT:     global %2 p: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 n: u32 [storage=static] = reinterpret<u32, reason=assign, fits=always>(const<i32>(30)) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%8 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @x() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 h: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%6, from_bool<u32, reason=assign>(le<u32>(read<u32>(%4), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(30)))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%6), const<u32>(0))
// DEFAULT-NEXT:             write<i32>(%2, const<i32>(1));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%2, const<i32>(0));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%6), const<u32>(0))
// DEFAULT-NEXT:             write<i32>(%3, const<i32>(1));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%5);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%2), const<i32>(1)), ne<i32>(read<i32>(%3), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
