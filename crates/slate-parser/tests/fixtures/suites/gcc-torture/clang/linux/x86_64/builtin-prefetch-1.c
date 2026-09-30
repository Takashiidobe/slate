/* Test that __builtin_prefetch does no harm.

   Prefetch using all valid combinations of rw and locality values.
   These must be compile-time constants.  */

void exit(int);

#define NO_TEMPORAL_LOCALITY       0
#define LOW_TEMPORAL_LOCALITY      1
#define MODERATE_TEMPORAL_LOCALITY 1
#define HIGH_TEMPORAL_LOCALITY     3

#define READ_SHARED  2
#define WRITE_ACCESS 1
#define READ_ACCESS  0

enum locality { none, low, moderate, high };
enum rws { read, write, read_shared };

int arr[10];

void good_const(const int *p) {
  __builtin_prefetch(p, 0, 0);
  __builtin_prefetch(p, 0, 1);
  __builtin_prefetch(p, 0, 2);
  __builtin_prefetch(p, READ_ACCESS, 3);
  __builtin_prefetch(p, 1, NO_TEMPORAL_LOCALITY);
  __builtin_prefetch(p, 1, LOW_TEMPORAL_LOCALITY);
  __builtin_prefetch(p, 1, MODERATE_TEMPORAL_LOCALITY);
  __builtin_prefetch(p, WRITE_ACCESS, HIGH_TEMPORAL_LOCALITY);
}

void good_enum(const int *p) {
  __builtin_prefetch(p, read, none);
  __builtin_prefetch(p, read, low);
  __builtin_prefetch(p, read, moderate);
  __builtin_prefetch(p, read, high);
  __builtin_prefetch(p, write, none);
  __builtin_prefetch(p, write, low);
  __builtin_prefetch(p, write, moderate);
  __builtin_prefetch(p, write, high);
}

void good_expr(const int *p) {
  __builtin_prefetch(p, 1 - 1, 6 - (2 * 3));
  __builtin_prefetch(p, 1 + 0, 1 + 2);
}

void good_vararg(const int *p) {
  __builtin_prefetch(p, 0, 3);
  __builtin_prefetch(p, 0);
  __builtin_prefetch(p, 1);
  __builtin_prefetch(p);
}

int main() {
  good_const(arr);
  good_enum(arr);
  good_expr(arr);
  good_vararg(arr);
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
// DEFAULT-NEXT:     type @type[[TYPE_locality:[0-9]+]] locality = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_none:[0-9]+]] none = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_low:[0-9]+]] low = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_moderate:[0-9]+]] moderate = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_high:[0-9]+]] high = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_rws:[0-9]+]] rws = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_none]] read = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_low]] write = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_moderate]] read_shared = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %[[VALUE_arr:[0-9]+]] arr: array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_none]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_prefetch:[0-9]+]] @__builtin_prefetch(%[[VALUE1:[0-9]+]] <unnamed>: ptr<const void>, ...) -> void [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %[[VALUE_good_const:[0-9]+]] @good_const(%[[VALUE_p:[0-9]+]] p: ptr<const i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p]])), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p]])), const<i32>(0), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p]])), const<i32>(0), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p]])), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p]])), const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p]])), const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p]])), const<i32>(1), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_good_enum:[0-9]+]] @good_enum(%[[VALUE_p_2:[0-9]+]] p: ptr<const i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_2]])), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_2]])), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_2]])), const<i32>(0), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_2]])), const<i32>(0), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_2]])), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_2]])), const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_2]])), const<i32>(1), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_2]])), const<i32>(1), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_good_expr:[0-9]+]] @good_expr(%[[VALUE_p_3:[0-9]+]] p: ptr<const i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_3]])), sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(const<i32>(6), mul<i32, overflow=ub>(const<i32>(2), const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_3]])), add<i32, overflow=ub>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(1), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_good_vararg:[0-9]+]] @good_vararg(%[[VALUE_p_4:[0-9]+]] p: ptr<const i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_4]])), const<i32>(0), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_4]])), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_4]])), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(%[[VALUE___builtin_prefetch]], pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%[[VALUE_p_4]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i32>) -> void>(%[[VALUE_good_const]], pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_arr]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i32>) -> void>(%[[VALUE_good_enum]], pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_arr]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i32>) -> void>(%[[VALUE_good_expr]], pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_arr]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i32>) -> void>(%[[VALUE_good_vararg]], pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%[[VALUE_arr]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_none]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
