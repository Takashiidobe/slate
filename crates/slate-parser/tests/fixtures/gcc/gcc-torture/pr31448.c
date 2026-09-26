/* PR middle-end/31448, this used to ICE during expand because
   reduce_to_bit_field_precision was not ready to handle constants. */
/* { dg-require-effective-target int32plus } */

typedef struct _st {
  int iIndex  : 24;
  int iIndex1 : 24;
} st;
st  *next;
void g(void) {
  st              *next = 0;
  int              nIndx;
  const static int constreg[] = {
      0,
  };
  nIndx        = 0;
  next->iIndex = constreg[nIndx];
}
void f(void) {
  int              nIndx;
  const static int constreg[] = {
      0xFEFEFEFE,
  };
  nIndx         = 0;
  next->iIndex  = constreg[nIndx];
  next->iIndex1 = constreg[nIndx];
}
int main(void) {
  st a;
  next = &a;
  f();
  if (next->iIndex != 0xFFFEFEFE)
    __builtin_abort();
  if (next->iIndex1 != 0xFFFEFEFE)
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
// DEFAULT-NEXT:     type @type0 _st = struct {
// DEFAULT-NEXT:         field0 iIndex: i32 : 24;
// DEFAULT-NEXT:         field1 iIndex1: i32 : 24;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4], bit_offsets=[Some(0), Some(32)], bit_units=[(0, 3), (4, 3)], field_units=[Some(0), Some(1)]];
// DEFAULT-NEXT:     type @type1 st = @type0;
// DEFAULT-NEXT:     global %2 next: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 constreg: array<i32, 1> [storage=static] [const] = aggregate<array<i32, 1>, zero_fill=false>(index0 = const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %9 constreg: array<i32, 1> [storage=static] [const] = aggregate<array<i32, 1>, zero_fill=false>(index0 = reinterpret<i32, reason=assign, fits=unknown>(const<u32>(4278124286))) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @g() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 next: ptr<@type0> [storage=automatic] = null<ptr<@type0>>;
// DEFAULT-NEXT:         let %5 nIndx: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%5, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..3, bits=0..24>(deref(read<ptr<@type0>>(%4))), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(1)>(%6), read<i32>(%5)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 nIndx: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%8, const<i32>(0));
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..3, bits=0..24>(deref(read<ptr<@type0>>(%2))), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(1)>(%9), read<i32>(%8)))));
// DEFAULT-NEXT:         write<i32>(bitfield1<unit=1, bytes=4..7, bits=0..24>(deref(read<ptr<@type0>>(%2))), read<i32>(deref(ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<const i32>, length=Some(1)>(%9), read<i32>(%8)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 a: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type0>>(%2, addr_of<ptr<@type0>>(%11));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%7);
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(bitfield0<unit=0, bytes=0..3, bits=0..24>(deref(read<ptr<@type0>>(%2))))), const<u32>(4294901502))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(read<i32>(bitfield1<unit=1, bytes=4..7, bits=0..24>(deref(read<ptr<@type0>>(%2))))), const<u32>(4294901502))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
