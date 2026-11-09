#include <stddef.h>
#include <time.h>

#define ASSERT_TYPE(E, T)            \
  do {                               \
    typedef __typeof__(E) actual;    \
    actual **x = 0;                  \
    T **y = 0;                       \
    x = y;                           \
    y = x;                           \
  } while (0)

#define type_if_not(T, E) __typeof__(0 ? (T *)0 : (void *)(E))
#define type_if(T, E) type_if_not(T, !(E))
#define pick(T1, E1, T2) __typeof__(*(__typeof__(0 ? (type_if(T1, (E1)))0 : (type_if(T2, !(E1)))0))0)

void constant_types(void) {
  ASSERT_TYPE(0x7fffffff, int);
  ASSERT_TYPE(0x80000000, unsigned int);
  ASSERT_TYPE(2147483648, long);
  ASSERT_TYPE(0xffffffffu, unsigned int);
  ASSERT_TYPE(0x100000000u, unsigned long);
  ASSERT_TYPE(1l, long);
  ASSERT_TYPE(0b1, int);
  ASSERT_TYPE(0b10000000000000000000000000000000, unsigned int);
  pick(int, 2147483647 <= 2147483647, long) a = 0;
  pick(int, 2147483648 <= 2147483647, long) b = 0;
  ASSERT_TYPE(a, int);
  ASSERT_TYPE(b, long);
}

long *not_null_constant(int *p, long *c) {
  c = 1 ? p : (void *)(void *)0;
  c = 1 ? p : (void *)(char *)0;
  return c;
}

size_t format_time(char *buf, size_t n, const struct tm *t) {
  return strftime(buf, n, "%Y", t);
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_tm:[0-9]+]] tm = struct {
// DEFAULT-NEXT:         field0 tm_sec: i32;
// DEFAULT-NEXT:         field1 tm_min: i32;
// DEFAULT-NEXT:         field2 tm_hour: i32;
// DEFAULT-NEXT:         field3 tm_mday: i32;
// DEFAULT-NEXT:         field4 tm_mon: i32;
// DEFAULT-NEXT:         field5 tm_year: i32;
// DEFAULT-NEXT:         field6 tm_wday: i32;
// DEFAULT-NEXT:         field7 tm_yday: i32;
// DEFAULT-NEXT:         field8 tm_isdst: i32;
// DEFAULT-NEXT:         field9 tm_gmtoff: i64;
// DEFAULT-NEXT:         field10 tm_zone: ptr<const i8>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 4, 8, 12, 16, 20, 24, 28, 32, 40, 48]];
// DEFAULT-NEXT:     type @type[[TYPE_actual:[0-9]+]] actual = i32 [c="typeof(0x7fffffff)"];
// DEFAULT-NEXT:     type @type[[TYPE_actual_2:[0-9]+]] actual = u32 [c="typeof(0x80000000)"];
// DEFAULT-NEXT:     type @type[[TYPE_actual_3:[0-9]+]] actual = i64 [c="typeof(2147483648)"];
// DEFAULT-NEXT:     type @type[[TYPE_actual_4:[0-9]+]] actual = u32 [c="typeof(0xffffffffu)"];
// DEFAULT-NEXT:     type @type[[TYPE_actual_5:[0-9]+]] actual = u64 [c="typeof(0x100000000u)"];
// DEFAULT-NEXT:     type @type[[TYPE_actual_6:[0-9]+]] actual = i64 [c="typeof(1l)"];
// DEFAULT-NEXT:     type @type[[TYPE_actual_7:[0-9]+]] actual = i32 [c="typeof(0b1)"];
// DEFAULT-NEXT:     type @type[[TYPE_actual_8:[0-9]+]] actual = u32 [c="typeof(0b10000000000000000000000000000000)"];
// DEFAULT-NEXT:     type @type[[TYPE_actual_9:[0-9]+]] actual = i32 [c="typeof(a)"];
// DEFAULT-NEXT:     type @type[[TYPE_actual_10:[0-9]+]] actual = i64 [c="typeof(b)"];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 89, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_strftime:[0-9]+]] @strftime(%[[VALUE___s:[0-9]+]] __s: ptr<i8> [restrict], %[[VALUE___maxsize:[0-9]+]] __maxsize: u64 [c="size_t"], %[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], %[[VALUE___tp:[0-9]+]] __tp: ptr<const @type[[TYPE_tm]]> [restrict]) -> u64 [linkage=external] [c="size_t(char *restrict, size_t, const char *restrict, const struct tm *restrict)"];
// DEFAULT-NEXT:     fn %[[VALUE_constant_types:[0-9]+]] @constant_types() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x:[0-9]+]] x: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>> [c="actual **"] [c="actual **"];
// DEFAULT-NEXT:                 let %[[VALUE_y:[0-9]+]] y: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_x]], read<ptr<ptr<i32>>>(%[[VALUE_y]]) [c="actual **"]) [c="actual **"];
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_y]], read<ptr<ptr<i32>>>(%[[VALUE_x]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_2:[0-9]+]] x: ptr<ptr<u32>> [storage=automatic] = null<ptr<ptr<u32>>> [c="actual **"] [c="actual **"];
// DEFAULT-NEXT:                 let %[[VALUE_y_2:[0-9]+]] y: ptr<ptr<u32>> [storage=automatic] = null<ptr<ptr<u32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<u32>>>(%[[VALUE_x_2]], read<ptr<ptr<u32>>>(%[[VALUE_y_2]]) [c="actual **"]) [c="actual **"];
// DEFAULT-NEXT:                 write<ptr<ptr<u32>>>(%[[VALUE_y_2]], read<ptr<ptr<u32>>>(%[[VALUE_x_2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_3:[0-9]+]] x: ptr<ptr<i64>> [storage=automatic] = null<ptr<ptr<i64>>> [c="actual **"] [c="actual **"];
// DEFAULT-NEXT:                 let %[[VALUE_y_3:[0-9]+]] y: ptr<ptr<i64>> [storage=automatic] = null<ptr<ptr<i64>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i64>>>(%[[VALUE_x_3]], read<ptr<ptr<i64>>>(%[[VALUE_y_3]]) [c="actual **"]) [c="actual **"];
// DEFAULT-NEXT:                 write<ptr<ptr<i64>>>(%[[VALUE_y_3]], read<ptr<ptr<i64>>>(%[[VALUE_x_3]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_4:[0-9]+]] x: ptr<ptr<u32>> [storage=automatic] = null<ptr<ptr<u32>>> [c="actual **"] [c="actual **"];
// DEFAULT-NEXT:                 let %[[VALUE_y_4:[0-9]+]] y: ptr<ptr<u32>> [storage=automatic] = null<ptr<ptr<u32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<u32>>>(%[[VALUE_x_4]], read<ptr<ptr<u32>>>(%[[VALUE_y_4]]) [c="actual **"]) [c="actual **"];
// DEFAULT-NEXT:                 write<ptr<ptr<u32>>>(%[[VALUE_y_4]], read<ptr<ptr<u32>>>(%[[VALUE_x_4]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_5:[0-9]+]] x: ptr<ptr<u64>> [storage=automatic] = null<ptr<ptr<u64>>> [c="actual **"] [c="actual **"];
// DEFAULT-NEXT:                 let %[[VALUE_y_5:[0-9]+]] y: ptr<ptr<u64>> [storage=automatic] = null<ptr<ptr<u64>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<u64>>>(%[[VALUE_x_5]], read<ptr<ptr<u64>>>(%[[VALUE_y_5]]) [c="actual **"]) [c="actual **"];
// DEFAULT-NEXT:                 write<ptr<ptr<u64>>>(%[[VALUE_y_5]], read<ptr<ptr<u64>>>(%[[VALUE_x_5]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_6:[0-9]+]] x: ptr<ptr<i64>> [storage=automatic] = null<ptr<ptr<i64>>> [c="actual **"] [c="actual **"];
// DEFAULT-NEXT:                 let %[[VALUE_y_6:[0-9]+]] y: ptr<ptr<i64>> [storage=automatic] = null<ptr<ptr<i64>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i64>>>(%[[VALUE_x_6]], read<ptr<ptr<i64>>>(%[[VALUE_y_6]]) [c="actual **"]) [c="actual **"];
// DEFAULT-NEXT:                 write<ptr<ptr<i64>>>(%[[VALUE_y_6]], read<ptr<ptr<i64>>>(%[[VALUE_x_6]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_7:[0-9]+]] x: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>> [c="actual **"] [c="actual **"];
// DEFAULT-NEXT:                 let %[[VALUE_y_7:[0-9]+]] y: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_x_7]], read<ptr<ptr<i32>>>(%[[VALUE_y_7]]) [c="actual **"]) [c="actual **"];
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_y_7]], read<ptr<ptr<i32>>>(%[[VALUE_x_7]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_8:[0-9]+]] x: ptr<ptr<u32>> [storage=automatic] = null<ptr<ptr<u32>>> [c="actual **"] [c="actual **"];
// DEFAULT-NEXT:                 let %[[VALUE_y_8:[0-9]+]] y: ptr<ptr<u32>> [storage=automatic] = null<ptr<ptr<u32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<u32>>>(%[[VALUE_x_8]], read<ptr<ptr<u32>>>(%[[VALUE_y_8]]) [c="actual **"]) [c="actual **"];
// DEFAULT-NEXT:                 write<ptr<ptr<u32>>>(%[[VALUE_y_8]], read<ptr<ptr<u32>>>(%[[VALUE_x_8]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: i32 [storage=automatic] = reinterpret<i32, reason=assign>(const<i32>(0)) [c="typeof(*(cast)0)"] [c="typeof(*(cast)0)"];
// DEFAULT-NEXT:         let %[[VALUE_b:[0-9]+]] b: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(0)) [c="typeof(*(cast)0)"] [c="typeof(*(cast)0)"];
// DEFAULT-NEXT:         do %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_9:[0-9]+]] x: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>> [c="actual **"] [c="actual **"];
// DEFAULT-NEXT:                 let %[[VALUE_y_9:[0-9]+]] y: ptr<ptr<i32>> [storage=automatic] = null<ptr<ptr<i32>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_x_9]], read<ptr<ptr<i32>>>(%[[VALUE_y_9]]) [c="actual **"]) [c="actual **"];
// DEFAULT-NEXT:                 write<ptr<ptr<i32>>>(%[[VALUE_y_9]], read<ptr<ptr<i32>>>(%[[VALUE_x_9]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         do %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_x_10:[0-9]+]] x: ptr<ptr<i64>> [storage=automatic] = null<ptr<ptr<i64>>> [c="actual **"] [c="actual **"];
// DEFAULT-NEXT:                 let %[[VALUE_y_10:[0-9]+]] y: ptr<ptr<i64>> [storage=automatic] = null<ptr<ptr<i64>>>;
// DEFAULT-NEXT:                 write<ptr<ptr<i64>>>(%[[VALUE_x_10]], read<ptr<ptr<i64>>>(%[[VALUE_y_10]]) [c="actual **"]) [c="actual **"];
// DEFAULT-NEXT:                 write<ptr<ptr<i64>>>(%[[VALUE_y_10]], read<ptr<ptr<i64>>>(%[[VALUE_x_10]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_not_null_constant:[0-9]+]] @not_null_constant(%[[VALUE_p:[0-9]+]] p: ptr<i32>, %[[VALUE_c:[0-9]+]] c: ptr<i64>) -> ptr<i64> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_c]], pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%[[VALUE_p]])), null<ptr<void>>)));
// DEFAULT-NEXT:         write<ptr<i64>>(%[[VALUE_c]], pointer_cast<ptr<i64>, reason=assign>(conditional<ptr<void>>(ne<i32>(const<i32>(1), const<i32>(0)), pointer_cast<ptr<void>, reason=usual_arith>(read<ptr<i32>>(%[[VALUE_p]])), pointer_cast<ptr<void>, reason=explicit>(null<ptr<i8>>))));
// DEFAULT-NEXT:         return read<ptr<i64>>(%[[VALUE_c]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_format_time:[0-9]+]] @format_time(%[[VALUE_buf:[0-9]+]] buf: ptr<i8>, %[[VALUE_n:[0-9]+]] n: u64 [c="size_t"], %[[VALUE_t:[0-9]+]] t: ptr<const @type[[TYPE_tm]]>) -> u64 [linkage=external] [fallthrough=ub_if_used] [c="size_t(char *, size_t, const struct tm *)"] {
// DEFAULT-NEXT:         return call<u64, signature=fn(ptr<i8>, u64, ptr<const i8>, ptr<const @type[[TYPE_tm]]>) -> u64>(%[[VALUE_strftime]], read<ptr<i8>>(%[[VALUE_buf]]), read<u64>(%[[VALUE_n]]) [c="size_t"], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])), read<ptr<const @type[[TYPE_tm]]>>(%[[VALUE_t]])) [c="size_t"];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
