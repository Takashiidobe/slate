#include <stdio.h>

int main(void) {
  int v;

  v = 5;
  printf("%d\n", __sync_fetch_and_add(&v, 3));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_fetch_and_sub(&v, 3));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_fetch_and_or(&v, 0xF0));
  printf("%d\n", v);

  v = 0xFF;
  printf("%d\n", __sync_fetch_and_and(&v, 0x0F));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_fetch_and_xor(&v, 0xFF));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_fetch_and_nand(&v, 3));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_sub_and_fetch(&v, 3));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_or_and_fetch(&v, 0xF0));
  printf("%d\n", v);

  v = 0xFF;
  printf("%d\n", __sync_and_and_fetch(&v, 0x0F));
  printf("%d\n", v);

  v = 0x0F;
  printf("%d\n", __sync_xor_and_fetch(&v, 0xFF));
  printf("%d\n", v);

  v = 5;
  printf("%d\n", __sync_nand_and_fetch(&v, 3));
  printf("%d\n", v);

  __sync_synchronize();
  printf("ok\n");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut v: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: i32 = 5;
// LOWERING-NEXT:     v = _v1;
// LOWERING-NEXT:     let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: i32 = 3;
// LOWERING-NEXT:     let _v4: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_add(_v3, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v5: i32 = unsafe { printf(_v2 as *const i8, _v4) };
// LOWERING-NEXT:     let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v7: i32 = v;
// LOWERING-NEXT:     let _v8: i32 = unsafe { printf(_v6 as *const i8, _v7) };
// LOWERING-NEXT:     let _v9: i32 = 5;
// LOWERING-NEXT:     v = _v9;
// LOWERING-NEXT:     let _v10: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: i32 = 3;
// LOWERING-NEXT:     let _v12: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_sub(_v11, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v13: i32 = unsafe { printf(_v10 as *const i8, _v12) };
// LOWERING-NEXT:     let _v14: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v15: i32 = v;
// LOWERING-NEXT:     let _v16: i32 = unsafe { printf(_v14 as *const i8, _v15) };
// LOWERING-NEXT:     let _v17: i32 = 15;
// LOWERING-NEXT:     v = _v17;
// LOWERING-NEXT:     let _v18: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v19: i32 = 240;
// LOWERING-NEXT:     let _v20: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_or(_v19, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v21: i32 = unsafe { printf(_v18 as *const i8, _v20) };
// LOWERING-NEXT:     let _v22: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v23: i32 = v;
// LOWERING-NEXT:     let _v24: i32 = unsafe { printf(_v22 as *const i8, _v23) };
// LOWERING-NEXT:     let _v25: i32 = 255;
// LOWERING-NEXT:     v = _v25;
// LOWERING-NEXT:     let _v26: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v27: i32 = 15;
// LOWERING-NEXT:     let _v28: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_and(_v27, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v29: i32 = unsafe { printf(_v26 as *const i8, _v28) };
// LOWERING-NEXT:     let _v30: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v31: i32 = v;
// LOWERING-NEXT:     let _v32: i32 = unsafe { printf(_v30 as *const i8, _v31) };
// LOWERING-NEXT:     let _v33: i32 = 15;
// LOWERING-NEXT:     v = _v33;
// LOWERING-NEXT:     let _v34: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v35: i32 = 255;
// LOWERING-NEXT:     let _v36: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_xor(_v35, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v37: i32 = unsafe { printf(_v34 as *const i8, _v36) };
// LOWERING-NEXT:     let _v38: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v39: i32 = v;
// LOWERING-NEXT:     let _v40: i32 = unsafe { printf(_v38 as *const i8, _v39) };
// LOWERING-NEXT:     let _v41: i32 = 5;
// LOWERING-NEXT:     v = _v41;
// LOWERING-NEXT:     let _v42: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v43: i32 = 3;
// LOWERING-NEXT:     let _v44: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_nand(_v43, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v45: i32 = unsafe { printf(_v42 as *const i8, _v44) };
// LOWERING-NEXT:     let _v46: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v47: i32 = v;
// LOWERING-NEXT:     let _v48: i32 = unsafe { printf(_v46 as *const i8, _v47) };
// LOWERING-NEXT:     let _v49: i32 = 5;
// LOWERING-NEXT:     v = _v49;
// LOWERING-NEXT:     let _v50: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v51: i32 = 3;
// LOWERING-NEXT:     let _v52: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_sub(_v51, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v53: i32 = _v52 - _v51;
// LOWERING-NEXT:     let _v54: i32 = unsafe { printf(_v50 as *const i8, _v53) };
// LOWERING-NEXT:     let _v55: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v56: i32 = v;
// LOWERING-NEXT:     let _v57: i32 = unsafe { printf(_v55 as *const i8, _v56) };
// LOWERING-NEXT:     let _v58: i32 = 15;
// LOWERING-NEXT:     v = _v58;
// LOWERING-NEXT:     let _v59: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v60: i32 = 240;
// LOWERING-NEXT:     let _v61: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_or(_v60, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v62: i32 = _v61 | _v60;
// LOWERING-NEXT:     let _v63: i32 = unsafe { printf(_v59 as *const i8, _v62) };
// LOWERING-NEXT:     let _v64: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v65: i32 = v;
// LOWERING-NEXT:     let _v66: i32 = unsafe { printf(_v64 as *const i8, _v65) };
// LOWERING-NEXT:     let _v67: i32 = 255;
// LOWERING-NEXT:     v = _v67;
// LOWERING-NEXT:     let _v68: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v69: i32 = 15;
// LOWERING-NEXT:     let _v70: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_and(_v69, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v71: i32 = _v70 & _v69;
// LOWERING-NEXT:     let _v72: i32 = unsafe { printf(_v68 as *const i8, _v71) };
// LOWERING-NEXT:     let _v73: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v74: i32 = v;
// LOWERING-NEXT:     let _v75: i32 = unsafe { printf(_v73 as *const i8, _v74) };
// LOWERING-NEXT:     let _v76: i32 = 15;
// LOWERING-NEXT:     v = _v76;
// LOWERING-NEXT:     let _v77: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v78: i32 = 255;
// LOWERING-NEXT:     let _v79: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_xor(_v78, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v80: i32 = _v79 ^ _v78;
// LOWERING-NEXT:     let _v81: i32 = unsafe { printf(_v77 as *const i8, _v80) };
// LOWERING-NEXT:     let _v82: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v83: i32 = v;
// LOWERING-NEXT:     let _v84: i32 = unsafe { printf(_v82 as *const i8, _v83) };
// LOWERING-NEXT:     let _v85: i32 = 5;
// LOWERING-NEXT:     v = _v85;
// LOWERING-NEXT:     let _v86: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v87: i32 = 3;
// LOWERING-NEXT:     let _v88: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_nand(_v87, std::sync::atomic::Ordering::SeqCst) };
// LOWERING-NEXT:     let _v89: i32 = _v88 & _v87;
// LOWERING-NEXT:     let _v90: i32 = !_v89;
// LOWERING-NEXT:     let _v91: i32 = unsafe { printf(_v86 as *const i8, _v90) };
// LOWERING-NEXT:     let _v92: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v93: i32 = v;
// LOWERING-NEXT:     let _v94: i32 = unsafe { printf(_v92 as *const i8, _v93) };
// LOWERING-NEXT:     std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// LOWERING-NEXT:     let _v95: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v96: i32 = unsafe { printf(_v95 as *const i8) };
// LOWERING-NEXT:     let _v97: i32 = 0;
// LOWERING-NEXT:     __retval = _v97;
// LOWERING-NEXT:     let _v98: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v98 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut v: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: v = 5;
// REWRITES-NEXT: let _v2: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: i32 = 3;
// REWRITES-NEXT: let _v4: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_add(_v3, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v5: i32 = unsafe { printf(_v2 as *const i8, _v4) };
// REWRITES-NEXT: let _v6: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v8: i32 = unsafe { printf(_v6 as *const i8, v) };
// REWRITES-NEXT: v = 5;
// REWRITES-NEXT: let _v10: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: i32 = 3;
// REWRITES-NEXT: let _v12: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_sub(_v11, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v13: i32 = unsafe { printf(_v10 as *const i8, _v12) };
// REWRITES-NEXT: let _v14: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: i32 = unsafe { printf(_v14 as *const i8, v) };
// REWRITES-NEXT: v = 15;
// REWRITES-NEXT: let _v18: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v19: i32 = 240;
// REWRITES-NEXT: let _v20: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_or(_v19, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v21: i32 = unsafe { printf(_v18 as *const i8, _v20) };
// REWRITES-NEXT: let _v22: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v24: i32 = unsafe { printf(_v22 as *const i8, v) };
// REWRITES-NEXT: v = 255;
// REWRITES-NEXT: let _v26: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v27: i32 = 15;
// REWRITES-NEXT: let _v28: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_and(_v27, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v29: i32 = unsafe { printf(_v26 as *const i8, _v28) };
// REWRITES-NEXT: let _v30: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v32: i32 = unsafe { printf(_v30 as *const i8, v) };
// REWRITES-NEXT: v = 15;
// REWRITES-NEXT: let _v34: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v35: i32 = 255;
// REWRITES-NEXT: let _v36: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_xor(_v35, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v37: i32 = unsafe { printf(_v34 as *const i8, _v36) };
// REWRITES-NEXT: let _v38: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v40: i32 = unsafe { printf(_v38 as *const i8, v) };
// REWRITES-NEXT: v = 5;
// REWRITES-NEXT: let _v42: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v43: i32 = 3;
// REWRITES-NEXT: let _v44: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_nand(_v43, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v45: i32 = unsafe { printf(_v42 as *const i8, _v44) };
// REWRITES-NEXT: let _v46: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v48: i32 = unsafe { printf(_v46 as *const i8, v) };
// REWRITES-NEXT: v = 5;
// REWRITES-NEXT: let _v50: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v51: i32 = 3;
// REWRITES-NEXT: let _v52: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_sub(_v51, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v54: i32 = unsafe { printf(_v50 as *const i8, _v52 - _v51) };
// REWRITES-NEXT: let _v55: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v57: i32 = unsafe { printf(_v55 as *const i8, v) };
// REWRITES-NEXT: v = 15;
// REWRITES-NEXT: let _v59: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v60: i32 = 240;
// REWRITES-NEXT: let _v61: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_or(_v60, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v63: i32 = unsafe { printf(_v59 as *const i8, _v61 | _v60) };
// REWRITES-NEXT: let _v64: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v66: i32 = unsafe { printf(_v64 as *const i8, v) };
// REWRITES-NEXT: v = 255;
// REWRITES-NEXT: let _v68: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v69: i32 = 15;
// REWRITES-NEXT: let _v70: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_and(_v69, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v72: i32 = unsafe { printf(_v68 as *const i8, _v70 & _v69) };
// REWRITES-NEXT: let _v73: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v75: i32 = unsafe { printf(_v73 as *const i8, v) };
// REWRITES-NEXT: v = 15;
// REWRITES-NEXT: let _v77: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v78: i32 = 255;
// REWRITES-NEXT: let _v79: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_xor(_v78, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v81: i32 = unsafe { printf(_v77 as *const i8, _v79 ^ _v78) };
// REWRITES-NEXT: let _v82: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v84: i32 = unsafe { printf(_v82 as *const i8, v) };
// REWRITES-NEXT: v = 5;
// REWRITES-NEXT: let _v86: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v87: i32 = 3;
// REWRITES-NEXT: let _v88: i32 = unsafe { std::sync::atomic::AtomicI32::from_ptr(std::ptr::addr_of_mut!(v)).fetch_nand(_v87, std::sync::atomic::Ordering::SeqCst) };
// REWRITES-NEXT: let _v90: i32 = !(_v88 & _v87);
// REWRITES-NEXT: let _v91: i32 = unsafe { printf(_v86 as *const i8, _v90) };
// REWRITES-NEXT: let _v92: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v94: i32 = unsafe { printf(_v92 as *const i8, v) };
// REWRITES-NEXT: std::sync::atomic::fence(std::sync::atomic::Ordering::SeqCst);
// REWRITES-NEXT: let _v95: *mut i8 = b"ok\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v96: i32 = unsafe { printf(_v95 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
