/* PR target/60062 */

int a;

static void foo(const char *p1, int p2) {
  if (__builtin_strcmp(p1, "hello") != 0)
    __builtin_abort();
}

static void bar(const char *p1) {
  if (__builtin_strcmp(p1, "hello") != 0)
    __builtin_abort();
}

__attribute__((optimize(0))) int main() {
  foo("hello", a);
  bar("hello");
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
// DEFAULT-NEXT:     global %0 a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 .str7: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 .str8: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %10 .str10: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([104, 101, 108, 108, 111, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @foo(%2 p1: ptr<const i8>, %3 p2: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(__builtin_strcmp, read<ptr<const i8>>(%2), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%7))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 p1: ptr<const i8>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>) -> i32>(__builtin_strcmp, read<ptr<const i8>>(%5), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%8))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>, i32) -> void>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%9)), read<i32>(%0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<const i8>) -> void>(%4, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(6)>(%10)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
