/* PR debug/67192 */
/* { dg-do run } */
/* { dg-options "-g -Wmisleading-indentation" } */

volatile int cnt = 0;

__attribute__((noinline, noclone)) static int last(void) {
  return ++cnt % 5 == 0;
}

__attribute__((noinline, noclone)) static void do_it(void) {
  asm volatile("" : : "r"(&cnt) : "memory");
}

__attribute__((noinline, noclone)) static void f1(void) {
  for (;; do_it()) {
    if (last())
      break;
  }
  do_it(); /* { dg-final { gdb-test . "cnt" "5" } } */
}

__attribute__((noinline, noclone)) static void f2(void) {
  while (1) {
    if (last())
      break;
    do_it();
  }
  do_it(); /* { dg-final { gdb-test . "cnt" "10" } } */
}

__attribute__((noinline, noclone)) static void f3(void) {
  for (;; do_it())
    if (last())
      break;
  do_it(); /* { dg-final { gdb-test . "cnt" "15" } } */
}

__attribute__((noinline, noclone)) static void f4(void) {
  while (1) /* { dg-final { gdb-test . "cnt" "15" } } */
    if (last())
      break;
    else
      do_it();
  do_it(); /* { dg-final { gdb-test . "cnt" "20" } } */
}

void (*volatile fnp1)(void) = f1;
void (*volatile fnp2)(void) = f2;
void (*volatile fnp3)(void) = f3;
void (*volatile fnp4)(void) = f4;

int
main() {
  asm volatile("" : : "r"(&fnp1) : "memory");
  asm volatile("" : : "r"(&fnp2) : "memory");
  asm volatile("" : : "r"(&fnp3) : "memory");
  asm volatile("" : : "r"(&fnp4) : "memory");
  fnp1();
  fnp2();
  fnp3();
  fnp4();
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
// DEFAULT-NEXT:     global %0 cnt: volatile i32 [storage=static] = const<i32>(0) [linkage=external];
// DEFAULT-NEXT:     global %7 fnp1: volatile ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%3) [linkage=external];
// DEFAULT-NEXT:     global %8 fnp2: volatile ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%4) [linkage=external];
// DEFAULT-NEXT:     global %9 fnp3: volatile ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%5) [linkage=external];
// DEFAULT-NEXT:     global %10 fnp4: volatile ptr<fn() -> void> [storage=static] = function_decay<ptr<fn() -> void>>(%6) [linkage=external];
// DEFAULT-NEXT:     fn %1 @last() -> i32 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16: i32 [synthetic] = read<i32, volatile>(%0);
// DEFAULT-NEXT:         let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:         write<i32, volatile>(%0, read<i32>(%17));
// DEFAULT-NEXT:         return from_bool<i32, reason=return>(eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%17), const<i32>(5)), const<i32>(0)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @do_it() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" addr_of<ptr<volatile i32>>(%0);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @f1() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn() -> i32>(%1), const<i32>(0))
// DEFAULT-NEXT:                         break %12;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f2() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %13 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn() -> i32>(%1), const<i32>(0))
// DEFAULT-NEXT:                     break %13;
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @f3() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn() -> i32>(%1), const<i32>(0))
// DEFAULT-NEXT:                     break %14;
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @f4() -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         while %15 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             if ne<i32>(call<i32, signature=fn() -> i32>(%1), const<i32>(0))
// DEFAULT-NEXT:                 break %15;
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" addr_of<ptr<volatile ptr<fn() -> void>>>(%7);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" addr_of<ptr<volatile ptr<fn() -> void>>>(%8);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" addr_of<ptr<volatile ptr<fn() -> void>>>(%9);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         asm volatile "" {
// DEFAULT-NEXT:             in 0 "r" addr_of<ptr<volatile ptr<fn() -> void>>>(%10);
// DEFAULT-NEXT:             clobbers: memory;
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>, volatile>(%7));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>, volatile>(%8));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>, volatile>(%9));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(read<ptr<fn() -> void>, volatile>(%10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
