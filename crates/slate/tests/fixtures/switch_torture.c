#include <stdio.h>

int f(int x) {
  int out = 0;
  // @lowering-begin
  // @rewrite-begin
  switch (x) {
  case -500 ... - 1:
    out += 100;
  case 0:
  case 1:
    out += 1;
  case 2 ... 100:
    out += 2;
    if (out > 50) {
      break;
    }
    out += 3;
  case 101:
    out += 4;
    if (x % 2 == 0) {
      break;
    }
  default:
    out += 5;
  case 200 ... 500:
  case 600:
  case 700 ... 900:
    out += 6;
    break;
  case 999:
    out += 7;
  case 1000:
    out += 8;
  }
  // @rewrite-end
  // @lowering-end
  return out;
}

int main(void) {
  for (int x = -600; x < 1001; x++)
    printf("%d,", f(x));
  printf("\n");
  return 0;
}

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING-DAG: {
// COMMON-LOWERING-DAG:     let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:     {
// COMMON-LOWERING-DAG:         let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:         let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-DAG:             -500..=-1 => 0,
// COMMON-LOWERING-DAG:             0 => 1,
// COMMON-LOWERING-DAG:             1 => 2,
// COMMON-LOWERING-DAG:             2..=100 => 3,
// COMMON-LOWERING-DAG:             101 => 4,
// COMMON-LOWERING-DAG:             200..=500 => 6,
// COMMON-LOWERING-DAG:             600 => 7,
// COMMON-LOWERING-DAG:             700..=900 => 8,
// COMMON-LOWERING-DAG:             999 => 9,
// COMMON-LOWERING-DAG:             1000 => 10,
// COMMON-LOWERING-DAG:             _ => 5,
// COMMON-LOWERING-DAG:         };
// COMMON-LOWERING-DAG:         '__switch0: loop {
// COMMON-LOWERING-DAG:             match __switch_case0 {
// COMMON-LOWERING-DAG:                 0 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     __switch_case0 = 1;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 1 => {
// COMMON-LOWERING-DAG:                     __switch_case0 = 2;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 2 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     __switch_case0 = 3;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 3 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 50;
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                             break '__switch0;
// COMMON-LOWERING-DAG:                         }
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     __switch_case0 = 4;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 4 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     {
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-DAG:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                         if {{__v[0-9]+}} {
// COMMON-LOWERING-DAG:                             break '__switch0;
// COMMON-LOWERING-DAG:                         }
// COMMON-LOWERING-DAG:                     }
// COMMON-LOWERING-DAG:                     __switch_case0 = 5;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 5 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     __switch_case0 = 6;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 6 => {
// COMMON-LOWERING-DAG:                     __switch_case0 = 7;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 7 => {
// COMMON-LOWERING-DAG:                     __switch_case0 = 8;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 8 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     break '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 9 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     __switch_case0 = 10;
// COMMON-LOWERING-DAG:                     continue '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 10 => {
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-DAG:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     out = {{__v[0-9]+}};
// COMMON-LOWERING-DAG:                     break '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:                 _ => {
// COMMON-LOWERING-DAG:                     break '__switch0;
// COMMON-LOWERING-DAG:                 }
// COMMON-LOWERING-DAG:             }
// COMMON-LOWERING-DAG:         }
// COMMON-LOWERING-DAG:     }
// COMMON-LOWERING-DAG: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES-DAG: match x {
// COMMON-REWRITES-DAG:     -500..=-1 => {
// COMMON-REWRITES-DAG:         out += 100;
// COMMON-REWRITES-DAG:         let _v38: i32 = 1;
// COMMON-REWRITES-DAG:         let _v39: i32 = out;
// COMMON-REWRITES-DAG:         let _v40: i32 = _v39 + _v38;
// COMMON-REWRITES-DAG:         out = _v40;
// COMMON-REWRITES-DAG:         let _v41: i32 = 2;
// COMMON-REWRITES-DAG:         let _v42: i32 = out;
// COMMON-REWRITES-DAG:         let _v43: i32 = _v42 + _v41;
// COMMON-REWRITES-DAG:         out = _v43;
// COMMON-REWRITES-DAG:         let _v44: i32 = out;
// COMMON-REWRITES-DAG:         let _v45: i32 = 50;
// COMMON-REWRITES-DAG:         let _v46: bool = _v44 > _v45;
// COMMON-REWRITES-DAG:         if _v46 {
// COMMON-REWRITES-DAG:         } else {
// COMMON-REWRITES-DAG:             let _v47: i32 = 3;
// COMMON-REWRITES-DAG:             let _v48: i32 = out;
// COMMON-REWRITES-DAG:             let _v49: i32 = _v48 + _v47;
// COMMON-REWRITES-DAG:             out = _v49;
// COMMON-REWRITES-DAG:             let _v50: i32 = 4;
// COMMON-REWRITES-DAG:             let _v51: i32 = out;
// COMMON-REWRITES-DAG:             let _v52: i32 = _v51 + _v50;
// COMMON-REWRITES-DAG:             out = _v52;
// COMMON-REWRITES-DAG:             let _v53: i32 = x;
// COMMON-REWRITES-DAG:             let _v54: i32 = 2;
// COMMON-REWRITES-DAG:             let _v55: i32 = _v53 % _v54;
// COMMON-REWRITES-DAG:             let _v56: i32 = 0;
// COMMON-REWRITES-DAG:             let _v57: bool = _v55 == _v56;
// COMMON-REWRITES-DAG:             if _v57 {
// COMMON-REWRITES-DAG:             } else {
// COMMON-REWRITES-DAG:                 let _v58: i32 = 5;
// COMMON-REWRITES-DAG:                 let _v59: i32 = out;
// COMMON-REWRITES-DAG:                 let _v60: i32 = _v59 + _v58;
// COMMON-REWRITES-DAG:                 out = _v60;
// COMMON-REWRITES-DAG:                 let _v61: i32 = 6;
// COMMON-REWRITES-DAG:                 let _v62: i32 = out;
// COMMON-REWRITES-DAG:                 let _v63: i32 = _v62 + _v61;
// COMMON-REWRITES-DAG:                 out = _v63;
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     0 | 1 => {
// COMMON-REWRITES-DAG:         out += 1;
// COMMON-REWRITES-DAG:         let _v64: i32 = 2;
// COMMON-REWRITES-DAG:         let _v65: i32 = out;
// COMMON-REWRITES-DAG:         let _v66: i32 = _v65 + _v64;
// COMMON-REWRITES-DAG:         out = _v66;
// COMMON-REWRITES-DAG:         let _v67: i32 = out;
// COMMON-REWRITES-DAG:         let _v68: i32 = 50;
// COMMON-REWRITES-DAG:         let _v69: bool = _v67 > _v68;
// COMMON-REWRITES-DAG:         if _v69 {
// COMMON-REWRITES-DAG:         } else {
// COMMON-REWRITES-DAG:             let _v70: i32 = 3;
// COMMON-REWRITES-DAG:             let _v71: i32 = out;
// COMMON-REWRITES-DAG:             let _v72: i32 = _v71 + _v70;
// COMMON-REWRITES-DAG:             out = _v72;
// COMMON-REWRITES-DAG:             let _v73: i32 = 4;
// COMMON-REWRITES-DAG:             let _v74: i32 = out;
// COMMON-REWRITES-DAG:             let _v75: i32 = _v74 + _v73;
// COMMON-REWRITES-DAG:             out = _v75;
// COMMON-REWRITES-DAG:             let _v76: i32 = x;
// COMMON-REWRITES-DAG:             let _v77: i32 = 2;
// COMMON-REWRITES-DAG:             let _v78: i32 = _v76 % _v77;
// COMMON-REWRITES-DAG:             let _v79: i32 = 0;
// COMMON-REWRITES-DAG:             let _v80: bool = _v78 == _v79;
// COMMON-REWRITES-DAG:             if _v80 {
// COMMON-REWRITES-DAG:             } else {
// COMMON-REWRITES-DAG:                 let _v81: i32 = 5;
// COMMON-REWRITES-DAG:                 let _v82: i32 = out;
// COMMON-REWRITES-DAG:                 let _v83: i32 = _v82 + _v81;
// COMMON-REWRITES-DAG:                 out = _v83;
// COMMON-REWRITES-DAG:                 let _v84: i32 = 6;
// COMMON-REWRITES-DAG:                 let _v85: i32 = out;
// COMMON-REWRITES-DAG:                 let _v86: i32 = _v85 + _v84;
// COMMON-REWRITES-DAG:                 out = _v86;
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     2..=100 => {
// COMMON-REWRITES-DAG:         out += 2;
// COMMON-REWRITES-DAG:         if out > 50 {
// COMMON-REWRITES-DAG:         } else {
// COMMON-REWRITES-DAG:             out += 3;
// COMMON-REWRITES-DAG:             let _v87: i32 = 4;
// COMMON-REWRITES-DAG:             let _v88: i32 = out;
// COMMON-REWRITES-DAG:             let _v89: i32 = _v88 + _v87;
// COMMON-REWRITES-DAG:             out = _v89;
// COMMON-REWRITES-DAG:             let _v90: i32 = x;
// COMMON-REWRITES-DAG:             let _v91: i32 = 2;
// COMMON-REWRITES-DAG:             let _v92: i32 = _v90 % _v91;
// COMMON-REWRITES-DAG:             let _v93: i32 = 0;
// COMMON-REWRITES-DAG:             let _v94: bool = _v92 == _v93;
// COMMON-REWRITES-DAG:             if _v94 {
// COMMON-REWRITES-DAG:             } else {
// COMMON-REWRITES-DAG:                 let _v95: i32 = 5;
// COMMON-REWRITES-DAG:                 let _v96: i32 = out;
// COMMON-REWRITES-DAG:                 let _v97: i32 = _v96 + _v95;
// COMMON-REWRITES-DAG:                 out = _v97;
// COMMON-REWRITES-DAG:                 let _v98: i32 = 6;
// COMMON-REWRITES-DAG:                 let _v99: i32 = out;
// COMMON-REWRITES-DAG:                 let _v100: i32 = _v99 + _v98;
// COMMON-REWRITES-DAG:                 out = _v100;
// COMMON-REWRITES-DAG:             }
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     101 => {
// COMMON-REWRITES-DAG:         out += 4;
// COMMON-REWRITES-DAG:         if x % 2 == 0 {
// COMMON-REWRITES-DAG:         } else {
// COMMON-REWRITES-DAG:             let _v101: i32 = 5;
// COMMON-REWRITES-DAG:             let _v102: i32 = out;
// COMMON-REWRITES-DAG:             let _v103: i32 = _v102 + _v101;
// COMMON-REWRITES-DAG:             out = _v103;
// COMMON-REWRITES-DAG:             let _v104: i32 = 6;
// COMMON-REWRITES-DAG:             let _v105: i32 = out;
// COMMON-REWRITES-DAG:             let _v106: i32 = _v105 + _v104;
// COMMON-REWRITES-DAG:             out = _v106;
// COMMON-REWRITES-DAG:         }
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     200..=500 | 600 | 700..=900 => {
// COMMON-REWRITES-DAG:         out += 6;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     999 => {
// COMMON-REWRITES-DAG:         out += 7;
// COMMON-REWRITES-DAG:         let _v110: i32 = 8;
// COMMON-REWRITES-DAG:         let _v111: i32 = out;
// COMMON-REWRITES-DAG:         let _v112: i32 = _v111 + _v110;
// COMMON-REWRITES-DAG:         out = _v112;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     1000 => {
// COMMON-REWRITES-DAG:         out += 8;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG:     _ => {
// COMMON-REWRITES-DAG:         out += 5;
// COMMON-REWRITES-DAG:         let _v107: i32 = 6;
// COMMON-REWRITES-DAG:         let _v108: i32 = out;
// COMMON-REWRITES-DAG:         let _v109: i32 = _v108 + _v107;
// COMMON-REWRITES-DAG:         out = _v109;
// COMMON-REWRITES-DAG:     }
// COMMON-REWRITES-DAG: }
// SLATE-FILECHECK-END common-rewrites
