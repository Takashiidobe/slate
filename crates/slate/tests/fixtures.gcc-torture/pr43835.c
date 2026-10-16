struct PMC {
  unsigned flags;
};

typedef struct Pcc_cell {
  struct PMC *p;
  long        bla;
  long        type;
} Pcc_cell;

extern void abort();
extern void Parrot_gc_mark_PMC_alive_fun(int *interp, struct PMC *pmc)
    __attribute__((noinline));

void Parrot_gc_mark_PMC_alive_fun(int *interp, struct PMC *pmc) { abort(); }

static void mark_cell(int *interp, Pcc_cell *c) __attribute__((__nonnull__(1)))
__attribute__((__nonnull__(2))) __attribute__((noinline));

static void mark_cell(int *interp, Pcc_cell *c) {
  if (c->type == 4 && c->p && !(c->p->flags & (1 << 18)))
    Parrot_gc_mark_PMC_alive_fun(interp, c->p);
}

void foo(int *interp, Pcc_cell *c);

void foo(int *interp, Pcc_cell *c) { mark_cell(interp, c); }

int main() {
  int      i;
  Pcc_cell c;
  c.p    = 0;
  c.bla  = 42;
  c.type = 4;
  foo(&i, &c);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct PMC {
// CHECK-NEXT:     flags: u32,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct Pcc_cell {
// CHECK-NEXT:     p: *mut PMC,
// CHECK-NEXT:     bla: i64,
// CHECK-NEXT:     r#type: i64,
// CHECK-NEXT: }
// CHECK-NEXT: fn Parrot_gc_mark_PMC_alive_fun(mut interp: &i32, mut pmc: *mut PMC) {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-NEXT: fn mark_cell(mut interp: &i32, mut c: &Pcc_cell) {
// CHECK-NEXT:     if (unsafe { (*(c as *const Pcc_cell)).r#type }) == ((4 as i32) as i64)
// CHECK-NEXT:         && (unsafe { (*(c as *const Pcc_cell)).p }) != std::ptr::null_mut::<PMC>()
// CHECK-NEXT:         && !((unsafe { (*unsafe { (*(c as *const Pcc_cell)).p }).flags })
// CHECK-NEXT:             & (((1 as i32) << (18 as i32)) as u32) != (0 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         Parrot_gc_mark_PMC_alive_fun(interp, unsafe { (*(c as *const Pcc_cell)).p });
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut interp: &i32, mut c: &Pcc_cell) {
// CHECK-NEXT:     mark_cell(interp, c);
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     let mut c: Pcc_cell = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     c.p = std::ptr::null_mut::<PMC>();
// CHECK-NEXT:     c.bla = (42 as i32) as i64;
// CHECK-NEXT:     c.r#type = (4 as i32) as i64;
// CHECK-NEXT:     foo(
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(i)) },
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(c)) },
// CHECK-NEXT:     );
// CHECK-NEXT:     0 as i32
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
