/* PR tree-optimization/110914 */

__attribute__((noipa)) int foo(const char *s, unsigned long l) {
  unsigned char r = 0;
  __builtin_memcpy(&r, s, l != 0);
  return r;
}

int main() {
  const char *p = "123456";
  int         a = foo(p, __builtin_strlen(p) - 5);
  int         b = foo(p, __builtin_strlen(p) - 6);
  if (a != '1')
    __builtin_abort();
  if (b != 0)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %12 .str12: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([49, 50, 51, 52, 53, 54, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %11 @__builtin_memcpy(%8 <unnamed>: ptr<void>, %9 <unnamed>: ptr<const void>, %10 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %0 @foo(%1 s: ptr<const i8>, %2 l: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 r: u8 [storage=automatic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%11, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<u8>>(%3)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<const i8>>(%1)), from_bool<u64, reason=arg>(ne<u64>(read<u64>(%2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(0))))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(widen<u32, reason=return>(read<u8>(%3)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_strlen(%13 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %15 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 p: ptr<const i8> [storage=automatic] = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(7)>(%12));
// DEFAULT-NEXT:         let %6 a: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, u64) -> i32>(%0, read<ptr<const i8>>(%5), sub<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%14, read<ptr<const i8>>(%5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(5)))));
// DEFAULT-NEXT:         let %7 b: i32 [storage=automatic] = call<i32, signature=fn(ptr<const i8>, u64) -> i32>(%0, read<ptr<const i8>>(%5), sub<u64, overflow=wrap>(call<u64, signature=fn(ptr<const i8>) -> u64>(%14, read<ptr<const i8>>(%5)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6)))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(49))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%7), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%15);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
