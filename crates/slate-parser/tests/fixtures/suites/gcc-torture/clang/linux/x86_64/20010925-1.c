extern void exit(int);
extern void abort(void);

extern void *memcpy(void *, const void *, __SIZE_TYPE__);
int          foo(void *, void *, unsigned int c);

int src[10];
int dst[10];

int main() {
  if (foo(dst, src, 10) != 0)
    abort();
  exit(0);
}

int foo(void *a, void *b, unsigned int c) {
  if (c == 0)
    return 1;

  memcpy(a, b, c);
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
// DEFAULT-NEXT:     global %5 src: array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %6 dst: array<i32, 10> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %0 @exit(%11 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @memcpy(%12 <unnamed>: ptr<void>, %13 <unnamed>: ptr<const void>, %14 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo(%8 a: ptr<void>, %9 b: ptr<void>, %10 c: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%2, read<ptr<void>>(%8), pointer_cast<ptr<const void>, reason=arg>(read<ptr<void>>(%9)), widen<u64, reason=arg>(read<u32>(%10)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, ptr<void>, u32) -> i32>(%4, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%6)), pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i32>, length=Some(10)>(%5)), reinterpret<u32, reason=arg, fits=always>(const<i32>(10))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
