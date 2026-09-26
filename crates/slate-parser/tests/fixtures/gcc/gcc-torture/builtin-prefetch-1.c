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
// DEFAULT-NEXT:     type @type0 locality = enum : u32 {
// DEFAULT-NEXT:         %0 none = const<i32>(0);
// DEFAULT-NEXT:         %1 low = const<i32>(1);
// DEFAULT-NEXT:         %2 moderate = const<i32>(2);
// DEFAULT-NEXT:         %3 high = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 rws = enum : u32 {
// DEFAULT-NEXT:         %0 read = const<i32>(0);
// DEFAULT-NEXT:         %1 write = const<i32>(1);
// DEFAULT-NEXT:         %2 read_shared = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     global %10 arr: array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%20 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @good_const(%12 p: ptr<const i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%12)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%12)), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%12)), const<i32>(0), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%12)), const<i32>(0), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%12)), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%12)), const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%12)), const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%12)), const<i32>(1), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @good_enum(%14 p: ptr<const i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%14)), const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%14)), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%14)), const<i32>(0), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%14)), const<i32>(0), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%14)), const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%14)), const<i32>(1), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%14)), const<i32>(1), const<i32>(2));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%14)), const<i32>(1), const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @good_expr(%16 p: ptr<const i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%16)), sub<i32, overflow=ub>(const<i32>(1), const<i32>(1)), sub<i32, overflow=ub>(const<i32>(6), mul<i32, overflow=ub>(const<i32>(2), const<i32>(3))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%16)), add<i32, overflow=ub>(const<i32>(1), const<i32>(0)), add<i32, overflow=ub>(const<i32>(1), const<i32>(2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @good_vararg(%18 p: ptr<const i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%18)), const<i32>(0), const<i32>(3));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%18)), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%18)), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const void>, ...) -> void>(__builtin_prefetch, pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i32>>(%18)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i32>) -> void>(%11, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i32>) -> void>(%13, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i32>) -> void>(%15, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i32>) -> void>(%17, pointer_cast<ptr<const i32>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%10)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
