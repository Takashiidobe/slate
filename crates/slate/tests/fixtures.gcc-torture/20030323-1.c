/* PR opt/10116 */
/* { dg-require-effective-target return_address } */
/* Removed tablejump while label still in use; this is really a link test.  */

void *NSReturnAddress(int offset) {
  switch (offset) {
  case 0:
    return __builtin_return_address(0 + 1);
  case 1:
    return __builtin_return_address(1 + 1);
  case 2:
    return __builtin_return_address(2 + 1);
  case 3:
    return __builtin_return_address(3 + 1);
  case 4:
    return __builtin_return_address(4 + 1);
  case 5:
    return __builtin_return_address(5 + 1);
  case 6:
    return __builtin_return_address(6 + 1);
  case 7:
    return __builtin_return_address(7 + 1);
  case 8:
    return __builtin_return_address(8 + 1);
  case 9:
    return __builtin_return_address(9 + 1);
  case 10:
    return __builtin_return_address(10 + 1);
  case 11:
    return __builtin_return_address(11 + 1);
  case 12:
    return __builtin_return_address(12 + 1);
  case 13:
    return __builtin_return_address(13 + 1);
  case 14:
    return __builtin_return_address(14 + 1);
  case 15:
    return __builtin_return_address(15 + 1);
  case 16:
    return __builtin_return_address(16 + 1);
  case 17:
    return __builtin_return_address(17 + 1);
  case 18:
    return __builtin_return_address(18 + 1);
  case 19:
    return __builtin_return_address(19 + 1);
  case 20:
    return __builtin_return_address(20 + 1);
  case 21:
    return __builtin_return_address(21 + 1);
  case 22:
    return __builtin_return_address(22 + 1);
  case 23:
    return __builtin_return_address(23 + 1);
  case 24:
    return __builtin_return_address(24 + 1);
  case 25:
    return __builtin_return_address(25 + 1);
  case 26:
    return __builtin_return_address(26 + 1);
  case 27:
    return __builtin_return_address(27 + 1);
  case 28:
    return __builtin_return_address(28 + 1);
  case 29:
    return __builtin_return_address(29 + 1);
  case 30:
    return __builtin_return_address(30 + 1);
  case 31:
    return __builtin_return_address(31 + 1);
  case 32:
    return __builtin_return_address(32 + 1);
  case 33:
    return __builtin_return_address(33 + 1);
  case 34:
    return __builtin_return_address(34 + 1);
  case 35:
    return __builtin_return_address(35 + 1);
  case 36:
    return __builtin_return_address(36 + 1);
  case 37:
    return __builtin_return_address(37 + 1);
  case 38:
    return __builtin_return_address(38 + 1);
  case 39:
    return __builtin_return_address(39 + 1);
  case 40:
    return __builtin_return_address(40 + 1);
  case 41:
    return __builtin_return_address(41 + 1);
  case 42:
    return __builtin_return_address(42 + 1);
  case 43:
    return __builtin_return_address(43 + 1);
  case 44:
    return __builtin_return_address(44 + 1);
  case 45:
    return __builtin_return_address(45 + 1);
  case 46:
    return __builtin_return_address(46 + 1);
  case 47:
    return __builtin_return_address(47 + 1);
  case 48:
    return __builtin_return_address(48 + 1);
  case 49:
    return __builtin_return_address(49 + 1);
  case 50:
    return __builtin_return_address(50 + 1);
  case 51:
    return __builtin_return_address(51 + 1);
  case 52:
    return __builtin_return_address(52 + 1);
  case 53:
    return __builtin_return_address(53 + 1);
  case 54:
    return __builtin_return_address(54 + 1);
  case 55:
    return __builtin_return_address(55 + 1);
  case 56:
    return __builtin_return_address(56 + 1);
  case 57:
    return __builtin_return_address(57 + 1);
  case 58:
    return __builtin_return_address(58 + 1);
  case 59:
    return __builtin_return_address(59 + 1);
  case 60:
    return __builtin_return_address(60 + 1);
  case 61:
    return __builtin_return_address(61 + 1);
  case 62:
    return __builtin_return_address(62 + 1);
  case 63:
    return __builtin_return_address(63 + 1);
  case 64:
    return __builtin_return_address(64 + 1);
  case 65:
    return __builtin_return_address(65 + 1);
  case 66:
    return __builtin_return_address(66 + 1);
  case 67:
    return __builtin_return_address(67 + 1);
  case 68:
    return __builtin_return_address(68 + 1);
  case 69:
    return __builtin_return_address(69 + 1);
  case 70:
    return __builtin_return_address(70 + 1);
  case 71:
    return __builtin_return_address(71 + 1);
  case 72:
    return __builtin_return_address(72 + 1);
  case 73:
    return __builtin_return_address(73 + 1);
  case 74:
    return __builtin_return_address(74 + 1);
  case 75:
    return __builtin_return_address(75 + 1);
  case 76:
    return __builtin_return_address(76 + 1);
  case 77:
    return __builtin_return_address(77 + 1);
  case 78:
    return __builtin_return_address(78 + 1);
  case 79:
    return __builtin_return_address(79 + 1);
  case 80:
    return __builtin_return_address(80 + 1);
  case 81:
    return __builtin_return_address(81 + 1);
  case 82:
    return __builtin_return_address(82 + 1);
  case 83:
    return __builtin_return_address(83 + 1);
  case 84:
    return __builtin_return_address(84 + 1);
  case 85:
    return __builtin_return_address(85 + 1);
  case 86:
    return __builtin_return_address(86 + 1);
  case 87:
    return __builtin_return_address(87 + 1);
  case 88:
    return __builtin_return_address(88 + 1);
  case 89:
    return __builtin_return_address(89 + 1);
  case 90:
    return __builtin_return_address(90 + 1);
  case 91:
    return __builtin_return_address(91 + 1);
  case 92:
    return __builtin_return_address(92 + 1);
  case 93:
    return __builtin_return_address(93 + 1);
  case 94:
    return __builtin_return_address(94 + 1);
  case 95:
    return __builtin_return_address(95 + 1);
  case 96:
    return __builtin_return_address(96 + 1);
  case 97:
    return __builtin_return_address(97 + 1);
  case 98:
    return __builtin_return_address(98 + 1);
  case 99:
    return __builtin_return_address(99 + 1);
  }
  return 0;
}

int main() { return 0; }

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR opt/10116
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target return_address }
// CHECK-NEXT: //!
// CHECK-NEXT: //! Removed tablejump while label still in use; this is really a link test.
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.returnaddress.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_ded315013ea6523([[ARG_0:arg[0-9]+]]: i32) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn NSReturnAddress(mut offset: i32) -> *mut () {
// CHECK-NEXT:     match offset {
// CHECK-NEXT:         0 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((0 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         1 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((1 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         2 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((2 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         3 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((3 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         4 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((4 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         5 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((5 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         6 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((6 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         7 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((7 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         8 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((8 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         9 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((9 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         10 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((10 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         11 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((11 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         12 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((12 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         13 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((13 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         14 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((14 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         15 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((15 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         16 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((16 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         17 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((17 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         18 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((18 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         19 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((19 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         20 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((20 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         21 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((21 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         22 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((22 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         23 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((23 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         24 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((24 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         25 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((25 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         26 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((26 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         27 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((27 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         28 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((28 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         29 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((29 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         30 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((30 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         31 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((31 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         32 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((32 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         33 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((33 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         34 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((34 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         35 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((35 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         36 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((36 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         37 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((37 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         38 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((38 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         39 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((39 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         40 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((40 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         41 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((41 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         42 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((42 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         43 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((43 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         44 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((44 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         45 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((45 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         46 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((46 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         47 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((47 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         48 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((48 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         49 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((49 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         50 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((50 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         51 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((51 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         52 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((52 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         53 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((53 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         54 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((54 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         55 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((55 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         56 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((56 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         57 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((57 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         58 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((58 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         59 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((59 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         60 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((60 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         61 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((61 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         62 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((62 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         63 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((63 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         64 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((64 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         65 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((65 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         66 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((66 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         67 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((67 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         68 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((68 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         69 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((69 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         70 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((70 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         71 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((71 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         72 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((72 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         73 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((73 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         74 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((74 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         75 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((75 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         76 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((76 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         77 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((77 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         78 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((78 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         79 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((79 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         80 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((80 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         81 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((81 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         82 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((82 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         83 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((83 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         84 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((84 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         85 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((85 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         86 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((86 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         87 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((87 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         88 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((88 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         89 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((89 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         90 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((90 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         91 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((91 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         92 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((92 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         93 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((93 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         94 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((94 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         95 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((95 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         96 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((96 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         97 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((97 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         98 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((98 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         99 => {
// CHECK-NEXT:             return unsafe {
// CHECK-NEXT:                 __slate_intrinsic_ded315013ea6523(
// CHECK-NEXT:                     const { (((99 as i32) + (1 as i32)) as u32) as i32 },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {}
// CHECK-NEXT:     }
// CHECK-NEXT:     std::ptr::null_mut::<()>()
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
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
