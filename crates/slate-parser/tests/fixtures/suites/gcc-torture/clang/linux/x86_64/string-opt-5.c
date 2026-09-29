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
// DEFAULT-NEXT:     global %[[VALUE_x:[0-9]+]] x: i32 [storage=static] = const<i32>(6) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_y:[0-9]+]] y: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([104, 105, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_bar:[0-9]+]] bar: ptr<i8> [storage=static] = array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str]]) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_buf:[0-9]+]] buf: array<i8, 64> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([104, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([101, 108, 108, 111, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 111, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([32, 111, 111, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([104, 101, 108, 108, 111, 0, 0, 0, 32, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([33, 33, 33, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([33, 33, 33, 45, 45, 45, 45, 45, 45, 45, 45, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([45, 45, 45, 0, 0, 0, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 11> [storage=static] = code_units<array<i8, 11>>([45, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_strlen:[0-9]+]] @strlen(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strcmp:[0-9]+]] @strcmp(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strchr:[0-9]+]] @strchr(%[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE4:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strrchr:[0-9]+]] @strrchr(%[[VALUE5:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE6:[0-9]+]] <unnamed>: i32) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_strncpy:[0-9]+]] @strncpy(%[[VALUE7:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE8:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE9:[0-9]+]] <unnamed>: u64) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE10:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE11:[0-9]+]] <unnamed>: i32, %[[VALUE12:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcmp:[0-9]+]] @memcmp(%[[VALUE13:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE14:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE15:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_foo:[0-9]+]] foo: ptr<const i8> [storage=automatic] [const] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:         let %[[VALUE_dst:[0-9]+]] dst: array<i8, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_bar]]))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_bar]]), and<i32>(read<i32>(%[[VALUE17]]), const<i32>(2))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_foo]]), const<i32>(6))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE_strlen]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_foo]]), and<i32>(read<i32>(%[[VALUE21]]), const<i32>(1)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(10))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(9))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(6));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_foo]]), read<i32>(%[[VALUE23]])), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_3]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(3))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ge<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_foo]]), pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_bar]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         if ge<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], read<ptr<const i8>>(%[[VALUE_foo]]), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_bar]]), and<i32>(read<i32>(%[[VALUE24]]), const<i32>(1))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(4))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_foo]]), and<i32>(read<i32>(%[[VALUE26]]), const<i32>(7))), const<i32>(108)), pointer_cast<ptr<i8>, reason=usual_arith>(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_foo]]), const<i32>(9))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_bar]])), const<i32>(111)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_bar]]), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_bar]])), const<i32>(0)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_bar]]), const<i32>(8)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strrchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_bar]])), const<i32>(120)), null<ptr<i8>>)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<const i8>, i32) -> ptr<i8>>(%[[VALUE_strrchr]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%[[VALUE_bar]])), const<i32>(111)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_bar]]), const<i32>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE29:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE28]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:         let %[[VALUE30:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:         let %[[VALUE31:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE30]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE31]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_foo]]), and<i32>(read<i32>(%[[VALUE28]]), const<i32>(1))), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_4]]), and<i32>(read<i32>(%[[VALUE31]]), const<i32>(1))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(6)), ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]]), const<i32>(5))), truncate<i8, reason=assign, fits=always>(const<i32>(32)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]]), const<i32>(6))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], const<i32>(5));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE32:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE33:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE32]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE33]]));
// DEFAULT-NEXT:         let %[[VALUE34:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]]), const<i32>(1)), ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_foo]]), and<i32>(read<i32>(%[[VALUE32]]), const<i32>(3))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4)))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(6)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE34]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE34]], ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(%[[VALUE_strcmp]], pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]]), const<i32>(1))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_5]]))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE34]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]])), const<i32>(32), const<u64>(64));
// DEFAULT-NEXT:         let %[[VALUE35:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE36:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE35]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE36]]));
// DEFAULT-NEXT:         let %[[VALUE37:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:         let %[[VALUE38:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE37]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE38]]));
// DEFAULT-NEXT:         let %[[VALUE39:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]]), and<i32>(read<i32>(%[[VALUE36]]), const<i32>(1))), pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_6]]), and<i32>(read<i32>(%[[VALUE37]]), const<i32>(3)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(10)))), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]]), const<i32>(1))), ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(7))), ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(2)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE39]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE39]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_7]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(12)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE39]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]])), const<i32>(32), const<u64>(64));
// DEFAULT-NEXT:         let %[[VALUE40:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>, u64) -> ptr<i8>>(%[[VALUE_strncpy]], array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]]), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str_8]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]]))
// DEFAULT-NEXT:             write<bool>(%[[VALUE40]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE40]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_str_9]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(9)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE40]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], const<i32>(33));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]])), const<i32>(32), const<u64>(64));
// DEFAULT-NEXT:         let %[[VALUE41:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE42:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE41]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE42]]));
// DEFAULT-NEXT:         let %[[VALUE43:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:         let %[[VALUE44:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE43]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE44]]));
// DEFAULT-NEXT:         let %[[VALUE45:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]])), read<i32>(%[[VALUE41]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE44]])))), pointer_cast<ptr<void>, reason=usual_arith>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]))), ne<i32>(read<i32>(%[[VALUE_x]]), add<i32, overflow=ub>(const<i32>(33), const<i32>(1)))), ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(3)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE45]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE45]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_10]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE45]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE46:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:         let %[[VALUE47:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE46]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE47]]));
// DEFAULT-NEXT:         let %[[VALUE48:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), read<i32>(%[[VALUE46]]))), const<i32>(45), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8)))), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), const<i32>(3)))), ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(4)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE48]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE48]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str_11]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE48]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], const<i32>(10));
// DEFAULT-NEXT:         let %[[VALUE49:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE50:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE49]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE50]]));
// DEFAULT-NEXT:         let %[[VALUE51:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_y]]);
// DEFAULT-NEXT:         let %[[VALUE52:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE51]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_y]], read<i32>(%[[VALUE52]]));
// DEFAULT-NEXT:         let %[[VALUE53:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), read<i32>(%[[VALUE50]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE51]])))), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), const<i32>(11)))), ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(11))), ne<i32>(read<i32>(%[[VALUE_y]]), const<i32>(5)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE53]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE53]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), const<i32>(8))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str_12]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(7)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE53]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE54:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         let %[[VALUE55:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE54]]), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE55]]));
// DEFAULT-NEXT:         let %[[VALUE56:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(ne<ptr<void>>(call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), read<i32>(%[[VALUE55]]))), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(6)))), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), const<i32>(15)))), ne<i32>(read<i32>(%[[VALUE_x]]), const<i32>(15)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE56]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE56]], ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE_memcmp]], pointer_cast<ptr<const void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(64)>(%[[VALUE_buf]]), const<i32>(10))), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(11)>(%[[VALUE_str_13]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(11)))), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE56]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
