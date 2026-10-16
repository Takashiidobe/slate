/* Copyright (C) 2000 Free Software Foundation, Inc.
   Contributed by Nathan Sidwell 23 Feb 2000 <nathan@codesourcery.com> */

/* __alignof__ should never return a non-power of 2
   eg, sizeof(long double) might be 12, but that means it must be alignable
   on a 4 byte boundary. */

void abort(void);

void check(char const *type, int align) {
  if ((align & -align) != align) {
    abort();
  }
}

#define QUOTE_(s) #s
#define QUOTE(s)  QUOTE_(s)

#define check(t) check(QUOTE(t), __alignof__(t))

// This struct should have an alignment of the lcm of all the types. If one of
// the base alignments is not a power of two, then A cannot be power of two
// aligned.
struct A {
  char               c;
  signed short       ss;
  unsigned short     us;
  signed int         si;
  unsigned int       ui;
  signed long        sl;
  unsigned long      ul;
  signed long long   sll;
  unsigned long long ull;
  float              f;
  double             d;
  long double        ld;
  void              *dp;
  void               (*fp)();
};

int main() {
  check(void);
  check(char);
  check(signed short);
  check(unsigned short);
  check(signed int);
  check(unsigned int);
  check(signed long);
  check(unsigned long);
  check(signed long long);
  check(unsigned long long);
  check(float);
  check(double);
  check(long double);
  check(void *);
  check(void (*)());
  check(struct A);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Copyright (C) 2000 Free Software Foundation, Inc.
// CHECK-NEXT: //! Contributed by Nathan Sidwell 23 Feb 2000 <nathan@codesourcery.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // __alignof__ should never return a non-power of 2
// CHECK-NEXT: // eg, sizeof(long double) might be 12, but that means it must be alignable
// CHECK-NEXT: // on a 4 byte boundary.
// CHECK-NEXT: fn check(mut r#type: *const i8, mut align: i32) {
// CHECK-NEXT:     if align & -align != align {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // This struct should have an alignment of the lcm of all the types. If one of
// CHECK-NEXT: //
// CHECK-NEXT: // the base alignments is not a power of two, then A cannot be power of two
// CHECK-NEXT: //
// CHECK-NEXT: // aligned.
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     check(c"void".as_ptr() as *const i8, ((1 as u64) as u32) as i32);
// CHECK-NEXT:     check(c"char".as_ptr() as *const i8, ((1 as u64) as u32) as i32);
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"signed short".as_ptr() as *const i8,
// CHECK-NEXT:         ((2 as u64) as u32) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"unsigned short".as_ptr() as *const i8,
// CHECK-NEXT:         ((2 as u64) as u32) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"signed int".as_ptr() as *const i8,
// CHECK-NEXT:         ((4 as u64) as u32) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"unsigned int".as_ptr() as *const i8,
// CHECK-NEXT:         ((4 as u64) as u32) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"signed long".as_ptr() as *const i8,
// CHECK-NEXT:         ((8 as u64) as u32) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"unsigned long".as_ptr() as *const i8,
// CHECK-NEXT:         ((8 as u64) as u32) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"signed long long".as_ptr() as *const i8,
// CHECK-NEXT:         ((8 as u64) as u32) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"unsigned long long".as_ptr() as *const i8,
// CHECK-NEXT:         ((8 as u64) as u32) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     check(c"float".as_ptr() as *const i8, ((4 as u64) as u32) as i32);
// CHECK-NEXT:     check(c"double".as_ptr() as *const i8, ((8 as u64) as u32) as i32);
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"long double".as_ptr() as *const i8,
// CHECK-NEXT:         ((16 as u64) as u32) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     check(c"void *".as_ptr() as *const i8, ((8 as u64) as u32) as i32);
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"void (*)()".as_ptr() as *const i8,
// CHECK-NEXT:         ((8 as u64) as u32) as i32,
// CHECK-NEXT:     );
// CHECK-NEXT:     check(
// CHECK-NEXT:         c"struct A".as_ptr() as *const i8,
// CHECK-NEXT:         ((16 as u64) as u32) as i32,
// CHECK-NEXT:     );
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
