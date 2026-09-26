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
// DEFAULT-NEXT:     global %16 .str16: array<i8, 2> [storage=static] = code_units<array<i8, 2>>([97, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %17 .str17: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([98, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %18 .str18: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([100, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([102, 103, 104, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %4 a: array<ptr<const i8>, 16> [storage=static] [align=16] = aggregate<array<ptr<const i8>, 16>, zero_fill=true>(index0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(2)>(%16)), index1 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%17)), index2 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(3)>(%18)), index3 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%19))) [linkage=external];
// DEFAULT-NEXT:     global %21 .str21: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([97, 98, 99, 100, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @strlen(%12 s: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %2 @strncmp(%13 s1: ptr<const i8>, %14 s2: ptr<const i8>, %15 n: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %3 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @foo(%6 x: ptr<i8>, %7 y: ptr<const i8>, %8 n: u64) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 i: u64 [storage=automatic];
// DEFAULT-NEXT:         let %10 j: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         for %20
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%9, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:             condition: lt<u64>(read<u64>(%9), read<u64>(%8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %22: u64 [synthetic] = read<u64>(%9);
// DEFAULT-NEXT:                 let %23: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%22), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                 write<u64>(%9, read<u64>(%23));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn(ptr<const i8>, ptr<const i8>, u64) -> i32>(%2, pointer_cast<ptr<const i8>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%6), read<u64>(%10))), read<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(array_decay<ptr<ptr<const i8>>, length=Some(16)>(%4), read<u64>(%9)))), call<u64, signature=fn(ptr<const i8>) -> u64>(%1, read<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(array_decay<ptr<ptr<const i8>>, length=Some(16)>(%4), read<u64>(%9)))))), const<i32>(0))
// DEFAULT-NEXT:                         return const<i32>(2);
// DEFAULT-NEXT:                     let %24: u64 [synthetic] = read<u64>(%10);
// DEFAULT-NEXT:                     let %25: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%24), call<u64, signature=fn(ptr<const i8>) -> u64>(%1, read<ptr<const i8>>(deref(ptr_offset<ptr<ptr<const i8>>, subtract=false, element=ptr<const i8>, overflow=ub>(array_decay<ptr<ptr<const i8>>, length=Some(16)>(%4), read<u64>(%9))))));
// DEFAULT-NEXT:                     write<u64>(%10, read<u64>(%25));
// DEFAULT-NEXT:                     if ne<ptr<const i8>>(read<ptr<const i8>>(%7), null<ptr<const i8>>)
// DEFAULT-NEXT:                         let %26: u64 [synthetic] = read<u64>(%10);
// DEFAULT-NEXT:                         let %27: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%26), call<u64, signature=fn(ptr<const i8>) -> u64>(%1, read<ptr<const i8>>(%7)));
// DEFAULT-NEXT:                         write<u64>(%10, read<u64>(%27));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i8>, ptr<const i8>, u64) -> i32>(%5, array_decay<ptr<i8>, length=Some(6)>(%21), null<ptr<const i8>>, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%3);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
