/* PR target/7042.  When reorg.c changed branches into return insns, it
   completely forgot about any current_function_epilogue_delay_list and
   dropped those insns.  Uncovered on cris-axis-elf, where an insn in an
   epilogue delay-slot set the return-value register with the testcase
   below.  Derived from ghostscript-6.52 (GPL) by hp@axis.com.  */

void abort(void);
void exit(int);

typedef struct font_hints_s {
  int axes_swapped;
  int x_inverted, y_inverted;
} font_hints;
typedef struct gs_fixed_point_s {
  long x, y;
} gs_fixed_point;

int line_hints(const font_hints *fh, const gs_fixed_point *p0,
               const gs_fixed_point *p1) {
  long dx = p1->x - p0->x;
  long dy = p1->y - p0->y;
  long adx, ady;
  int  xi = fh->x_inverted, yi = fh->y_inverted;
  int  hints;
  if (xi)
    dx = -dx;
  if (yi)
    dy = -dy;
  if (fh->axes_swapped) {
    long t  = dx;
    int  ti = xi;
    dx = dy, xi = yi;
    dy = t, yi = ti;
  }
  adx = dx < 0 ? -dx : dx;
  ady = dy < 0 ? -dy : dy;
  if (dy != 0 && (adx <= ady >> 4)) {
    hints = dy > 0 ? 2 : 1;
    if (xi)
      hints ^= 3;
  } else if (dx != 0 && (ady <= adx >> 4)) {
    hints = dx < 0 ? 8 : 4;
    if (yi)
      hints ^= 12;
  } else
    hints = 0;
  return hints;
}
int main() {
  static font_hints     fh[]  = {{0, 1, 0}, {0, 0, 1}, {0, 0, 0}};
  static gs_fixed_point gsf[] = {{0x30000, 0x13958},
                                 {0x30000, 0x18189},
                                 {0x13958, 0x30000},
                                 {0x18189, 0x30000}};
  if (line_hints(fh, gsf, gsf + 1) != 1 ||
      line_hints(fh + 1, gsf + 2, gsf + 3) != 8 ||
      line_hints(fh + 2, gsf + 2, gsf + 3) != 4)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type0 font_hints_s = struct {
// DEFAULT-NEXT:         field0 axes_swapped: i32;
// DEFAULT-NEXT:         field1 x_inverted: i32;
// DEFAULT-NEXT:         field2 y_inverted: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 font_hints = @type0;
// DEFAULT-NEXT:     type @type2 gs_fixed_point_s = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:         field1 y: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 gs_fixed_point = @type2;
// DEFAULT-NEXT:     global %20 fh: array<@type0, 3> [storage=static] [align=16] = aggregate<array<@type0, 3>, zero_fill=false>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(1), field2 = const<i32>(0)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(1)), index2 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %21 gsf: array<@type2, 4> [storage=static] [align=16] = aggregate<array<@type2, 4>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(196608)), field1 = widen<i64, reason=assign>(const<i32>(80216))), index1 = aggregate<@type2, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(196608)), field1 = widen<i64, reason=assign>(const<i32>(98697))), index2 = aggregate<@type2, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(80216)), field1 = widen<i64, reason=assign>(const<i32>(196608))), index3 = aggregate<@type2, zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(98697)), field1 = widen<i64, reason=assign>(const<i32>(196608)))) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%22 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @line_hints(%7 fh: ptr<const @type0>, %8 p0: ptr<const @type2>, %9 p1: ptr<const @type2>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 dx: i64 [storage=automatic] = sub<i64, overflow=ub>(read<i64>(field0(deref(read<ptr<const @type2>>(%9)))), read<i64>(field0(deref(read<ptr<const @type2>>(%8)))));
// DEFAULT-NEXT:         let %11 dy: i64 [storage=automatic] = sub<i64, overflow=ub>(read<i64>(field1(deref(read<ptr<const @type2>>(%9)))), read<i64>(field1(deref(read<ptr<const @type2>>(%8)))));
// DEFAULT-NEXT:         let %12 adx: i64 [storage=automatic];
// DEFAULT-NEXT:         let %13 ady: i64 [storage=automatic];
// DEFAULT-NEXT:         let %14 xi: i32 [storage=automatic] = read<i32>(field1(deref(read<ptr<const @type0>>(%7))));
// DEFAULT-NEXT:         let %15 yi: i32 [storage=automatic] = read<i32>(field2(deref(read<ptr<const @type0>>(%7))));
// DEFAULT-NEXT:         let %16 hints: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%14), const<i32>(0))
// DEFAULT-NEXT:             write<i64>(%10, neg<i64, overflow=ub>(read<i64>(%10)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%15), const<i32>(0))
// DEFAULT-NEXT:             write<i64>(%11, neg<i64, overflow=ub>(read<i64>(%11)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(deref(read<ptr<const @type0>>(%7)))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %17 t: i64 [storage=automatic] = read<i64>(%10);
// DEFAULT-NEXT:                 let %18 ti: i32 [storage=automatic] = read<i32>(%14);
// DEFAULT-NEXT:                 write<i64>(%10, read<i64>(%11));
// DEFAULT-NEXT:                 write<i32>(%14, read<i32>(%15));
// DEFAULT-NEXT:                 write<i64>(%11, read<i64>(%17));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%18));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i64>(%12, conditional<i64>(lt<i64>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(0))), neg<i64, overflow=ub>(read<i64>(%10)), read<i64>(%10)));
// DEFAULT-NEXT:         write<i64>(%13, conditional<i64>(lt<i64>(read<i64>(%11), widen<i64, reason=usual_arith>(const<i32>(0))), neg<i64, overflow=ub>(read<i64>(%11)), read<i64>(%11)));
// DEFAULT-NEXT:         if logical_and<bool>(ne<i64>(read<i64>(%11), widen<i64, reason=usual_arith>(const<i32>(0))), le<i64>(read<i64>(%12), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%13), const<i32>(4))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%16, conditional<i32>(gt<i64>(read<i64>(%11), widen<i64, reason=usual_arith>(const<i32>(0))), const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%14), const<i32>(0))
// DEFAULT-NEXT:                     let %23: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                     let %24: i32 [synthetic] = xor<i32>(read<i32>(%23), const<i32>(3));
// DEFAULT-NEXT:                     write<i32>(%16, read<i32>(%24));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(ne<i64>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(0))), le<i64>(read<i64>(%13), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%12), const<i32>(4))))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%16, conditional<i32>(lt<i64>(read<i64>(%10), widen<i64, reason=usual_arith>(const<i32>(0))), const<i32>(8), const<i32>(4)));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%15), const<i32>(0))
// DEFAULT-NEXT:                         let %25: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                         let %26: i32 [synthetic] = xor<i32>(read<i32>(%25), const<i32>(12));
// DEFAULT-NEXT:                         write<i32>(%16, read<i32>(%26));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %27: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const @type0>, ptr<const @type2>, ptr<const @type2>) -> i32>(%6, pointer_cast<ptr<const @type0>, reason=arg>(array_decay<ptr<@type0>, length=Some(3)>(%20)), pointer_cast<ptr<const @type2>, reason=arg>(array_decay<ptr<@type2>, length=Some(4)>(%21)), pointer_cast<ptr<const @type2>, reason=arg>(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%21), const<i32>(1)))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%27, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%27, ne<i32>(call<i32, signature=fn(ptr<const @type0>, ptr<const @type2>, ptr<const @type2>) -> i32>(%6, pointer_cast<ptr<const @type0>, reason=arg>(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%20), const<i32>(1))), pointer_cast<ptr<const @type2>, reason=arg>(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%21), const<i32>(2))), pointer_cast<ptr<const @type2>, reason=arg>(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%21), const<i32>(3)))), const<i32>(8)));
// DEFAULT-NEXT:         let %28: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%27)
// DEFAULT-NEXT:             write<bool>(%28, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%28, ne<i32>(call<i32, signature=fn(ptr<const @type0>, ptr<const @type2>, ptr<const @type2>) -> i32>(%6, pointer_cast<ptr<const @type0>, reason=arg>(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(3)>(%20), const<i32>(2))), pointer_cast<ptr<const @type2>, reason=arg>(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%21), const<i32>(2))), pointer_cast<ptr<const @type2>, reason=arg>(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%21), const<i32>(3)))), const<i32>(4)));
// DEFAULT-NEXT:         if read<bool>(%28)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
