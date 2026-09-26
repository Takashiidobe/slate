/* { dg-options "-ftree-loop-distribution" } */
extern void  abort(void);
extern void *memset(void *s, int c, __SIZE_TYPE__ n);
extern int   memcmp(const void *s1, const void *s2, __SIZE_TYPE__ n);
/*extern int printf(const char *format, ...);*/

int main() {
  char A[30], B[30], C[30];
  int  i;

  /* prepare arrays */
  memset(A, 1, 30);
  memset(B, 1, 30);

  for (i = 20; i-- > 10;) {
    A[i] = 0;
    B[i] = 0;
  }

  /* expected result */
  memset(C, 1, 30);
  memset(C + 10, 0, 10);

  /* show result */
  /*  for (i = 0; i < 30; i++)
      printf("%d %d %d\n", A[i], B[i], C[i]); */

  /* compare results */
  if (memcmp(A, C, 30) || memcmp(B, C, 30))
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @memset(%8 s: ptr<void>, %9 c: i32, %10 n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @memcmp(%11 s1: ptr<const void>, %12 s2: ptr<const void>, %13 n: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 A: array<i8, 30> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %5 B: array<i8, 30> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %6 C: array<i8, 30> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%4)), const<i32>(1), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(30))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%5)), const<i32>(1), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(30))));
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(20));
// DEFAULT-NEXT:             condition: {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%16));
// DEFAULT-NEXT:                 yield gt<i32>(read<i32>(%15), const<i32>(10));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(30)>(%4), read<i32>(%7))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(30)>(%5), read<i32>(%7))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%6)), const<i32>(1), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(30))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(30)>(%6), const<i32>(10))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10))));
// DEFAULT-NEXT:         let %17: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%4)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(30)))), const<i32>(0))
// DEFAULT-NEXT:             write<bool>(%17, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%17, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%2, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%5)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(30)>(%6)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(30)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%17)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
