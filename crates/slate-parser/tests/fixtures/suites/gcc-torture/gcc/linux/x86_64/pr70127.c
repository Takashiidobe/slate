/* PR tree-optimization/70127 */

struct S {
  int        f;
  signed int g : 2;
} a[1], c = {5, 1}, d;
short b;

__attribute__((noinline, noclone)) void foo(int x) {
  if (x != 1)
    __builtin_abort();
}

int main() {
  while (b++ <= 0) {
    struct S e = {1, 1};
    d = e = a[0] = c;
  }
  foo(a[0].g);
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 f: i32;
// DEFAULT-NEXT:         field1 g: i32 : 2;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[None, Some(32)], bit_units=[(4, 1)], field_units=[None, Some(0)]];
// DEFAULT-NEXT:     global %1 a: array<@type0, 1> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: @type0 [storage=static] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(5), field1 = const<i32>(1)) [linkage=external];
// DEFAULT-NEXT:     global %3 d: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 b: i16 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %9 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %5 @foo(%6 x: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%9);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         while %10 {
// DEFAULT-NEXT:             let %11: i16 [synthetic] = read<i16>(%4);
// DEFAULT-NEXT:             let %12: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%11)), const<i32>(1)));
// DEFAULT-NEXT:             write<i16>(%4, read<i16>(%12));
// DEFAULT-NEXT:             yield le<i32>(widen<i32, reason=promotion>(read<i16>(%11)), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %8 e: @type0 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(1));
// DEFAULT-NEXT:                 write<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(1)>(%1), const<i32>(0))), copy<@type0, reason=assign>(read<@type0>(%2)));
// DEFAULT-NEXT:                 write<@type0>(%8, copy<@type0, reason=assign>(copy<@type0, reason=assign>(read<@type0>(%2))));
// DEFAULT-NEXT:                 write<@type0>(%3, copy<@type0, reason=assign>(copy<@type0, reason=assign>(copy<@type0, reason=assign>(read<@type0>(%2)))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%5, read<i32>(bitfield1<unit=0, bytes=4..5, bits=0..2>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(1)>(%1), const<i32>(0))))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
