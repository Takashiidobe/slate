/* PR middle-end/117498 */

int          a, d, f;
char         g;
volatile int c = 1;

int foo() {
  if (c == 0)
    return -1;
  return 1;
}

void bar(int h, int i, char *k, char *m) {
  for (; d < i; d += 2)
    for (int j = 0; j < h; j++)
      m[j] = k[4 * j];
}

void baz(long h) {
  char n = 0;
  bar(h, 4, &n, &g);
}

int main() {
  f = foo();
  baz((unsigned char)f - 4);
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
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: volatile i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(read<i32, volatile>(%[[VALUE_c]]), const<i32>(0))
// DEFAULT-NEXT:             return neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_h:[0-9]+]] h: i32, %[[VALUE_i:[0-9]+]] i: i32, %[[VALUE_k:[0-9]+]] k: ptr<i8>, %[[VALUE_m:[0-9]+]] m: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_d]]), read<i32>(%[[VALUE_i]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_d]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(2));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_d]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_h]]))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_m]]), read<i32>(%[[VALUE_j]]))), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_k]]), mul<i32, overflow=ub>(const<i32>(4), read<i32>(%[[VALUE_j]]))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_h_2:[0-9]+]] h: i64) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>, ptr<i8>) -> void>(%[[VALUE_bar]], truncate<i32, reason=arg, fits=unknown>(read<i64>(%[[VALUE_h_2]])), const<i32>(4), addr_of<ptr<i8>>(%[[VALUE_n]]), addr_of<ptr<i8>>(%[[VALUE_g]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_f]], call<i32, signature=fn() -> i32>(%[[VALUE_foo]]));
// DEFAULT-NEXT:         call<void, signature=fn(i64) -> void>(%[[VALUE_baz]], widen<i64, reason=arg>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(reinterpret<u8, reason=explicit, fits=unknown>(truncate<i8, reason=explicit, fits=unknown>(read<i32>(%[[VALUE_f]]))))), const<i32>(4))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
