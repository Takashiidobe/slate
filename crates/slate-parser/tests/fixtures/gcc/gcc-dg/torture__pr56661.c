/* { dg-do run } */

__attribute__((noinline, noclone)) void bar(int *b) { b[0] = b[1] = b[2] = 1; }

__attribute__((noinline, noclone)) int baz(int x) {
  if (x != 1)
    __builtin_abort();
}

void foo(int x) {
  if (x == 0) {
    int *b = __builtin_malloc(3 * sizeof(int));
    while (b[0])
      ;
  } else if (x == 1) {
    int  i, j;
    int *b = __builtin_malloc(3 * sizeof(int));
    for (i = 0; i < 2; i++) {
      bar(b);
      for (j = 0; j < 3; ++j)
        baz(b[j]);
      baz(b[0]);
    }
  }
}

int
main() {
  int x = 1;
  asm volatile("" : "+r"(x));
  foo(x);
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
// DEFAULT-NEXT:     fn %0 @bar(%1 b: ptr<i32>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%1), const<i32>(2))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%1), const<i32>(1))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%1), const<i32>(0))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @baz(%3 x: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_malloc(%13 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo(%5 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %6 b: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(4))));
// DEFAULT-NEXT:                 while %15 ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%6), const<i32>(0)))), const<i32>(0))
// DEFAULT-NEXT:                     ;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<i32>(read<i32>(%5), const<i32>(1))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %8 j: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %9 b: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%14, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))), const<u64>(4))));
// DEFAULT-NEXT:                     for %16
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%7), const<i32>(2))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %18: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                             let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%7, read<i32>(%19));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn(ptr<i32>) -> void>(%0, read<ptr<i32>>(%9));
// DEFAULT-NEXT:                                 for %17
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%8), const<i32>(3))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %20: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                                         let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%8, read<i32>(%21));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         call<i32, signature=fn(i32) -> i32>(%2, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), read<i32>(%8)))));
// DEFAULT-NEXT:                                 call<i32, signature=fn(i32) -> i32>(%2, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%9), const<i32>(0)))));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 x: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             out 0 "+r" place<i32>(%11);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, read<i32>(%11));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
