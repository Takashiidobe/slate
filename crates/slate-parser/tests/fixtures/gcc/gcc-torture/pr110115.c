/* PR middle-end/110115 */

int         a;
signed char b;

static int foo(signed char *e, int f) {
  int d;
  for (d = 0; d < f; d++)
    e[d] = 0;
  return d;
}

int bar(signed char e, int f) {
  signed char h[20];
  int         i = foo(h, f);
  return i;
}

int baz() {
  switch (a) {
  case 'f':
    return 0;
  default:
    return ~0;
  }
}

int main() {
  {
    signed char *k[3];
    int          d;
    for (d = 0; bar(8, 15) - 15 + d < 1; d++)
      k[baz() + 1] = &b;
    *k[0] = -*k[0];
  }
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
// DEFAULT-NEXT:     global %1 b: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @foo(%3 e: ptr<i8>, %4 f: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %5 d: i32 [storage=automatic];
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), read<i32>(%4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), read<i32>(%5))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return read<i32>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar(%7 e: i8, %8 f: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9 h: array<i8, 20> [storage=automatic];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, i32) -> i32>(%2, array_decay<ptr<i8>, length=Some(20)>(%9), read<i32>(%8));
// DEFAULT-NEXT:         return read<i32>(%10);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @baz() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %16 read<i32>(%0)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %16 const<i32>(102):
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:                 default %16:
// DEFAULT-NEXT:                     return not<i32>(const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %13 k: array<ptr<i8>, 3> [storage=automatic];
// DEFAULT-NEXT:             let %14 d: i32 [storage=automatic];
// DEFAULT-NEXT:             for %17
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%14, const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(call<i32, signature=fn(i8, i32) -> i32>(%6, truncate<i8, reason=arg, fits=always>(const<i32>(8)), const<i32>(15)), const<i32>(15)), read<i32>(%14)), const<i32>(1))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %20: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                     let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%14, read<i32>(%21));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     write<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(3)>(%13), add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%11), const<i32>(1)))), addr_of<ptr<i8>>(%1));
// DEFAULT-NEXT:             write<i8>(deref(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(3)>(%13), const<i32>(0))))), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(3)>(%13), const<i32>(0))))))))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
