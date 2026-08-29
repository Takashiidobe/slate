#include <stdio.h>

int main(void) {
  static const unsigned char data[] = {
#embed "c23_embed.bin"
  };
  static const unsigned char framed[] = {
    0xAA,
#embed "c23_embed.bin" prefix(0xBB,) suffix(, 0xCC)
    , 0xDD
  };
  static const unsigned char empty[] = {
#embed "c23_embed_empty.bin" if_empty(0xEE)
  };
  for (size_t i = 0; i < sizeof(data); i++) {
    putchar(data[i]);
  }
  int framed_ok = sizeof(framed) == 8 && framed[0] == 0xAA &&
                  framed[1] == 0xBB && framed[2] == 'C' && framed[3] == '2' &&
                  framed[4] == '3' && framed[5] == '\n' &&
                  framed[6] == 0xCC && framed[7] == 0xDD;
  int empty_ok = sizeof(empty) == 1 && empty[0] == 0xEE;
  return sizeof(data) == 4 && data[0] == 'C' && data[1] == '2' &&
                 data[2] == '3' && data[3] == '\n' && framed_ok && empty_ok
             ? 0
             : 1;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut main_data: [u8; 4] = [67, 50, 51, 10];
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut main_empty: [u8; 1] = [238];
// LOWERING-EMPTY:
// LOWERING-NEXT: static mut main_framed: [u8; 8] = [170, 187, 67, 50, 51, 10, 204, 221];
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn putchar(_0: i32) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut framed_ok: i32 = 0;
// LOWERING-NEXT:     let mut empty_ok: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: u64 = 0;
// LOWERING-NEXT:         let _v1: u64 = 0;
// LOWERING-NEXT:         i = _v1;
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let _v2: u64 = i;
// LOWERING-NEXT:             let _v3: u64 = 4;
// LOWERING-NEXT:             let _v4: bool = _v2 < _v3;
// LOWERING-NEXT:             if !_v4 {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let _v5: u64 = i;
// LOWERING-NEXT:                 let _v6: u8 = unsafe { main_data[(_v5 as usize)] };
// LOWERING-NEXT:                 let _v7: i32 = _v6 as i32;
// LOWERING-NEXT:                 let _v8: i32 = unsafe { putchar(_v7 as i32) };
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let _v9: u64 = i;
// LOWERING-NEXT:             let _v10: u64 = _v9 + 1;
// LOWERING-NEXT:             i = _v10;
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v11: u64 = 8;
// LOWERING-NEXT:     let _v12: u64 = 8;
// LOWERING-NEXT:     let _v13: bool = _v11 == _v12;
// LOWERING-NEXT:     let _v14: bool = if _v13 {
// LOWERING-NEXT:         let _v15: i64 = 0;
// LOWERING-NEXT:         let _v16: u8 = unsafe { main_framed[(_v15 as usize)] };
// LOWERING-NEXT:         let _v17: i32 = _v16 as i32;
// LOWERING-NEXT:         let _v18: i32 = 170;
// LOWERING-NEXT:         let _v19: bool = _v17 == _v18;
// LOWERING-NEXT:         _v19
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v20: bool = false;
// LOWERING-NEXT:         _v20
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v21: bool = if _v14 {
// LOWERING-NEXT:         let _v22: i64 = 1;
// LOWERING-NEXT:         let _v23: u8 = unsafe { main_framed[(_v22 as usize)] };
// LOWERING-NEXT:         let _v24: i32 = _v23 as i32;
// LOWERING-NEXT:         let _v25: i32 = 187;
// LOWERING-NEXT:         let _v26: bool = _v24 == _v25;
// LOWERING-NEXT:         _v26
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v27: bool = false;
// LOWERING-NEXT:         _v27
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v28: bool = if _v21 {
// LOWERING-NEXT:         let _v29: i64 = 2;
// LOWERING-NEXT:         let _v30: u8 = unsafe { main_framed[(_v29 as usize)] };
// LOWERING-NEXT:         let _v31: i32 = _v30 as i32;
// LOWERING-NEXT:         let _v32: i32 = 67;
// LOWERING-NEXT:         let _v33: bool = _v31 == _v32;
// LOWERING-NEXT:         _v33
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v34: bool = false;
// LOWERING-NEXT:         _v34
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v35: bool = if _v28 {
// LOWERING-NEXT:         let _v36: i64 = 3;
// LOWERING-NEXT:         let _v37: u8 = unsafe { main_framed[(_v36 as usize)] };
// LOWERING-NEXT:         let _v38: i32 = _v37 as i32;
// LOWERING-NEXT:         let _v39: i32 = 50;
// LOWERING-NEXT:         let _v40: bool = _v38 == _v39;
// LOWERING-NEXT:         _v40
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v41: bool = false;
// LOWERING-NEXT:         _v41
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v42: bool = if _v35 {
// LOWERING-NEXT:         let _v43: i64 = 4;
// LOWERING-NEXT:         let _v44: u8 = unsafe { main_framed[(_v43 as usize)] };
// LOWERING-NEXT:         let _v45: i32 = _v44 as i32;
// LOWERING-NEXT:         let _v46: i32 = 51;
// LOWERING-NEXT:         let _v47: bool = _v45 == _v46;
// LOWERING-NEXT:         _v47
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v48: bool = false;
// LOWERING-NEXT:         _v48
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v49: bool = if _v42 {
// LOWERING-NEXT:         let _v50: i64 = 5;
// LOWERING-NEXT:         let _v51: u8 = unsafe { main_framed[(_v50 as usize)] };
// LOWERING-NEXT:         let _v52: i32 = _v51 as i32;
// LOWERING-NEXT:         let _v53: i32 = 10;
// LOWERING-NEXT:         let _v54: bool = _v52 == _v53;
// LOWERING-NEXT:         _v54
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v55: bool = false;
// LOWERING-NEXT:         _v55
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v56: bool = if _v49 {
// LOWERING-NEXT:         let _v57: i64 = 6;
// LOWERING-NEXT:         let _v58: u8 = unsafe { main_framed[(_v57 as usize)] };
// LOWERING-NEXT:         let _v59: i32 = _v58 as i32;
// LOWERING-NEXT:         let _v60: i32 = 204;
// LOWERING-NEXT:         let _v61: bool = _v59 == _v60;
// LOWERING-NEXT:         _v61
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v62: bool = false;
// LOWERING-NEXT:         _v62
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v63: bool = if _v56 {
// LOWERING-NEXT:         let _v64: i64 = 7;
// LOWERING-NEXT:         let _v65: u8 = unsafe { main_framed[(_v64 as usize)] };
// LOWERING-NEXT:         let _v66: i32 = _v65 as i32;
// LOWERING-NEXT:         let _v67: i32 = 221;
// LOWERING-NEXT:         let _v68: bool = _v66 == _v67;
// LOWERING-NEXT:         _v68
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v69: bool = false;
// LOWERING-NEXT:         _v69
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v70: i32 = _v63 as i32;
// LOWERING-NEXT:     framed_ok = _v70;
// LOWERING-NEXT:     let _v71: u64 = 1;
// LOWERING-NEXT:     let _v72: u64 = 1;
// LOWERING-NEXT:     let _v73: bool = _v71 == _v72;
// LOWERING-NEXT:     let _v74: bool = if _v73 {
// LOWERING-NEXT:         let _v75: i64 = 0;
// LOWERING-NEXT:         let _v76: u8 = unsafe { main_empty[(_v75 as usize)] };
// LOWERING-NEXT:         let _v77: i32 = _v76 as i32;
// LOWERING-NEXT:         let _v78: i32 = 238;
// LOWERING-NEXT:         let _v79: bool = _v77 == _v78;
// LOWERING-NEXT:         _v79
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v80: bool = false;
// LOWERING-NEXT:         _v80
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v81: i32 = _v74 as i32;
// LOWERING-NEXT:     empty_ok = _v81;
// LOWERING-NEXT:     let _v82: u64 = 4;
// LOWERING-NEXT:     let _v83: u64 = 4;
// LOWERING-NEXT:     let _v84: bool = _v82 == _v83;
// LOWERING-NEXT:     let _v85: bool = if _v84 {
// LOWERING-NEXT:         let _v86: i64 = 0;
// LOWERING-NEXT:         let _v87: u8 = unsafe { main_data[(_v86 as usize)] };
// LOWERING-NEXT:         let _v88: i32 = _v87 as i32;
// LOWERING-NEXT:         let _v89: i32 = 67;
// LOWERING-NEXT:         let _v90: bool = _v88 == _v89;
// LOWERING-NEXT:         _v90
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v91: bool = false;
// LOWERING-NEXT:         _v91
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v92: bool = if _v85 {
// LOWERING-NEXT:         let _v93: i64 = 1;
// LOWERING-NEXT:         let _v94: u8 = unsafe { main_data[(_v93 as usize)] };
// LOWERING-NEXT:         let _v95: i32 = _v94 as i32;
// LOWERING-NEXT:         let _v96: i32 = 50;
// LOWERING-NEXT:         let _v97: bool = _v95 == _v96;
// LOWERING-NEXT:         _v97
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v98: bool = false;
// LOWERING-NEXT:         _v98
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v99: bool = if _v92 {
// LOWERING-NEXT:         let _v100: i64 = 2;
// LOWERING-NEXT:         let _v101: u8 = unsafe { main_data[(_v100 as usize)] };
// LOWERING-NEXT:         let _v102: i32 = _v101 as i32;
// LOWERING-NEXT:         let _v103: i32 = 51;
// LOWERING-NEXT:         let _v104: bool = _v102 == _v103;
// LOWERING-NEXT:         _v104
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v105: bool = false;
// LOWERING-NEXT:         _v105
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v106: bool = if _v99 {
// LOWERING-NEXT:         let _v107: i64 = 3;
// LOWERING-NEXT:         let _v108: u8 = unsafe { main_data[(_v107 as usize)] };
// LOWERING-NEXT:         let _v109: i32 = _v108 as i32;
// LOWERING-NEXT:         let _v110: i32 = 10;
// LOWERING-NEXT:         let _v111: bool = _v109 == _v110;
// LOWERING-NEXT:         _v111
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v112: bool = false;
// LOWERING-NEXT:         _v112
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v113: bool = if _v106 {
// LOWERING-NEXT:         let _v114: i32 = framed_ok;
// LOWERING-NEXT:         let _v115: bool = _v114 != 0;
// LOWERING-NEXT:         _v115
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v116: bool = false;
// LOWERING-NEXT:         _v116
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v117: bool = if _v113 {
// LOWERING-NEXT:         let _v118: i32 = empty_ok;
// LOWERING-NEXT:         let _v119: bool = _v118 != 0;
// LOWERING-NEXT:         _v119
// LOWERING-NEXT:     } else {
// LOWERING-NEXT:         let _v120: bool = false;
// LOWERING-NEXT:         _v120
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let _v121: i32 = 0;
// LOWERING-NEXT:     let _v122: i32 = 1;
// LOWERING-NEXT:     let _v123: i32 = if _v117 { _v121 } else { _v122 };
// LOWERING-NEXT:     __retval = _v123;
// LOWERING-NEXT:     let _v124: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v124 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut main_data: [u8; 4] = [67, 50, 51, 10];
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut main_empty: [u8; 1] = [238];
// REWRITES-EMPTY:
// REWRITES-NEXT: static mut main_framed: [u8; 8] = [170, 187, 67, 50, 51, 10, 204, 221];
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn putchar(_0: i32) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut framed_ok: i32 = 0;
// REWRITES-NEXT: let mut empty_ok: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: u64 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:                     let _v3: u64 = 4;
// REWRITES-NEXT:                     if !(i < _v3) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     {
// REWRITES-NEXT:                                     let _v8: i32 = unsafe { putchar(((unsafe { main_data[(i as usize)] }) as i32) as i32) };
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v11: u64 = 8;
// REWRITES-NEXT: let _v12: u64 = 8;
// REWRITES-NEXT: let _v14: bool = if _v11 == _v12 {
// REWRITES-NEXT:         let _v15: i64 = 0;
// REWRITES-NEXT:         let _v18: i32 = 170;
// REWRITES-NEXT:         let _v19: bool = ((unsafe { main_framed[(_v15 as usize)] }) as i32) == _v18;
// REWRITES-NEXT:     _v19
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v20: bool = false;
// REWRITES-NEXT:     _v20
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v21: bool = if _v14 {
// REWRITES-NEXT:         let _v22: i64 = 1;
// REWRITES-NEXT:         let _v25: i32 = 187;
// REWRITES-NEXT:         let _v26: bool = ((unsafe { main_framed[(_v22 as usize)] }) as i32) == _v25;
// REWRITES-NEXT:     _v26
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v27: bool = false;
// REWRITES-NEXT:     _v27
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v28: bool = if _v21 {
// REWRITES-NEXT:         let _v29: i64 = 2;
// REWRITES-NEXT:         let _v32: i32 = 67;
// REWRITES-NEXT:         let _v33: bool = ((unsafe { main_framed[(_v29 as usize)] }) as i32) == _v32;
// REWRITES-NEXT:     _v33
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v34: bool = false;
// REWRITES-NEXT:     _v34
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v35: bool = if _v28 {
// REWRITES-NEXT:         let _v36: i64 = 3;
// REWRITES-NEXT:         let _v39: i32 = 50;
// REWRITES-NEXT:         let _v40: bool = ((unsafe { main_framed[(_v36 as usize)] }) as i32) == _v39;
// REWRITES-NEXT:     _v40
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v41: bool = false;
// REWRITES-NEXT:     _v41
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v42: bool = if _v35 {
// REWRITES-NEXT:         let _v43: i64 = 4;
// REWRITES-NEXT:         let _v46: i32 = 51;
// REWRITES-NEXT:         let _v47: bool = ((unsafe { main_framed[(_v43 as usize)] }) as i32) == _v46;
// REWRITES-NEXT:     _v47
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v48: bool = false;
// REWRITES-NEXT:     _v48
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v49: bool = if _v42 {
// REWRITES-NEXT:         let _v50: i64 = 5;
// REWRITES-NEXT:         let _v53: i32 = 10;
// REWRITES-NEXT:         let _v54: bool = ((unsafe { main_framed[(_v50 as usize)] }) as i32) == _v53;
// REWRITES-NEXT:     _v54
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v55: bool = false;
// REWRITES-NEXT:     _v55
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v56: bool = if _v49 {
// REWRITES-NEXT:         let _v57: i64 = 6;
// REWRITES-NEXT:         let _v60: i32 = 204;
// REWRITES-NEXT:         let _v61: bool = ((unsafe { main_framed[(_v57 as usize)] }) as i32) == _v60;
// REWRITES-NEXT:     _v61
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v62: bool = false;
// REWRITES-NEXT:     _v62
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v63: bool = if _v56 {
// REWRITES-NEXT:         let _v64: i64 = 7;
// REWRITES-NEXT:         let _v67: i32 = 221;
// REWRITES-NEXT:         let _v68: bool = ((unsafe { main_framed[(_v64 as usize)] }) as i32) == _v67;
// REWRITES-NEXT:     _v68
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v69: bool = false;
// REWRITES-NEXT:     _v69
// REWRITES-NEXT: };
// REWRITES-NEXT: framed_ok = _v63 as i32;
// REWRITES-NEXT: let _v71: u64 = 1;
// REWRITES-NEXT: let _v72: u64 = 1;
// REWRITES-NEXT: let _v74: bool = if _v71 == _v72 {
// REWRITES-NEXT:         let _v75: i64 = 0;
// REWRITES-NEXT:         let _v78: i32 = 238;
// REWRITES-NEXT:         let _v79: bool = ((unsafe { main_empty[(_v75 as usize)] }) as i32) == _v78;
// REWRITES-NEXT:     _v79
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v80: bool = false;
// REWRITES-NEXT:     _v80
// REWRITES-NEXT: };
// REWRITES-NEXT: empty_ok = _v74 as i32;
// REWRITES-NEXT: let _v82: u64 = 4;
// REWRITES-NEXT: let _v83: u64 = 4;
// REWRITES-NEXT: let _v85: bool = if _v82 == _v83 {
// REWRITES-NEXT:         let _v86: i64 = 0;
// REWRITES-NEXT:         let _v89: i32 = 67;
// REWRITES-NEXT:         let _v90: bool = ((unsafe { main_data[(_v86 as usize)] }) as i32) == _v89;
// REWRITES-NEXT:     _v90
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v91: bool = false;
// REWRITES-NEXT:     _v91
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v92: bool = if _v85 {
// REWRITES-NEXT:         let _v93: i64 = 1;
// REWRITES-NEXT:         let _v96: i32 = 50;
// REWRITES-NEXT:         let _v97: bool = ((unsafe { main_data[(_v93 as usize)] }) as i32) == _v96;
// REWRITES-NEXT:     _v97
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v98: bool = false;
// REWRITES-NEXT:     _v98
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v99: bool = if _v92 {
// REWRITES-NEXT:         let _v100: i64 = 2;
// REWRITES-NEXT:         let _v103: i32 = 51;
// REWRITES-NEXT:         let _v104: bool = ((unsafe { main_data[(_v100 as usize)] }) as i32) == _v103;
// REWRITES-NEXT:     _v104
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v105: bool = false;
// REWRITES-NEXT:     _v105
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v106: bool = if _v99 {
// REWRITES-NEXT:         let _v107: i64 = 3;
// REWRITES-NEXT:         let _v110: i32 = 10;
// REWRITES-NEXT:         let _v111: bool = ((unsafe { main_data[(_v107 as usize)] }) as i32) == _v110;
// REWRITES-NEXT:     _v111
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v112: bool = false;
// REWRITES-NEXT:     _v112
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v113: bool = if _v106 {
// REWRITES-NEXT:         let _v115: bool = framed_ok != 0;
// REWRITES-NEXT:     _v115
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v116: bool = false;
// REWRITES-NEXT:     _v116
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v117: bool = if _v113 {
// REWRITES-NEXT:         let _v119: bool = empty_ok != 0;
// REWRITES-NEXT:     _v119
// REWRITES-NEXT: } else {
// REWRITES-NEXT:         let _v120: bool = false;
// REWRITES-NEXT:     _v120
// REWRITES-NEXT: };
// REWRITES-NEXT: let _v121: i32 = 0;
// REWRITES-NEXT: let _v122: i32 = 1;
// REWRITES-NEXT: __retval = if _v117 { _v121 } else { _v122 };
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
