#include <stdio.h>

static int sum_do_while(int n) {
  int total = 0;
  int i     = 1;
  do {
    total += i;
    i++;
  } while (i <= n);
  return total;
}

static int runs_once_when_false(void) {
  int total = 0;
  do {
    total += 7;
  } while (0);
  return total;
}

static int continue_checks_condition(int n) {
  int total = 0;
  int i     = 0;
  do {
    i++;
    if (i % 2 == 0) {
      continue;
    }
    total += i;
  } while (i < n);
  return total;
}

int main(void) {
  printf("%d\n", sum_do_while(5));
  printf("%d\n", sum_do_while(0));
  printf("%d\n", runs_once_when_false());
  printf("%d\n", continue_checks_condition(6));
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
// DEFAULT-NEXT:     global %16 .str16: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @printf(%12 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %1 @sum_do_while(%2 n: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         do %13
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), read<i32>(%4));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%21));
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%23));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while le<i32>(read<i32>(%4), read<i32>(%2));
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @runs_once_when_false() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         do %14
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %24: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(7));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%25));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @continue_checks_condition(%8 n: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         do %15
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%27));
// DEFAULT-NEXT:                 if eq<i32>(rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%10), const<i32>(2)), const<i32>(0))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         continue %15;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %29: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%28), read<i32>(%10));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%29));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while lt<i32>(read<i32>(%10), read<i32>(%8));
// DEFAULT-NEXT:         return read<i32>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%16)), call<i32, signature=fn(i32) -> i32>(%1, const<i32>(5)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), call<i32, signature=fn(i32) -> i32>(%1, const<i32>(0)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%18)), call<i32, signature=fn() -> i32>(%5));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%0, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%19)), call<i32, signature=fn(i32) -> i32>(%7, const<i32>(6)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
