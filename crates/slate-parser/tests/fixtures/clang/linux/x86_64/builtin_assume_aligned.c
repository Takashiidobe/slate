#include <stddef.h>
#include <stdio.h>

static void *first_word(void *p) {
  void *q = __builtin_assume_aligned(p, 32);
  return q;
}

static void *first_word_offset(void *p, size_t off) {
  void *q = __builtin_assume_aligned(p, 32, off);
  return q;
}

int main(void) {
  _Alignas(32) static unsigned char buf[64];
  void                             *a = first_word(buf);
  void                             *b = first_word_offset(buf + 8, 8);
  printf("%d %d\n", a == buf, b == buf + 8);
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
// DEFAULT-NEXT:     global %11 buf: array<u8, 64> [storage=static] [align=32] [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @printf(%14 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %17 @__builtin_assume_aligned(%15 <unnamed>: ptr<const void>, %16 <unnamed>: u64, ...) -> ptr<void> [linkage=external] [memory=none];
// DEFAULT-NEXT:     fn %3 @first_word(%4 p: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%17, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%4)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))));
// DEFAULT-NEXT:         return read<ptr<void>>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @first_word_offset(%7 p: ptr<void>, %8 off: u64) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 q: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<const void>, u64, ...) -> ptr<void>>(%17, pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%7)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(32))), read<u64>(%8));
// DEFAULT-NEXT:         return read<ptr<void>>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 a: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%3, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(64)>(%11)));
// DEFAULT-NEXT:         let %13 b: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%6, pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(64)>(%11), const<i32>(8))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(8))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%18)), from_bool<i32, reason=vararg>(eq<ptr<void>>(read<ptr<void>>(%12), pointer_cast<ptr<void>, reason=usual_arith>(array_decay<ptr<u8>, length=Some(64)>(%11)))), from_bool<i32, reason=vararg>(eq<ptr<void>>(read<ptr<void>>(%13), pointer_cast<ptr<void>, reason=usual_arith>(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(64)>(%11), const<i32>(8))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
