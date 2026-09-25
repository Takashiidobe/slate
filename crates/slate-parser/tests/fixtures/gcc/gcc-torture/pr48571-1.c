#define S (sizeof(int))

unsigned int                   c[624];
void __attribute__((noinline)) bar(void) {
  unsigned int i;
  /* Obfuscated c[i] = c[i-1] * 2.  */
  for (i = 1; i < 624; ++i)
    *(unsigned int *)((void *)c + (__SIZE_TYPE__)i * S) =
        2 * *(unsigned int *)((void *)c +
                              ((__SIZE_TYPE__)i + ((__SIZE_TYPE__)-S) / S) * S);
}
extern void abort(void);
int         main() {
  unsigned int i, j;
  for (i = 0; i < 624; ++i)
    c[i] = 1;
  bar();
  j = 1;
  for (i = 0; i < 624; ++i) {
    if (c[i] != j)
      abort();
    j = j * 2;
  }
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
// DEFAULT-NEXT:     global %0 c: array<u32, 624> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @bar() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %2 i: u32 [storage=automatic];
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%2, reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%2), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(624)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %10: u32 [synthetic] = read<u32>(%2);
// DEFAULT-NEXT:                 let %11: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%2, read<u32>(%11));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(array_decay<ptr<u32>, length=Some(624)>(%0)), mul<u64, overflow=wrap>(widen<u64, reason=explicit>(read<u32>(%2)), const<u64>(4))))), mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)), read<u32>(deref(pointer_cast<ptr<u32>, reason=explicit>(ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(pointer_cast<ptr<void>, reason=explicit>(array_decay<ptr<u32>, length=Some(624)>(%0)), mul<u64, overflow=wrap>(add<u64, overflow=wrap>(widen<u64, reason=explicit>(read<u32>(%2)), div<u64, by_zero=ub>(neg<u64, overflow=wrap>(const<u64>(4)), const<u64>(4))), const<u64>(4))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 i: u32 [storage=automatic];
// DEFAULT-NEXT:         let %6 j: u32 [storage=automatic];
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%5, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(624)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                 let %13: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%12), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%5, read<u32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(624)>(%0), read<u32>(%5))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         write<u32>(%6, reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         for %9
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%5, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(624)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                 let %15: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%14), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%5, read<u32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(624)>(%0), read<u32>(%5)))), read<u32>(%6))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:                     write<u32>(%6, mul<u32, overflow=wrap>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
