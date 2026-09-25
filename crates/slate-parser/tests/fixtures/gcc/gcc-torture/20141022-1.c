#define ABORT()                                                                \
  do {                                                                         \
    __builtin_printf("assert.\n");                                             \
    __builtin_abort();                                                         \
  } while (0)
int f(int a) __attribute__((noinline));
int f(int a) {
  int fem_key_src;
  int D2930   = a & 4294967291;
  fem_key_src = a == 6 ? 0 : 15;
  fem_key_src = D2930 != 1 ? fem_key_src : 0;
  return fem_key_src;
}

int main(void) {
  if (f(0) != 15)
    ABORT();
  if (f(1) != 0)
    ABORT();
  if (f(6) != 0)
    ABORT();
  if (f(5) != 0)
    ABORT();
  if (f(15) != 15)
    ABORT();
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
// DEFAULT-NEXT:     global %7 .str7: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([97, 115, 115, 101, 114, 116, 46, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %9 .str9: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([97, 115, 115, 101, 114, 116, 46, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([97, 115, 115, 101, 114, 116, 46, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %13 .str13: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([97, 115, 115, 101, 114, 116, 46, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %15 .str15: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([97, 115, 115, 101, 114, 116, 46, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @f(%1 a: i32) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %2 fem_key_src: i32 [storage=automatic];
// DEFAULT-NEXT:         let %3 D2930: i32 [storage=automatic] = truncate<i32, reason=assign, fits=unknown>(and<i64>(widen<i64, reason=usual_arith>(read<i32>(%1)), const<i64>(4294967291)));
// DEFAULT-NEXT:         write<i32>(%2, conditional<i32>(eq<i32>(read<i32>(%1), const<i32>(6)), const<i32>(0), const<i32>(15)));
// DEFAULT-NEXT:         write<i32>(%2, conditional<i32>(ne<i32>(read<i32>(%3), const<i32>(1)), read<i32>(%2), const<i32>(0)));
// DEFAULT-NEXT:         return read<i32>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(0)), const<i32>(15))
// DEFAULT-NEXT:             do %6
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%7)));
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(1)), const<i32>(0))
// DEFAULT-NEXT:             do %8
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%9)));
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(6)), const<i32>(0))
// DEFAULT-NEXT:             do %10
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%11)));
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(5)), const<i32>(0))
// DEFAULT-NEXT:             do %12
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%13)));
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32) -> i32>(%0, const<i32>(15)), const<i32>(15))
// DEFAULT-NEXT:             do %14
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(__builtin_printf, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%15)));
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
