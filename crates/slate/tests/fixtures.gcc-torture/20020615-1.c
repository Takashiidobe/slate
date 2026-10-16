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

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/7042.  When reorg.c changed branches into return insns, it
// CHECK-NEXT: //! completely forgot about any current_function_epilogue_delay_list and
// CHECK-NEXT: //! dropped those insns.  Uncovered on cris-axis-elf, where an insn in an
// CHECK-NEXT: //! epilogue delay-slot set the return-value register with the testcase
// CHECK-NEXT: //! below.  Derived from ghostscript-6.52 (GPL) by hp@axis.com.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct font_hints_s {
// CHECK-NEXT:     axes_swapped: i32,
// CHECK-NEXT:     x_inverted: i32,
// CHECK-NEXT:     y_inverted: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct gs_fixed_point_s {
// CHECK-NEXT:     x: i64,
// CHECK-NEXT:     y: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn line_hints(
// CHECK-NEXT:     mut [[FH_0:fh_[0-9]+]]: *const font_hints_s,
// CHECK-NEXT:     mut p0: *const gs_fixed_point_s,
// CHECK-NEXT:     mut p1: *const gs_fixed_point_s,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let mut dx: i64 = (unsafe { (*p1).x }) - unsafe { (*p0).x };
// CHECK-NEXT:     let mut dy: i64 = (unsafe { (*p1).y }) - unsafe { (*p0).y };
// CHECK-NEXT:     let mut adx: i64 = 0 as i64;
// CHECK-NEXT:     let mut ady: i64 = 0 as i64;
// CHECK-NEXT:     let mut xi: i32 = unsafe { (*[[FH_0]]).x_inverted };
// CHECK-NEXT:     let mut yi: i32 = unsafe { (*[[FH_0]]).y_inverted };
// CHECK-NEXT:     let mut hints: i32 = 0 as i32;
// CHECK-NEXT:     if xi != (0 as i32) {
// CHECK-NEXT:         dx = -dx;
// CHECK-NEXT:     }
// CHECK-NEXT:     if yi != (0 as i32) {
// CHECK-NEXT:         dy = -dy;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { (*[[FH_0]]).axes_swapped }) != (0 as i32) {
// CHECK-NEXT:         let mut t: i64 = dx;
// CHECK-NEXT:         let mut ti: i32 = xi;
// CHECK-NEXT:         dx = dy;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = yi;
// CHECK-NEXT:         xi = [[V_1]];
// CHECK-NEXT:         dy = t;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = ti;
// CHECK-NEXT:         yi = [[V_2]];
// CHECK-NEXT:     }
// CHECK-NEXT:     adx = if dx < ((0 as i32) as i64) { -dx } else { dx };
// CHECK-NEXT:     ady = if dy < ((0 as i32) as i64) { -dy } else { dy };
// CHECK-NEXT:     if dy != ((0 as i32) as i64) && adx <= ady >> (4 as i32) {
// CHECK-NEXT:         hints = if dy > ((0 as i32) as i64) {
// CHECK-NEXT:             2 as i32
// CHECK-NEXT:         } else {
// CHECK-NEXT:             1 as i32
// CHECK-NEXT:         };
// CHECK-NEXT:         if xi != (0 as i32) {
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = hints;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] ^ (3 as i32);
// CHECK-NEXT:             hints = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if dx != ((0 as i32) as i64) && ady <= adx >> (4 as i32) {
// CHECK-NEXT:             hints = if dx < ((0 as i32) as i64) {
// CHECK-NEXT:                 8 as i32
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 4 as i32
// CHECK-NEXT:             };
// CHECK-NEXT:             if yi != (0 as i32) {
// CHECK-NEXT:                 let mut [[V_5:__v[0-9]+]]: i32 = hints;
// CHECK-NEXT:                 let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] ^ (12 as i32);
// CHECK-NEXT:                 hints = [[V_6]];
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             hints = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     hints
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[V_7:__v[0-9]+]]: bool;
// CHECK-NEXT:     if line_hints(
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(fh.0) as *mut font_hints_s }) as *const font_hints_s,
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(gsf.0) as *mut gs_fixed_point_s })
// CHECK-NEXT:             as *const gs_fixed_point_s,
// CHECK-NEXT:         (unsafe {
// CHECK-NEXT:             unsafe { std::ptr::addr_of_mut!(gsf.0) as *mut gs_fixed_point_s }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         }) as *const gs_fixed_point_s,
// CHECK-NEXT:     ) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         [[V_7]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_7]] = line_hints(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(fh.0) as *mut font_hints_s }
// CHECK-NEXT:                     .offset((1 as i32) as isize)
// CHECK-NEXT:             }) as *const font_hints_s,
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gsf.0) as *mut gs_fixed_point_s }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             }) as *const gs_fixed_point_s,
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gsf.0) as *mut gs_fixed_point_s }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             }) as *const gs_fixed_point_s,
// CHECK-NEXT:         ) != (8 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut [[V_8:__v[0-9]+]]: bool;
// CHECK-NEXT:     if [[V_7]] {
// CHECK-NEXT:         [[V_8]] = true as bool;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         [[V_8]] = line_hints(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(fh.0) as *mut font_hints_s }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             }) as *const font_hints_s,
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gsf.0) as *mut gs_fixed_point_s }
// CHECK-NEXT:                     .offset((2 as i32) as isize)
// CHECK-NEXT:             }) as *const gs_fixed_point_s,
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(gsf.0) as *mut gs_fixed_point_s }
// CHECK-NEXT:                     .offset((3 as i32) as isize)
// CHECK-NEXT:             }) as *const gs_fixed_point_s,
// CHECK-NEXT:         ) != (4 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     if [[V_8]] {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut fh: __SlateAlign16<[font_hints_s; 3]> = __SlateAlign16([
// CHECK-NEXT:     font_hints_s {
// CHECK-NEXT:         axes_swapped: 0 as i32,
// CHECK-NEXT:         x_inverted: 1 as i32,
// CHECK-NEXT:         y_inverted: 0 as i32,
// CHECK-NEXT:     },
// CHECK-NEXT:     font_hints_s {
// CHECK-NEXT:         axes_swapped: 0 as i32,
// CHECK-NEXT:         x_inverted: 0 as i32,
// CHECK-NEXT:         y_inverted: 1 as i32,
// CHECK-NEXT:     },
// CHECK-NEXT:     font_hints_s {
// CHECK-NEXT:         axes_swapped: 0 as i32,
// CHECK-NEXT:         x_inverted: 0 as i32,
// CHECK-NEXT:         y_inverted: 0 as i32,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: static mut gsf: __SlateAlign16<[gs_fixed_point_s; 4]> = __SlateAlign16([
// CHECK-NEXT:     gs_fixed_point_s {
// CHECK-NEXT:         x: (196608 as i32) as i64,
// CHECK-NEXT:         y: (80216 as i32) as i64,
// CHECK-NEXT:     },
// CHECK-NEXT:     gs_fixed_point_s {
// CHECK-NEXT:         x: (196608 as i32) as i64,
// CHECK-NEXT:         y: (98697 as i32) as i64,
// CHECK-NEXT:     },
// CHECK-NEXT:     gs_fixed_point_s {
// CHECK-NEXT:         x: (80216 as i32) as i64,
// CHECK-NEXT:         y: (196608 as i32) as i64,
// CHECK-NEXT:     },
// CHECK-NEXT:     gs_fixed_point_s {
// CHECK-NEXT:         x: (98697 as i32) as i64,
// CHECK-NEXT:         y: (196608 as i32) as i64,
// CHECK-NEXT:     },
// CHECK-NEXT: ]);
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
