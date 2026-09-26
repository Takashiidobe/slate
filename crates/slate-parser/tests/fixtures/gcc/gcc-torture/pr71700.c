struct S {
  signed   f0 : 16;
  unsigned f1 : 1;
};

int             b;
static struct S c[] = {{-1, 0}, {-1, 0}};
struct S        d;

int main() {
  struct S e = c[0];
  d          = e;
  if (d.f1 != 0)
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 f0: i32 : 16;
// DEFAULT-NEXT:         field1 f1: u32 : 1;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 2], bit_offsets=[Some(0), Some(16)], bit_units=[(0, 3)], field_units=[Some(0), Some(0)]];
// DEFAULT-NEXT:     global %1 b: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 c: array<@type0, 2> [storage=static] = aggregate<array<@type0, 2>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0))), index1 = aggregate<@type0, zero_fill=false>(field0 = neg<i32, overflow=ub>(const<i32>(1)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)))) [linkage=internal];
// DEFAULT-NEXT:     global %3 d: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %6 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 e: @type0 [storage=automatic] = copy<@type0, reason=assign>(read<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%2), const<i32>(0)))));
// DEFAULT-NEXT:         write<@type0>(%3, copy<@type0, reason=assign>(read<@type0>(%5)));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..3, bits=16..17>(%3))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
