/* Copyright (C) 2000  Free Software Foundation.

   Ensure builtin strlen, strcmp, strchr, strrchr and strncpy
   perform correctly.

   Written by Jakub Jelinek, 11/7/2000.  */

extern void          abort(void);
extern __SIZE_TYPE__ strlen(const char *);
extern int           strcmp(const char *, const char *);
extern char         *strchr(const char *, int);
extern char         *strrchr(const char *, int);
extern char         *strncpy(char *, const char *, __SIZE_TYPE__);
extern void         *memset(void *, int, __SIZE_TYPE__);
extern int           memcmp(const void *, const void *, __SIZE_TYPE__);

int   x   = 6;
int   y   = 1;
char *bar = "hi world";
char  buf[64];

int main() {
  const char *const foo = "hello world";
  char              dst[64];

  if (strlen(bar) != 8)
    abort();
  if (strlen(bar + (++x & 2)) != 6)
    abort();
  if (x != 7)
    abort();
  if (strlen(foo + (x++, 6)) != 5)
    abort();
  if (x != 8)
    abort();
  if (strlen(foo + (++x & 1)) != 10)
    abort();
  if (x != 9)
    abort();
  if (strcmp(foo + (x -= 6), "lo world"))
    abort();
  if (x != 3)
    abort();
  if (strcmp(foo, bar) >= 0)
    abort();
  if (strcmp(foo, bar + (x++ & 1)) >= 0)
    abort();
  if (x != 4)
    abort();
  if (strchr(foo + (x++ & 7), 'l') != foo + 9)
    abort();
  if (x != 5)
    abort();
  if (strchr(bar, 'o') != bar + 4)
    abort();
  if (strchr(bar, '\0') != bar + 8)
    abort();
  if (strrchr(bar, 'x'))
    abort();
  if (strrchr(bar, 'o') != bar + 4)
    abort();
  if (strcmp(foo + (x++ & 1), "ello world" + (--y & 1)))
    abort();
  if (x != 6 || y != 0)
    abort();
  dst[5] = ' ';
  dst[6] = '\0';
  x      = 5;
  y      = 1;
  if (strncpy(dst + 1, foo + (x++ & 3), 4) != dst + 1 || x != 6 ||
      strcmp(dst + 1, "ello "))
    abort();
  memset(dst, ' ', sizeof dst);
  if (strncpy(dst + (++x & 1), (y++ & 3) + "foo", 10) != dst + 1 || x != 7 ||
      y != 2 || memcmp(dst, " oo\0\0\0\0\0\0\0\0 ", 12))
    abort();
  memset(dst, ' ', sizeof dst);
  if (strncpy(dst, "hello", 8) != dst || memcmp(dst, "hello\0\0\0 ", 9))
    abort();
  x = '!';
  memset(buf, ' ', sizeof buf);
  if (memset(buf, x++, ++y) != buf || x != '!' + 1 || y != 3 ||
      memcmp(buf, "!!!", 3))
    abort();
  if (memset(buf + y++, '-', 8) != buf + 3 || y != 4 ||
      memcmp(buf, "!!!--------", 11))
    abort();
  x = 10;
  if (memset(buf + ++x, 0, y++) != buf + 11 || x != 11 || y != 5 ||
      memcmp(buf + 8, "---\0\0\0", 7))
    abort();
  if (memset(buf + (x += 4), 0, 6) != buf + 15 || x != 15 ||
      memcmp(buf + 10, "-\0\0\0\0\0\0\0\0\0", 11))
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
// DEFAULT-NEXT:     global %8 x: i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %9 y: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %31 .str31: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([104, 105, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 bar: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(9)>(%31) [linkage=external];
// DEFAULT-NEXT:     global %11 buf: array<i8, 64> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %32 .str32: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %33 .str33: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([101, 108, 108, 111, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([32, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %39 .str39: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([104, 101, 108, 108, 111, 0, 0, 0, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %40 .str40: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([33, 33, 33, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %41 .str41: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([33, 33, 33, 45, 45, 45, 45, 45, 45, 45, 45, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %42 .str42: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([45, 45, 45, 0, 0, 0, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %43 .str43: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @strlen(%15 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @strcmp(%16 <unnamed>: ptr<const i8>, %17 <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @strchr(%18 <unnamed>: ptr<const i8>, %19 <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %4 @strrchr(%20 <unnamed>: ptr<const i8>, %21 <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %5 @strncpy(%22 <unnamed>: ptr<i8>, %23 <unnamed>: ptr<const i8>, %24 <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %6 @memset(%25 <unnamed>: ptr<void>, %26 <unnamed>: i32, %27 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @memcmp(%28 <unnamed>: ptr<const void>, %29 <unnamed>: ptr<const void>, %30 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 foo: ptr<const i8> [storage=automatic] [const] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%32));
// DEFAULT-NEXT:         let %14 dst: array<i8, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%1, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%10))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %44: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%45));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%1, pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), and<i32>(read<i32>(%45), const<i32>(2))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %46: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%47));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%1, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%13), const<i32>(6))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %48: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%49));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%1, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%13), and<i32>(read<i32>(%49), const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %50: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %51: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%50), const<i32>(6));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%51));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%2, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%13), read<i32>(%51)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%33))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ge<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%2, read<ptr<const i8>>(%13), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %52: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %53: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%52), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%53));
// DEFAULT-NEXT:         if ge<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%2, read<ptr<const i8>>(%13), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), and<i32>(read<i32>(%52), const<i32>(1))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %54: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %55: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%54), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%55));
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%3, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%13), and<i32>(read<i32>(%54), const<i32>(7))), const<i32>(108)), pointer_cast<ptr<i8>, reason=usual_arith>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%13), const<i32>(9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%8), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%10)), const<i32>(111)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%10)), const<i32>(0)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), const<i32>(8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%10)), const<i32>(120)), null<ptr<i8>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%10)), const<i32>(111)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %56: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %57: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%56), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%57));
// DEFAULT-NEXT:         let %58: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %59: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%58), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%59));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%2, ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%13), and<i32>(read<i32>(%56), const<i32>(1))), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%34), and<i32>(read<i32>(%59), const<i32>(1))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%8), const<i32>(6)), ne<i32>(read<i32>(%9), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%14), const<i32>(5))), truncate<i8, reason=assign, fits=always>(const<i32>(32)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%14), const<i32>(6))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%8, const<i32>(5));
// DEFAULT-NEXT:         write<i32>(%9, const<i32>(1));
// DEFAULT-NEXT:         let %60: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %61: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%60), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%61));
// DEFAULT-NEXT:         let %62: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%5, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%14), const<i32>(1)), ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%13), and<i32>(read<i32>(%60), const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%14), const<i32>(1))), ne<i32>(read<i32>(%8), const<i32>(6)))
// DEFAULT-NEXT:             write<bool>(%62, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%62, ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%14), const<i32>(1))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%35))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%62)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%14)), const<i32>(32), const<u64>(64));
// DEFAULT-NEXT:         let %63: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %64: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%63), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%64));
// DEFAULT-NEXT:         let %65: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %66: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%65), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%66));
// DEFAULT-NEXT:         let %67: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%5, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%14), and<i32>(read<i32>(%64), const<i32>(1))), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%36), and<i32>(read<i32>(%65), const<i32>(3)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%14), const<i32>(1))), ne<i32>(read<i32>(%8), const<i32>(7))), ne<i32>(read<i32>(%9), const<i32>(2)))
// DEFAULT-NEXT:             write<bool>(%67, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%67, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%7, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%14)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%37)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%67)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%14)), const<i32>(32), const<u64>(64));
// DEFAULT-NEXT:         let %68: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%5, array_decay<ptr<i8>, length=Some(64)>(%14), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%38)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), array_decay<ptr<i8>, length=Some(64)>(%14))
// DEFAULT-NEXT:             write<bool>(%68, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%68, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%7, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%14)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%39)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%68)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%8, const<i32>(33));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%11)), const<i32>(32), const<u64>(64));
// DEFAULT-NEXT:         let %69: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %70: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%69), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%70));
// DEFAULT-NEXT:         let %71: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %72: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%71), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%72));
// DEFAULT-NEXT:         let %73: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%11)), read<i32>(%69), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%72)))), pointer_cast<ptr<void>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(64)>(%11))), ne<i32>(read<i32>(%8), add<i32, overflow=ub>(const<i32>(33), const<i32>(1)))), ne<i32>(read<i32>(%9), const<i32>(3)))
// DEFAULT-NEXT:             write<bool>(%73, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%73, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%7, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%11)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%40)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%73)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %74: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %75: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%74), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%75));
// DEFAULT-NEXT:         let %76: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%11), read<i32>(%74))), const<i32>(45), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%11), const<i32>(3)))), ne<i32>(read<i32>(%9), const<i32>(4)))
// DEFAULT-NEXT:             write<bool>(%76, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%76, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%7, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%11)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%41)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%76)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%8, const<i32>(10));
// DEFAULT-NEXT:         let %77: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %78: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%77), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%78));
// DEFAULT-NEXT:         let %79: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:         let %80: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%79), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%9, read<i32>(%80));
// DEFAULT-NEXT:         let %81: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%11), read<i32>(%78))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%79)))), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%11), const<i32>(11)))), ne<i32>(read<i32>(%8), const<i32>(11))), ne<i32>(read<i32>(%9), const<i32>(5)))
// DEFAULT-NEXT:             write<bool>(%81, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%81, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%7, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%11), const<i32>(8))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%42)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%81)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %82: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         let %83: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%82), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(%83));
// DEFAULT-NEXT:         let %84: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%11), read<i32>(%83))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%11), const<i32>(15)))), ne<i32>(read<i32>(%8), const<i32>(15)))
// DEFAULT-NEXT:             write<bool>(%84, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%84, ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%7, pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%11), const<i32>(10))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%43)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%84)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
