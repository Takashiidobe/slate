/* PR middle-end/111422 */

int         a, b;
int        *c = &b;
unsigned    d;
signed char e;
int         f = 1;

int foo(int k, signed char *l) {
  if (k < 6)
    return a;
  l[0] = l[1] = l[k - 1] = 8;
  return 0;
}

int bar(int k) {
  signed char g[11];
  int         h = foo(k, g);
  return h;
}

int main() {
  for (; b < 8; b = b + 1)
    ;
  int  j;
  int *n[8];
  for (j = 0; 18446744073709551608ULL + bar(*c) + *c + j < 2; j++)
    n[j] = &f;
  for (; e <= 4; e++)
    d = *n[0] == f;
  if (d != 1)
    __builtin_abort();
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
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: ptr<i32> [storage=static] = addr_of<ptr<i32>>(%1) [linkage=external];
// DEFAULT-NEXT:     global %3 d: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 e: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 f: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %6 @foo(%7 k: i32, %8 l: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if lt<i32>(read<i32>(%7), const<i32>(6))
// DEFAULT-NEXT:             return read<i32>(%0);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%8), sub<i32, overflow=ub>(read<i32>(%7), const<i32>(1)))), truncate<i8, reason=assign, fits=always>(const<i32>(8)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%8), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(8)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%8), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(8)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @bar(%10 k: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 g: array<i8, 11> [storage=automatic];
// DEFAULT-NEXT:         let %12 h: i32 [storage=automatic] = call<i32, signature=fn(i32, ptr<i8>) -> i32>(%6, read<i32>(%10), array_decay<ptr<i8>, length=Some(11)>(%11));
// DEFAULT-NEXT:         return read<i32>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         for %16
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%1), const<i32>(8))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 write<i32>(%1, add<i32, overflow=ub>(read<i32>(%1), const<i32>(1)));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:         let %14 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %15 n: array<ptr<i32>, 8> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %17
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u64>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(18446744073709551608), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(call<i32, signature=fn(i32) -> i32>(%9, read<i32>(deref(read<ptr<i32>>(%2))))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(deref(read<ptr<i32>>(%2)))))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<i32>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(8)>(%15), read<i32>(%14))), addr_of<ptr<i32>>(%5));
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: le<i32>(widen<i32, reason=promotion>(read<i8>(%4)), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: i8 [synthetic] = read<i8>(%4);
// DEFAULT-NEXT:                 let %22: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(%21)), const<i32>(1)));
// DEFAULT-NEXT:                 write<i8>(%4, read<i8>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u32>(%3, from_bool<u32, reason=assign>(eq<i32>(read<i32>(deref(read<ptr<i32>>(deref(ptr_offset<ptr<ptr<i32>>, subtract=false, element=ptr<i32>, overflow=ub>(array_decay<ptr<ptr<i32>>, length=Some(8)>(%15), const<i32>(0)))))), read<i32>(%5))));
// DEFAULT-NEXT:         if ne<u32>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
