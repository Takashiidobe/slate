#include <stdarg.h>

// SLATE-FILECHECK-DEFINES DEFAULT

void abort(void);
void exit(int);

struct tiny {
  char c;
  char d;
};

void f(int n, ...) {
  struct tiny x;
  int         i;

  va_list ap;
  va_start(ap, n);
  for (i = 0; i < n; i++) {
    x = va_arg(ap, struct tiny);
    if (x.c != i + 10)
      abort();
    if (x.d != i + 20)
      abort();
  }
  {
    long x = va_arg(ap, long);
    if (x != 123)
      abort();
  }
  va_end(ap);
}

int main(void) {
  struct tiny x[3];
  x[0].c = 10;
  x[1].c = 11;
  x[2].c = 12;
  x[0].d = 20;
  x[1].d = 21;
  x[2].d = 22;
  f(3, x[0], x[1], x[2], (long)123);
  exit(0);
}

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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     type @type1 tiny = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 d: i8;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @exit(%12 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @f(%5 n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 x: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%8);
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%7, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%7), read<i32>(%5))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%7);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%7, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<@type1>(%6, copy<@type1, reason=assign>(va_arg<@type1>(%8)));
// DEFAULT-NEXT:                     copy<@type1, reason=assign>(va_arg<@type1>(%8));
// DEFAULT-NEXT:                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%6))), add<i32, overflow=ub>(read<i32>(%7), const<i32>(10)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(field1(%6))), add<i32, overflow=ub>(read<i32>(%7), const<i32>(20)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %9 x: i64 [storage=automatic] = va_arg<i64>(%8);
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%9), widen<i64, reason=usual_arith>(const<i32>(123)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         va_end(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 x: array<@type1, 3> [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(3)>(%11), const<i32>(0)))), truncate<i8, reason=assign, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:         write<i8>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(3)>(%11), const<i32>(1)))), truncate<i8, reason=assign, fits=always>(const<i32>(11)));
// DEFAULT-NEXT:         write<i8>(field0(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(3)>(%11), const<i32>(2)))), truncate<i8, reason=assign, fits=always>(const<i32>(12)));
// DEFAULT-NEXT:         write<i8>(field1(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(3)>(%11), const<i32>(0)))), truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(field1(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(3)>(%11), const<i32>(1)))), truncate<i8, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<i8>(field1(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(3)>(%11), const<i32>(2)))), truncate<i8, reason=assign, fits=always>(const<i32>(22)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, coerce<i16>, coerce<i16>, coerce<i16>, scalar) -> void>(%4, const<i32>(3), copy<@type1, reason=vararg>(read<@type1>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(3)>(%11), const<i32>(0))))), copy<@type1, reason=vararg>(read<@type1>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(3)>(%11), const<i32>(1))))), copy<@type1, reason=vararg>(read<@type1>(deref(ptr_offset<ptr<@type1>, subtract=false, element=@type1, overflow=ub>(array_decay<ptr<@type1>, length=Some(3)>(%11), const<i32>(2))))), widen<i64, reason=explicit>(const<i32>(123)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
