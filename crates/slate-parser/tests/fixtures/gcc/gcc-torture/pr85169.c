/* PR target/85169 */

typedef char V __attribute__((vector_size(64)));

static void __attribute__((noipa)) foo(V *p) {
  V v   = *p;
  v[63] = 1;
  *p    = v;
}

int main() {
  V v = (V){};
  foo(&v);
  for (unsigned i = 0; i < 64; i++)
    if (v[i] != (i == 63))
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
// DEFAULT-NEXT:     type @type0 V = vector<i8, 64>;
// DEFAULT-NEXT:     fn %1 @foo(%2 p: ptr<vector<i8, 64>>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 v: vector<i8, 64> [storage=automatic] = read<vector<i8, 64>>(deref(read<ptr<vector<i8, 64>>>(%2)));
// DEFAULT-NEXT:         write<i8>(lane(%3, const<i32>(63)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<vector<i8, 64>>(deref(read<ptr<vector<i8, 64>>>(%2)), read<vector<i8, 64>>(%3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 v: vector<i8, 64> [storage=automatic] = read<vector<i8, 64>>(compound_literal %7 [storage=automatic] = aggregate<vector<i8, 64>, zero_fill=true>());
// DEFAULT-NEXT:         call<void, signature=fn(ptr<vector<i8, 64>>) -> void>(%1, addr_of<ptr<vector<i8, 64>>>(%5));
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %6 i: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: u32 [synthetic] = read<u32>(%6);
// DEFAULT-NEXT:                 let %10: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%9), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%6, read<u32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(widen<i32, reason=promotion>(read<i8>(lane(%5, read<u32>(%6)))), from_bool<i32, reason=promotion>(eq<u32>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(63)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
