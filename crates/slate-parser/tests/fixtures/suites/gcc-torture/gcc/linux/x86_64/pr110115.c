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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_e:[0-9]+]] e: ptr<i8>, %[[VALUE_f:[0-9]+]] f: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_d]]), read<i32>(%[[VALUE_f]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_e]]), read<i32>(%[[VALUE_d]]))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_e_2:[0-9]+]] e: i8, %[[VALUE_f_2:[0-9]+]] f: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: array<i8, 20> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = call<i32, signature=fn(ptr<i8>, i32) -> i32>(%[[VALUE_foo]], array_decay<ptr<i8>, length=Some(20)>(%[[VALUE_h]]), read<i32>(%[[VALUE_f_2]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         switch %[[VALUE3:[0-9]+]] read<i32>(%[[VALUE_a]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 case %[[VALUE3]] const<i32>(102):
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:                 default %[[VALUE3]]:
// DEFAULT-NEXT:                     return not<i32>(const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_k:[0-9]+]] k: array<ptr<i8>, 3> [storage=automatic] [align=16];
// DEFAULT-NEXT:             let %[[VALUE_d_2:[0-9]+]] d: i32 [storage=automatic];
// DEFAULT-NEXT:             for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_d_2]], const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<i32>(add<i32, overflow=ub>(sub<i32, overflow=ub>(call<i32, signature=fn(i8, i32) -> i32>(%[[VALUE_bar]], truncate<i8, reason=arg, fits=always>(const<i32>(8)), const<i32>(15)), const<i32>(15)), read<i32>(%[[VALUE_d_2]])), const<i32>(1))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d_2]]);
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_d_2]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     write<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(3)>(%[[VALUE_k]]), add<i32, overflow=ub>(call<i32, signature=fn() -> i32>(%[[VALUE_baz]]), const<i32>(1)))), addr_of<ptr<i8>>(%[[VALUE_b]]));
// DEFAULT-NEXT:             write<i8>(deref(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(3)>(%[[VALUE_k]]), const<i32>(0))))), truncate<i8, reason=assign, fits=unknown>(neg<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(array_decay<ptr<ptr<i8>>, length=Some(3)>(%[[VALUE_k]]), const<i32>(0))))))))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
