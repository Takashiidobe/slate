void abort(void);
void exit(int);

typedef enum {
  END   = -1,
  EMPTY = (1 << 8),
  BACKREF,
  BEGLINE,
  ENDLINE,
  BEGWORD,
  ENDWORD,
  LIMWORD,
  NOTLIMWORD,
  QMARK,
  STAR,
  PLUS,
  REPMN,
  CAT,
  OR,
  ORTOP,
  LPAREN,
  RPAREN,
  CSET
} token;

static token tok;

static int atom() {
  if ((tok >= 0 && tok < (1 << 8)) || tok >= CSET || tok == BACKREF ||
      tok == BEGLINE || tok == ENDLINE || tok == BEGWORD || tok == ENDWORD ||
      tok == LIMWORD || tok == NOTLIMWORD)
    return 1;
  else
    return 0;
}

int main(void) {
  tok = 0;
  if (atom() != 1)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-NEXT: static mut tok: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn atom() -> i32 {
// CHECK-NEXT:     if ((unsafe { tok }) as i32) >= (0 as i32)
// CHECK-NEXT:         && ((unsafe { tok }) as i32) < (1 as i32) << (8 as i32)
// CHECK-NEXT:         || ((unsafe { tok }) as i32) >= (273 as i32)
// CHECK-NEXT:         || ((unsafe { tok }) as i32) == (257 as i32)
// CHECK-NEXT:         || ((unsafe { tok }) as i32) == (258 as i32)
// CHECK-NEXT:         || ((unsafe { tok }) as i32) == (259 as i32)
// CHECK-NEXT:         || ((unsafe { tok }) as i32) == (260 as i32)
// CHECK-NEXT:         || ((unsafe { tok }) as i32) == (261 as i32)
// CHECK-NEXT:         || ((unsafe { tok }) as i32) == (262 as i32)
// CHECK-NEXT:         || ((unsafe { tok }) as i32) == (263 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         tok = 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if atom() != (1 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
