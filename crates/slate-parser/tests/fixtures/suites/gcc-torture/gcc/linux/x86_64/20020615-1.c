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
// DEFAULT-NEXT:     type @type[[TYPE_font_hints_s:[0-9]+]] font_hints_s = struct {
// DEFAULT-NEXT:         field0 axes_swapped: i32;
// DEFAULT-NEXT:         field1 x_inverted: i32;
// DEFAULT-NEXT:         field2 y_inverted: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_font_hints:[0-9]+]] font_hints = @type[[TYPE_font_hints_s]];
// DEFAULT-NEXT:     type @type[[TYPE_gs_fixed_point_s:[0-9]+]] gs_fixed_point_s = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:         field1 y: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_gs_fixed_point:[0-9]+]] gs_fixed_point = @type[[TYPE_gs_fixed_point_s]];
// DEFAULT-NEXT:     global %[[VALUE_fh:[0-9]+]] fh: array<@type[[TYPE_font_hints_s]], 3> [storage=static] [align=16] = aggregate<array<@type[[TYPE_font_hints_s]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE_font_hints_s]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(1), field2 = const<i32>(0)), index1 = aggregate<@type[[TYPE_font_hints_s]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(1)), index2 = aggregate<@type[[TYPE_font_hints_s]], zero_fill=false>(field0 = const<i32>(0), field1 = const<i32>(0), field2 = const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_gsf:[0-9]+]] gsf: array<@type[[TYPE_gs_fixed_point_s]], 4> [storage=static] [align=16] = aggregate<array<@type[[TYPE_gs_fixed_point_s]], 4>, zero_fill=false>(index0 = aggregate<@type[[TYPE_gs_fixed_point_s]], zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(196608)), field1 = widen<i64, reason=assign>(const<i32>(80216))), index1 = aggregate<@type[[TYPE_gs_fixed_point_s]], zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(196608)), field1 = widen<i64, reason=assign>(const<i32>(98697))), index2 = aggregate<@type[[TYPE_gs_fixed_point_s]], zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(80216)), field1 = widen<i64, reason=assign>(const<i32>(196608))), index3 = aggregate<@type[[TYPE_gs_fixed_point_s]], zero_fill=false>(field0 = widen<i64, reason=assign>(const<i32>(98697)), field1 = widen<i64, reason=assign>(const<i32>(196608)))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_line_hints:[0-9]+]] @line_hints(%[[VALUE_fh_2:[0-9]+]] fh: ptr<const @type[[TYPE_font_hints_s]]>, %[[VALUE_p0:[0-9]+]] p0: ptr<const @type[[TYPE_gs_fixed_point_s]]>, %[[VALUE_p1:[0-9]+]] p1: ptr<const @type[[TYPE_gs_fixed_point_s]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_dx:[0-9]+]] dx: i64 [storage=automatic] = sub<i64, overflow=ub>(read<i64>(field0(deref(read<ptr<const @type[[TYPE_gs_fixed_point_s]]>>(%[[VALUE_p1]])))), read<i64>(field0(deref(read<ptr<const @type[[TYPE_gs_fixed_point_s]]>>(%[[VALUE_p0]])))));
// DEFAULT-NEXT:         let %[[VALUE_dy:[0-9]+]] dy: i64 [storage=automatic] = sub<i64, overflow=ub>(read<i64>(field1(deref(read<ptr<const @type[[TYPE_gs_fixed_point_s]]>>(%[[VALUE_p1]])))), read<i64>(field1(deref(read<ptr<const @type[[TYPE_gs_fixed_point_s]]>>(%[[VALUE_p0]])))));
// DEFAULT-NEXT:         let %[[VALUE_adx:[0-9]+]] adx: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ady:[0-9]+]] ady: i64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_xi:[0-9]+]] xi: i32 [storage=automatic] = read<i32>(field1(deref(read<ptr<const @type[[TYPE_font_hints_s]]>>(%[[VALUE_fh_2]]))));
// DEFAULT-NEXT:         let %[[VALUE_yi:[0-9]+]] yi: i32 [storage=automatic] = read<i32>(field2(deref(read<ptr<const @type[[TYPE_font_hints_s]]>>(%[[VALUE_fh_2]]))));
// DEFAULT-NEXT:         let %[[VALUE_hints:[0-9]+]] hints: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_xi]]), const<i32>(0))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_dx]], neg<i64, overflow=ub>(read<i64>(%[[VALUE_dx]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_yi]]), const<i32>(0))
// DEFAULT-NEXT:             write<i64>(%[[VALUE_dy]], neg<i64, overflow=ub>(read<i64>(%[[VALUE_dy]])));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(deref(read<ptr<const @type[[TYPE_font_hints_s]]>>(%[[VALUE_fh_2]])))), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_t:[0-9]+]] t: i64 [storage=automatic] = read<i64>(%[[VALUE_dx]]);
// DEFAULT-NEXT:                 let %[[VALUE_ti:[0-9]+]] ti: i32 [storage=automatic] = read<i32>(%[[VALUE_xi]]);
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_dx]], read<i64>(%[[VALUE_dy]]));
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_yi]]);
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_xi]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:                 write<i64>(%[[VALUE_dy]], read<i64>(%[[VALUE_t]]));
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_ti]]);
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_yi]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<i64>(%[[VALUE_adx]], conditional<i64>(lt<i64>(read<i64>(%[[VALUE_dx]]), widen<i64, reason=usual_arith>(const<i32>(0))), neg<i64, overflow=ub>(read<i64>(%[[VALUE_dx]])), read<i64>(%[[VALUE_dx]])));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_ady]], conditional<i64>(lt<i64>(read<i64>(%[[VALUE_dy]]), widen<i64, reason=usual_arith>(const<i32>(0))), neg<i64, overflow=ub>(read<i64>(%[[VALUE_dy]])), read<i64>(%[[VALUE_dy]])));
// DEFAULT-NEXT:         if logical_and<bool>(ne<i64>(read<i64>(%[[VALUE_dy]]), widen<i64, reason=usual_arith>(const<i32>(0))), le<i64>(read<i64>(%[[VALUE_adx]]), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_ady]]), const<i32>(4))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_hints]], conditional<i32>(gt<i64>(read<i64>(%[[VALUE_dy]]), widen<i64, reason=usual_arith>(const<i32>(0))), const<i32>(2), const<i32>(1)));
// DEFAULT-NEXT:                 if ne<i32>(read<i32>(%[[VALUE_xi]]), const<i32>(0))
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_hints]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = xor<i32>(read<i32>(%[[VALUE3]]), const<i32>(3));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_hints]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(ne<i64>(read<i64>(%[[VALUE_dx]]), widen<i64, reason=usual_arith>(const<i32>(0))), le<i64>(read<i64>(%[[VALUE_ady]]), shr<i64, amount_out_of_range=ub, fill=sign_extend>(read<i64>(%[[VALUE_adx]]), const<i32>(4))))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_hints]], conditional<i32>(lt<i64>(read<i64>(%[[VALUE_dx]]), widen<i64, reason=usual_arith>(const<i32>(0))), const<i32>(8), const<i32>(4)));
// DEFAULT-NEXT:                     if ne<i32>(read<i32>(%[[VALUE_yi]]), const<i32>(0))
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_hints]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = xor<i32>(read<i32>(%[[VALUE5]]), const<i32>(12));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_hints]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_hints]], const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_hints]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const @type[[TYPE_font_hints_s]]>, ptr<const @type[[TYPE_gs_fixed_point_s]]>, ptr<const @type[[TYPE_gs_fixed_point_s]]>) -> i32>(%[[VALUE_line_hints]], pointer_cast<ptr<const @type[[TYPE_font_hints_s]]>, reason=arg>(array_decay<ptr<@type[[TYPE_font_hints_s]]>, length=Some(3)>(%[[VALUE_fh]])), pointer_cast<ptr<const @type[[TYPE_gs_fixed_point_s]]>, reason=arg>(array_decay<ptr<@type[[TYPE_gs_fixed_point_s]]>, length=Some(4)>(%[[VALUE_gsf]])), pointer_cast<ptr<const @type[[TYPE_gs_fixed_point_s]]>, reason=arg>(ptr_offset<ptr<@type[[TYPE_gs_fixed_point_s]]>, subtract=false, element=@type[[TYPE_gs_fixed_point_s]], overflow=ub>(array_decay<ptr<@type[[TYPE_gs_fixed_point_s]]>, length=Some(4)>(%[[VALUE_gsf]]), const<i32>(1)))), const<i32>(1))
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE7]], ne<i32>(call<i32, signature=fn(ptr<const @type[[TYPE_font_hints_s]]>, ptr<const @type[[TYPE_gs_fixed_point_s]]>, ptr<const @type[[TYPE_gs_fixed_point_s]]>) -> i32>(%[[VALUE_line_hints]], pointer_cast<ptr<const @type[[TYPE_font_hints_s]]>, reason=arg>(ptr_offset<ptr<@type[[TYPE_font_hints_s]]>, subtract=false, element=@type[[TYPE_font_hints_s]], overflow=ub>(array_decay<ptr<@type[[TYPE_font_hints_s]]>, length=Some(3)>(%[[VALUE_fh]]), const<i32>(1))), pointer_cast<ptr<const @type[[TYPE_gs_fixed_point_s]]>, reason=arg>(ptr_offset<ptr<@type[[TYPE_gs_fixed_point_s]]>, subtract=false, element=@type[[TYPE_gs_fixed_point_s]], overflow=ub>(array_decay<ptr<@type[[TYPE_gs_fixed_point_s]]>, length=Some(4)>(%[[VALUE_gsf]]), const<i32>(2))), pointer_cast<ptr<const @type[[TYPE_gs_fixed_point_s]]>, reason=arg>(ptr_offset<ptr<@type[[TYPE_gs_fixed_point_s]]>, subtract=false, element=@type[[TYPE_gs_fixed_point_s]], overflow=ub>(array_decay<ptr<@type[[TYPE_gs_fixed_point_s]]>, length=Some(4)>(%[[VALUE_gsf]]), const<i32>(3)))), const<i32>(8)));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE7]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE8]], ne<i32>(call<i32, signature=fn(ptr<const @type[[TYPE_font_hints_s]]>, ptr<const @type[[TYPE_gs_fixed_point_s]]>, ptr<const @type[[TYPE_gs_fixed_point_s]]>) -> i32>(%[[VALUE_line_hints]], pointer_cast<ptr<const @type[[TYPE_font_hints_s]]>, reason=arg>(ptr_offset<ptr<@type[[TYPE_font_hints_s]]>, subtract=false, element=@type[[TYPE_font_hints_s]], overflow=ub>(array_decay<ptr<@type[[TYPE_font_hints_s]]>, length=Some(3)>(%[[VALUE_fh]]), const<i32>(2))), pointer_cast<ptr<const @type[[TYPE_gs_fixed_point_s]]>, reason=arg>(ptr_offset<ptr<@type[[TYPE_gs_fixed_point_s]]>, subtract=false, element=@type[[TYPE_gs_fixed_point_s]], overflow=ub>(array_decay<ptr<@type[[TYPE_gs_fixed_point_s]]>, length=Some(4)>(%[[VALUE_gsf]]), const<i32>(2))), pointer_cast<ptr<const @type[[TYPE_gs_fixed_point_s]]>, reason=arg>(ptr_offset<ptr<@type[[TYPE_gs_fixed_point_s]]>, subtract=false, element=@type[[TYPE_gs_fixed_point_s]], overflow=ub>(array_decay<ptr<@type[[TYPE_gs_fixed_point_s]]>, length=Some(4)>(%[[VALUE_gsf]]), const<i32>(3)))), const<i32>(4)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE8]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
