

#include <stdarg.h>
#include <stdio.h>

static int sum(int n, ...) {
  va_list ap;
  va_start(ap, n);
  int total = 0;
  for (int i = 0; i < n; i++) {
    total += va_arg(ap, int);
  }
  va_end(ap);
  return total;
}

static int pick_second(int marker, ...) {
  va_list ap;
  va_start(ap, marker);
  int first  = va_arg(ap, int);
  int second = va_arg(ap, int);
  va_end(ap);
  return marker + first * 10 + second;
}

int main(void) {
  printf("%d\n", sum(4, 10, 20, 30, 40));
  printf("%d\n", pick_second(5, 7, 9));
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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 va_list = va_list;
// DEFAULT-NEXT:     global %17 .str17: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @printf(%15 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @sum(%5 n: i32, ...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%6);
// DEFAULT-NEXT:         let %7 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %8 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), read<i32>(%5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %21: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                     let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), va_arg<i32>(%6));
// DEFAULT-NEXT:                     write<i32>(%7, read<i32>(%22));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         va_end(%6);
// DEFAULT-NEXT:         return read<i32>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @pick_second(%10 marker: i32, ...) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%11);
// DEFAULT-NEXT:         let %12 first: i32 [storage=automatic] = va_arg<i32>(%11);
// DEFAULT-NEXT:         let %13 second: i32 [storage=automatic] = va_arg<i32>(%11);
// DEFAULT-NEXT:         va_end(%11);
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%10), mul<i32, overflow=ub>(read<i32>(%12), const<i32>(10))), read<i32>(%13));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%17)), call<i32, signature=fn(i32, ...) -> i32>(%4, const<i32>(4), const<i32>(10), const<i32>(20), const<i32>(30), const<i32>(40)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%18)), call<i32, signature=fn(i32, ...) -> i32>(%9, const<i32>(5), const<i32>(7), const<i32>(9)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
