struct PMC {
  unsigned flags;
};

typedef struct Pcc_cell {
  struct PMC *p;
  long        bla;
  long        type;
} Pcc_cell;

int gi;
int cond;

extern void abort();
extern void never_ever(int interp, struct PMC *pmc)
    __attribute__((noinline, noclone));

void never_ever(int interp, struct PMC *pmc) { abort(); }

static void mark_cell(int *interp, Pcc_cell *c) __attribute__((__nonnull__(1)));

static void mark_cell(int *interp, Pcc_cell *c) {
  if (!cond)
    return;

  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 18)))
    never_ever(gi + 1, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 17)))
    never_ever(gi + 2, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 16)))
    never_ever(gi + 3, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 15)))
    never_ever(gi + 4, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 14)))
    never_ever(gi + 5, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 13)))
    never_ever(gi + 6, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 12)))
    never_ever(gi + 7, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 11)))
    never_ever(gi + 8, c->p);
  if (c && c->type == 4 && c->p && !(c->p->flags & (1 << 10)))
    never_ever(gi + 9, c->p);
}

static void foo(int *interp, Pcc_cell *c) { mark_cell(interp, c); }

static struct Pcc_cell *__attribute__((noinline, noclone)) getnull(void) {
  return (struct Pcc_cell *)0;
}

int main() {
  int i;

  cond = 1;
  for (i = 0; i < 100; i++)
    foo(&gi, getnull());
  return 0;
}

void bar_1(int *interp, Pcc_cell *c) {
  c->bla += 1;
  mark_cell(interp, c);
}

void bar_2(int *interp, Pcc_cell *c) {
  c->bla += 2;
  mark_cell(interp, c);
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
// CHECK-NEXT: static mut gi: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: static mut cond: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn never_ever(mut interp: i32, mut pmc: *mut PMC) {
// CHECK-NEXT:     unsafe { abort() };
// CHECK-NEXT: }
// CHECK-NEXT: fn mark_cell(mut interp: *mut i32, mut c: *mut Pcc_cell) {
// CHECK-NEXT:     if !((unsafe { cond }) != (0 as i32)) {
// CHECK-NEXT:         return;
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != std::ptr::null_mut::<Pcc_cell>()
// CHECK-NEXT:         && (unsafe { (*c).r#type }) == ((4 as i32) as i64)
// CHECK-NEXT:         && (unsafe { (*c).p }) != std::ptr::null_mut::<PMC>()
// CHECK-NEXT:         && !((unsafe { (*unsafe { (*c).p }).flags })
// CHECK-NEXT:             & (((1 as i32) << (18 as i32)) as u32) != (0 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         never_ever((unsafe { gi }) + (1 as i32), unsafe { (*c).p });
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != std::ptr::null_mut::<Pcc_cell>()
// CHECK-NEXT:         && (unsafe { (*c).r#type }) == ((4 as i32) as i64)
// CHECK-NEXT:         && (unsafe { (*c).p }) != std::ptr::null_mut::<PMC>()
// CHECK-NEXT:         && !((unsafe { (*unsafe { (*c).p }).flags })
// CHECK-NEXT:             & (((1 as i32) << (17 as i32)) as u32) != (0 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         never_ever((unsafe { gi }) + (2 as i32), unsafe { (*c).p });
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != std::ptr::null_mut::<Pcc_cell>()
// CHECK-NEXT:         && (unsafe { (*c).r#type }) == ((4 as i32) as i64)
// CHECK-NEXT:         && (unsafe { (*c).p }) != std::ptr::null_mut::<PMC>()
// CHECK-NEXT:         && !((unsafe { (*unsafe { (*c).p }).flags })
// CHECK-NEXT:             & (((1 as i32) << (16 as i32)) as u32) != (0 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         never_ever((unsafe { gi }) + (3 as i32), unsafe { (*c).p });
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != std::ptr::null_mut::<Pcc_cell>()
// CHECK-NEXT:         && (unsafe { (*c).r#type }) == ((4 as i32) as i64)
// CHECK-NEXT:         && (unsafe { (*c).p }) != std::ptr::null_mut::<PMC>()
// CHECK-NEXT:         && !((unsafe { (*unsafe { (*c).p }).flags })
// CHECK-NEXT:             & (((1 as i32) << (15 as i32)) as u32) != (0 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         never_ever((unsafe { gi }) + (4 as i32), unsafe { (*c).p });
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != std::ptr::null_mut::<Pcc_cell>()
// CHECK-NEXT:         && (unsafe { (*c).r#type }) == ((4 as i32) as i64)
// CHECK-NEXT:         && (unsafe { (*c).p }) != std::ptr::null_mut::<PMC>()
// CHECK-NEXT:         && !((unsafe { (*unsafe { (*c).p }).flags })
// CHECK-NEXT:             & (((1 as i32) << (14 as i32)) as u32) != (0 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         never_ever((unsafe { gi }) + (5 as i32), unsafe { (*c).p });
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != std::ptr::null_mut::<Pcc_cell>()
// CHECK-NEXT:         && (unsafe { (*c).r#type }) == ((4 as i32) as i64)
// CHECK-NEXT:         && (unsafe { (*c).p }) != std::ptr::null_mut::<PMC>()
// CHECK-NEXT:         && !((unsafe { (*unsafe { (*c).p }).flags })
// CHECK-NEXT:             & (((1 as i32) << (13 as i32)) as u32) != (0 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         never_ever((unsafe { gi }) + (6 as i32), unsafe { (*c).p });
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != std::ptr::null_mut::<Pcc_cell>()
// CHECK-NEXT:         && (unsafe { (*c).r#type }) == ((4 as i32) as i64)
// CHECK-NEXT:         && (unsafe { (*c).p }) != std::ptr::null_mut::<PMC>()
// CHECK-NEXT:         && !((unsafe { (*unsafe { (*c).p }).flags })
// CHECK-NEXT:             & (((1 as i32) << (12 as i32)) as u32) != (0 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         never_ever((unsafe { gi }) + (7 as i32), unsafe { (*c).p });
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != std::ptr::null_mut::<Pcc_cell>()
// CHECK-NEXT:         && (unsafe { (*c).r#type }) == ((4 as i32) as i64)
// CHECK-NEXT:         && (unsafe { (*c).p }) != std::ptr::null_mut::<PMC>()
// CHECK-NEXT:         && !((unsafe { (*unsafe { (*c).p }).flags })
// CHECK-NEXT:             & (((1 as i32) << (11 as i32)) as u32) != (0 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         never_ever((unsafe { gi }) + (8 as i32), unsafe { (*c).p });
// CHECK-NEXT:     }
// CHECK-NEXT:     if c != std::ptr::null_mut::<Pcc_cell>()
// CHECK-NEXT:         && (unsafe { (*c).r#type }) == ((4 as i32) as i64)
// CHECK-NEXT:         && (unsafe { (*c).p }) != std::ptr::null_mut::<PMC>()
// CHECK-NEXT:         && !((unsafe { (*unsafe { (*c).p }).flags })
// CHECK-NEXT:             & (((1 as i32) << (10 as i32)) as u32) != (0 as u32))
// CHECK-NEXT:     {
// CHECK-NEXT:         never_ever((unsafe { gi }) + (9 as i32), unsafe { (*c).p });
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(mut interp: *mut i32, mut c: *mut Pcc_cell) {
// CHECK-NEXT:     mark_cell(interp, c);
// CHECK-NEXT: }
// CHECK-NEXT: fn getnull() -> *mut Pcc_cell {
// CHECK-NEXT:     std::ptr::null_mut::<Pcc_cell>()
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         cond = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (100 as i32) {
// CHECK-NEXT:         foo(unsafe { std::ptr::addr_of_mut!(gi) }, getnull());
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn bar_1(mut interp: &i32, mut c: *mut Pcc_cell) {
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i64 = unsafe { (*c).bla };
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i64 = [[V_2]] + ((1 as i32) as i64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*c).bla = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     mark_cell((interp as *const i32) as *mut i32, c);
// CHECK-NEXT: }
// CHECK-NEXT: fn bar_2(mut interp: &i32, mut c: *mut Pcc_cell) {
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: i64 = unsafe { (*c).bla };
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: i64 = [[V_4]] + ((2 as i32) as i64);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*c).bla = [[V_5]];
// CHECK-NEXT:     }
// CHECK-NEXT:     mark_cell((interp as *const i32) as *mut i32, c);
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
