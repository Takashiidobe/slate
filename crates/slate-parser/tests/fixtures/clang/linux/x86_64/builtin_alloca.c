#include <stdio.h>
#include <string.h>

static int const_alloca(void) {
  unsigned char *buf = __builtin_alloca(16);
  memset(buf, 3, 16);
  return buf[0] + buf[15];
}

static int dyn_alloca(int n) {
  unsigned char *buf = __builtin_alloca(n);
  memset(buf, 7, n);
  return buf[0] + buf[n - 1];
}

static int uninitialized_alloca(int n) {
  unsigned char *buf = __builtin_alloca_uninitialized(n);
  memset(buf, 9, n);
  return buf[0] + buf[n - 1];
}

int main(void) {
  printf("%d %d %d\n", const_alloca(), dyn_alloca(10),
         uninitialized_alloca(12));
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %24 .str24: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%16 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @memset(%17 __s: ptr<void>, %18 __c: i32, %19 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %21 @__builtin_alloca(%20 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %7 @const_alloca() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 buf: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%21, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<u8>>(%8)), const<i32>(3), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%8), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%8), const<i32>(15)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @dyn_alloca(%10 n: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 buf: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%21, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%10)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<u8>>(%11)), const<i32>(7), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%10))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%11), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%11), sub<i32, overflow=ub>(read<i32>(%10), const<i32>(1))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @__builtin_alloca_uninitialized(%22 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %12 @uninitialized_alloca(%13 n: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 buf: ptr<u8> [storage=automatic] = pointer_cast<ptr<u8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%23, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%13)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(read<ptr<u8>>(%14)), const<i32>(9), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%13))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%14), const<i32>(0)))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%14), sub<i32, overflow=ub>(read<i32>(%13), const<i32>(1))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%24)), call<i32, signature=fn() -> i32>(%7), call<i32, signature=fn(i32) -> i32>(%9, const<i32>(10)), call<i32, signature=fn(i32) -> i32>(%12, const<i32>(12)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
