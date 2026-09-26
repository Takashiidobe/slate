/* { dg-do run } */
/* { dg-require-effective-target size32plus } */

/* Large so an out-of-bound read will crash.  */
unsigned char c[0x30001] = {1};
int           j          = 2;

static void foo(unsigned long *x, unsigned char *y) {
  int           i;
  unsigned long w = x[0];
  for (i = 0; i < j; i++) {
    w += *y;
    y += 0x10000;
    w += *y;
    y += 0x10000;
  }
  x[1] = w;
}

__attribute__((noinline, noclone)) void bar(unsigned long *x) { foo(x, c); }

int
main() {
  unsigned long a[2] = {0, -1UL};
  asm volatile("" ::"r"(c) : "memory");
  c[0] = 0;
  bar(a);
  if (a[1] != 0)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %0 c: array<u8, 196609> [storage=static] [align=16] = aggregate<array<u8, 196609>, zero_fill=true>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(1)))) [linkage=external];
// DEFAULT-NEXT:     global %1 j: i32 [storage=static] = const<i32>(2) [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 x: ptr<u64>, %4 y: ptr<u8>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 w: u64 [storage=automatic] = read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%3), const<i32>(0))));
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), read<i32>(%1))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %15: u64 [synthetic] = read<u64>(%6);
// DEFAULT-NEXT:                     let %16: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%15), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%4))))))));
// DEFAULT-NEXT:                     write<u64>(%6, read<u64>(%16));
// DEFAULT-NEXT:                     let %17: ptr<u8> [synthetic] = read<ptr<u8>>(%4);
// DEFAULT-NEXT:                     let %18: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%17), const<i32>(65536));
// DEFAULT-NEXT:                     write<ptr<u8>>(%4, read<ptr<u8>>(%18));
// DEFAULT-NEXT:                     let %19: u64 [synthetic] = read<u64>(%6);
// DEFAULT-NEXT:                     let %20: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%19), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(read<ptr<u8>>(%4))))))));
// DEFAULT-NEXT:                     write<u64>(%6, read<u64>(%20));
// DEFAULT-NEXT:                     let %21: ptr<u8> [synthetic] = read<ptr<u8>>(%4);
// DEFAULT-NEXT:                     let %22: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%21), const<i32>(65536));
// DEFAULT-NEXT:                     write<ptr<u8>>(%4, read<ptr<u8>>(%22));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(read<ptr<u64>>(%3), const<i32>(1))), read<u64>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @bar(%8 x: ptr<u64>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u64>, ptr<u8>) -> void>(%2, read<ptr<u64>>(%8), array_decay<ptr<u8>, length=Some(196609)>(%0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 a: array<u64, 2> [storage=automatic] [align=16] = aggregate<array<u64, 2>, zero_fill=false>(index0 = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))), index1 = neg<u64, overflow=wrap>(const<u64>(1)));
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" array_decay<ptr<u8>, length=Some(196609)>(%0);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(196609)>(%0), const<i32>(0))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u64>) -> void>(%7, array_decay<ptr<u64>, length=Some(2)>(%10));
// DEFAULT-NEXT:         if ne<u64>(read<u64>(deref(ptr_offset<ptr<u64>, subtract=false, element=u64, overflow=ub>(array_decay<ptr<u64>, length=Some(2)>(%10), const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
