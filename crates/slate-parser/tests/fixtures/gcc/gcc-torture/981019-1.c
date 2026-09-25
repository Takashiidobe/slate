extern void abort(void);
extern int  f2(void);
extern int  f3(void);
extern void f1(void);

void ff(int fname, int part, int nparts) {
  if (fname) /* bb 0 */
  {
    if (nparts) /* bb 1 */
      f1();     /* bb 2 */
  } else
    fname = 2; /* bb 3  */

  /* bb 4 is the branch to bb 10
     (bb 10 is physically at the end of the loop) */
  while (f3() /* bb 10 */) {
    if (nparts /* bb 5 */ && f2() /* bb 6 */) {
      f1(); /* bb 7 ... */
      nparts = part;
      if (f3()) /* ... bb 7 */
        f1();   /* bb 8 */
      f1();     /* bb 9 */
      break;
    }
  }

  if (nparts) /* bb 11 */
    f1();     /* bb 12 */
  return;     /* bb 13 */
}

int main(void) {
  ff(0, 1, 0);
  return 0;
}

int f3(void) {
  static int x = 0;
  x            = !x;
  return x;
}
void f1(void) { abort(); }
int  f2(void) { abort(); }


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
// DEFAULT-NEXT:     global %9 x: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @f2() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @f3() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%9, from_bool<i32, reason=assign>(not<bool>(ne<i32>(read<i32>(%9), const<i32>(0)))));
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f1() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @ff(%5 fname: i32, %6 part: i32, %7 nparts: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%5, const<i32>(2));
// DEFAULT-NEXT:         while %10 ne<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %11: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:                     write<bool>(%11, ne<i32>(call<i32, signature=fn() -> i32>(%1), const<i32>(0)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%11, const<bool>(false));
// DEFAULT-NEXT:                 if read<bool>(%11)
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                         write<i32>(%7, read<i32>(%6));
// DEFAULT-NEXT:                         if ne<i32>(call<i32, signature=fn() -> i32>(%2), const<i32>(0))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                         break %10;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32) -> void>(%4, const<i32>(0), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
