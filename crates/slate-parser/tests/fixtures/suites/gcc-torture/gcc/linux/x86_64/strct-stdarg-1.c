#include <stdarg.h>

void abort(void);
void exit(int);

struct tiny {
  char c;
  char d;
  char e;
  char f;
  char g;
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
    if (x.e != i + 30)
      abort();
    if (x.f != i + 40)
      abort();
    if (x.g != i + 50)
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
  x[0].e = 30;
  x[1].e = 31;
  x[2].e = 32;
  x[0].f = 40;
  x[1].f = 41;
  x[2].f = 42;
  x[0].g = 50;
  x[1].g = 51;
  x[2].g = 52;
  f(3, x[0], x[1], x[2], (long)123);
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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     type @type2 tiny = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 d: i8;
// DEFAULT-NEXT:         field2 e: i8;
// DEFAULT-NEXT:         field3 f: i8;
// DEFAULT-NEXT:         field4 g: i8;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 1, 2, 3, 4]];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%13 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @f(%6 n: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 x: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %9 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%9);
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%8), read<i32>(%6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%8, read<i32>(%16));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<@type2>(%7, copy<@type2, reason=assign>(va_arg<@type2>(%9)));
// DEFAULT-NEXT:                     copy<@type2, reason=assign>(va_arg<@type2>(%9));
// DEFAULT-NEXT:                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%7))), add<i32, overflow=ub>(read<i32>(%8), const<i32>(10)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(field1(%7))), add<i32, overflow=ub>(read<i32>(%8), const<i32>(20)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(field2(%7))), add<i32, overflow=ub>(read<i32>(%8), const<i32>(30)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(field3(%7))), add<i32, overflow=ub>(read<i32>(%8), const<i32>(40)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                     if ne<i32>(widen<i32, reason=promotion>(read<i8>(field4(%7))), add<i32, overflow=ub>(read<i32>(%8), const<i32>(50)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %10 x: i64 [storage=automatic] = va_arg<i64>(%9);
// DEFAULT-NEXT:             if ne<i64>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(123)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         va_end(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 x: array<@type2, 3> [storage=automatic];
// DEFAULT-NEXT:         write<i8>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(0)))), truncate<i8, reason=assign, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:         write<i8>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(1)))), truncate<i8, reason=assign, fits=always>(const<i32>(11)));
// DEFAULT-NEXT:         write<i8>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(2)))), truncate<i8, reason=assign, fits=always>(const<i32>(12)));
// DEFAULT-NEXT:         write<i8>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(0)))), truncate<i8, reason=assign, fits=always>(const<i32>(20)));
// DEFAULT-NEXT:         write<i8>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(1)))), truncate<i8, reason=assign, fits=always>(const<i32>(21)));
// DEFAULT-NEXT:         write<i8>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(2)))), truncate<i8, reason=assign, fits=always>(const<i32>(22)));
// DEFAULT-NEXT:         write<i8>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(0)))), truncate<i8, reason=assign, fits=always>(const<i32>(30)));
// DEFAULT-NEXT:         write<i8>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(1)))), truncate<i8, reason=assign, fits=always>(const<i32>(31)));
// DEFAULT-NEXT:         write<i8>(field2(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(2)))), truncate<i8, reason=assign, fits=always>(const<i32>(32)));
// DEFAULT-NEXT:         write<i8>(field3(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(0)))), truncate<i8, reason=assign, fits=always>(const<i32>(40)));
// DEFAULT-NEXT:         write<i8>(field3(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(1)))), truncate<i8, reason=assign, fits=always>(const<i32>(41)));
// DEFAULT-NEXT:         write<i8>(field3(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(2)))), truncate<i8, reason=assign, fits=always>(const<i32>(42)));
// DEFAULT-NEXT:         write<i8>(field4(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(0)))), truncate<i8, reason=assign, fits=always>(const<i32>(50)));
// DEFAULT-NEXT:         write<i8>(field4(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(1)))), truncate<i8, reason=assign, fits=always>(const<i32>(51)));
// DEFAULT-NEXT:         write<i8>(field4(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(2)))), truncate<i8, reason=assign, fits=always>(const<i32>(52)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c, native_c, native_c, scalar) -> void>(%5, const<i32>(3), copy<@type2, reason=vararg>(read<@type2>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(0))))), copy<@type2, reason=vararg>(read<@type2>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(1))))), copy<@type2, reason=vararg>(read<@type2>(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(3)>(%12), const<i32>(2))))), widen<i64, reason=explicit>(const<i32>(123)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
