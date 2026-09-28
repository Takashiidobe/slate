/* PR tree-optimization/19828 */
typedef __SIZE_TYPE__ size_t;
extern size_t         strlen(const char *s);
extern int            strncmp(const char *s1, const char *s2, size_t n);
extern void           abort(void);

const char *a[16] = {"a", "bc", "de", "fgh"};

int foo(char *x, const char *y, size_t n) {
  size_t i, j = 0;
  for (i = 0; i < n; i++) {
    if (strncmp(x + j, a[i], strlen(a[i])) != 0)
      return 2;
    j += strlen(a[i]);
    if (y)
      j += strlen(y);
  }
  return 0;
}

int main(void) {
  if (foo("abcde", (const char *)0, 3) != 0)
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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     global %20 .str20: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %22 .str22: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([100, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %23 .str23: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %8 a: array<ptr<const i8>, 16> [storage=static] [align=16] = aggregate<array<ptr<const i8>, 16>, zero_fill=true>(index0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%20)), index1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%21)), index2 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%22)), index3 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%23))) [linkage=external];
// DEFAULT-NEXT:     global %25 .str25: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 98, 99, 100, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @strlen(%16 s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %6 @strncmp(%17 s1: ptr<const i8>, %18 s2: ptr<const i8>, %19 n: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %7 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @foo(%10 x: ptr<i8>, %11 y: ptr<const i8>, %12 n: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 i: u64 [storage=automatic];
// DEFAULT-NEXT:         let %14 j: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         for %24
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%13, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%13), read<u64>(%12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: u64 [synthetic] = read<u64>(%13);
// DEFAULT-NEXT:                 let %27: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%26), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%13, read<u64>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%6, pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%10), read<u64>(%14))), read<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(array_decay<ptr<ptr<const i8>>, length=Some(16)>(%8), read<u64>(%13)))), call<u64, signature=fn(ptr<const i8>) -> u64>(%2, read<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(array_decay<ptr<ptr<const i8>>, length=Some(16)>(%8), read<u64>(%13)))))), const<i32>(0))
// DEFAULT-NEXT:                         return const<i32>(2);
// DEFAULT-NEXT:                     let %28: u64 [synthetic] = read<u64>(%14);
// DEFAULT-NEXT:                     let %29: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%28), call<u64, signature=fn(ptr<const i8>) -> u64>(%2, read<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(array_decay<ptr<ptr<const i8>>, length=Some(16)>(%8), read<u64>(%13))))));
// DEFAULT-NEXT:                     write<u64>(%14, read<u64>(%29));
// DEFAULT-NEXT:                     if ne<ptr<const i8>>(read<ptr<const i8>>(%11), null<ptr<const i8>>)
// DEFAULT-NEXT:                         let %30: u64 [synthetic] = read<u64>(%14);
// DEFAULT-NEXT:                         let %31: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%30), call<u64, signature=fn(ptr<const i8>) -> u64>(%2, read<ptr<const i8>>(%11)));
// DEFAULT-NEXT:                         write<u64>(%14, read<u64>(%31));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i8>, ptr<const i8>, u64) -> i32>(%9, array_decay<ptr<i8>, length=Some(6)>(%25), null<ptr<const i8>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
