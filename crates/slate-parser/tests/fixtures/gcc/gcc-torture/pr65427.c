/* PR tree-optimization/65427 */

typedef int V __attribute__((vector_size(8 * sizeof(int))));
V           a, b, c, d, e, f;

__attribute__((noinline, noclone)) void foo(int x, int y) {
  do {
    if (x)
      d = a ^ c;
    else
      d = a ^ b;
  } while (y);
}

int main() {
  a = (V){1, 2, 3, 4, 5, 6, 7, 8};
  b = (V){0x40, 0x80, 0x40, 0x80, 0x40, 0x80, 0x40, 0x80};
  e = (V){0x41, 0x82, 0x43, 0x84, 0x45, 0x86, 0x47, 0x88};
  foo(0, 0);
  if (__builtin_memcmp(&d, &e, sizeof(V)) != 0)
    __builtin_abort();
  c = (V){0x80, 0x40, 0x80, 0x40, 0x80, 0x40, 0x80, 0x40};
  f = (V){0x81, 0x42, 0x83, 0x44, 0x85, 0x46, 0x87, 0x48};
  foo(1, 0);
  if (__builtin_memcmp(&d, &f, sizeof(V)) != 0)
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
// DEFAULT-NEXT:     type @type0 V = vector<i32, 8>;
// DEFAULT-NEXT:     global %1 a: vector<i32, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 b: vector<i32, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 c: vector<i32, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: vector<i32, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 e: vector<i32, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 f: vector<i32, 8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %7 @foo(%8 x: i32, %9 y: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %11
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                     write<vector<i32, 8>>(%4, xor<vector<i32, 8>, elementwise=true>(read<vector<i32, 8>>(%1), read<vector<i32, 8>>(%3)));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<vector<i32, 8>>(%4, xor<vector<i32, 8>, elementwise=true>(read<vector<i32, 8>>(%1), read<vector<i32, 8>>(%2)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(read<i32>(%9), const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<vector<i32, 8>>(%1, read<vector<i32, 8>>(compound_literal %12 [storage=automatic] = aggregate<vector<i32, 8>, zero_fill=false>(index0 = const<i32>(1), index1 = const<i32>(2), index2 = const<i32>(3), index3 = const<i32>(4), index4 = const<i32>(5), index5 = const<i32>(6), index6 = const<i32>(7), index7 = const<i32>(8))));
// DEFAULT-NEXT:         write<vector<i32, 8>>(%2, read<vector<i32, 8>>(compound_literal %13 [storage=automatic] = aggregate<vector<i32, 8>, zero_fill=false>(index0 = const<i32>(64), index1 = const<i32>(128), index2 = const<i32>(64), index3 = const<i32>(128), index4 = const<i32>(64), index5 = const<i32>(128), index6 = const<i32>(64), index7 = const<i32>(128))));
// DEFAULT-NEXT:         write<vector<i32, 8>>(%5, read<vector<i32, 8>>(compound_literal %14 [storage=automatic] = aggregate<vector<i32, 8>, zero_fill=false>(index0 = const<i32>(65), index1 = const<i32>(130), index2 = const<i32>(67), index3 = const<i32>(132), index4 = const<i32>(69), index5 = const<i32>(134), index6 = const<i32>(71), index7 = const<i32>(136))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%7, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i32, 8>>>(%4)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i32, 8>>>(%5)), const<u64>(32)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<vector<i32, 8>>(%3, read<vector<i32, 8>>(compound_literal %15 [storage=automatic] = aggregate<vector<i32, 8>, zero_fill=false>(index0 = const<i32>(128), index1 = const<i32>(64), index2 = const<i32>(128), index3 = const<i32>(64), index4 = const<i32>(128), index5 = const<i32>(64), index6 = const<i32>(128), index7 = const<i32>(64))));
// DEFAULT-NEXT:         write<vector<i32, 8>>(%6, read<vector<i32, 8>>(compound_literal %16 [storage=automatic] = aggregate<vector<i32, 8>, zero_fill=false>(index0 = const<i32>(129), index1 = const<i32>(66), index2 = const<i32>(131), index3 = const<i32>(68), index4 = const<i32>(133), index5 = const<i32>(70), index6 = const<i32>(135), index7 = const<i32>(72))));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%7, const<i32>(1), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(__builtin_memcmp, pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i32, 8>>>(%4)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<vector<i32, 8>>>(%6)), const<u64>(32)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
