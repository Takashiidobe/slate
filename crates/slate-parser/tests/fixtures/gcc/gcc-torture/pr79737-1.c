/* PR tree-optimization/79737 */

#if __SIZEOF_INT__ < 4
__extension__ typedef __INT32_TYPE__ int32_t;
#else
typedef int int32_t;
#endif

#pragma pack(1)
struct S {
  int32_t b : 18;
  int32_t c : 1;
  int32_t d : 24;
  int32_t e : 15;
  int32_t f : 14;
} i;
int             g, j, k;
static struct S h;

void foo() {
  for (j = 0; j < 6; j++)
    k = 0;
  for (; k < 3; k++) {
    struct S m = {5, 0, -5, 9, 5};
    h          = m;
    if (g)
      i = m;
    h.e = 0;
  }
}

int main() {
  foo();
  if (h.e != 0)
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
// DEFAULT-NEXT:     type @type0 int32_t = i32;
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 b: i32 : 18;
// DEFAULT-NEXT:         field1 c: i32 : 1;
// DEFAULT-NEXT:         field2 d: i32 : 24;
// DEFAULT-NEXT:         field3 e: i32 : 15;
// DEFAULT-NEXT:         field4 f: i32 : 14;
// DEFAULT-NEXT:     } [size=9, align=1, offsets=[0, 2, 2, 5, 7], bit_offsets=[Some(0), Some(18), Some(19), Some(43), Some(58)], bit_units=[(0, 9)], field_units=[Some(0), Some(0), Some(0), Some(0), Some(0)]];
// DEFAULT-NEXT:     global %2 i: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 j: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 k: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 h: @type1 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %7 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %10
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%4), const<i32>(6))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %12: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%13));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%5), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %15: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%5, read<i32>(%15));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %8 m: @type1 [storage=automatic] = aggregate<@type1, zero_fill=false>(field0 = const<i32>(5), field1 = const<i32>(0), field2 = neg<i32, overflow=ub>(const<i32>(5)), field3 = const<i32>(9), field4 = const<i32>(5));
// DEFAULT-NEXT:                     write<@type1>(%6, copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:                         write<@type1>(%2, copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:                     write<i32>(bitfield3<unit=0, bytes=0..9, bits=43..58>(%6), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(bitfield3<unit=0, bytes=0..9, bits=43..58>(%6)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
