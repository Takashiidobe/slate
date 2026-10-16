extern void  abort(void);
extern void *malloc(__SIZE_TYPE__);
extern void *memset(void *, int, __SIZE_TYPE__);
typedef struct {
  short              a;
  unsigned short     b;
  unsigned short     c;
  unsigned long long Count;
  long long          Count2;
} __attribute__((packed)) Struct1;

typedef struct {
  short              a;
  unsigned short     b;
  unsigned short     c;
  unsigned long long d;
  long long          e;
  long long          f;
} __attribute__((packed)) Struct2;

typedef union {
  Struct1 a;
  Struct2 b;
} Union;

typedef struct {
  int   Count;
  Union List[0];
} __attribute__((packed)) Struct3;

unsigned long long Sum(Struct3 *instrs) __attribute__((noinline));
unsigned long long Sum(Struct3 *instrs) {
  unsigned long long count = 0;
  int                i;

  for (i = 0; i < instrs->Count; i++) {
    count += instrs->List[i].a.Count;
  }
  return count;
}
long long Sum2(Struct3 *instrs) __attribute__((noinline));
long long Sum2(Struct3 *instrs) {
  long long count = 0;
  int       i;

  for (i = 0; i < instrs->Count; i++) {
    count += instrs->List[i].a.Count2;
  }
  return count;
}
int main(void) {
  Struct3 *p = malloc(sizeof(int) + 3 * sizeof(Union));
  memset(p, 0, sizeof(int) + 3 * sizeof(Union));
  p->Count            = 3;
  p->List[0].a.Count  = 555;
  p->List[1].a.Count  = 999;
  p->List[2].a.Count  = 0x101010101ULL;
  p->List[0].a.Count2 = 555;
  p->List[1].a.Count2 = 999;
  p->List[2].a.Count2 = 0x101010101LL;
  if (Sum(p) != 555 + 999 + 0x101010101ULL)
    abort();
  if (Sum2(p) != 555 + 999 + 0x101010101LL)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn malloc([[V_0:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT:     fn memset([[V_1:__v[0-9]+]]: *mut (), [[V_2:__v[0-9]+]]: i32, [[V_3:__v[0-9]+]]: u64) -> *mut ();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: i16,
// CHECK-NEXT:     b: u16,
// CHECK-NEXT:     c: u16,
// CHECK-NEXT:     Count: u64,
// CHECK-NEXT:     Count2: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: i16,
// CHECK-NEXT:     b: u16,
// CHECK-NEXT:     c: u16,
// CHECK-NEXT:     d: u64,
// CHECK-NEXT:     e: i64,
// CHECK-NEXT:     f: i64,
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_2:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     a: [[SLATERECORD_0]],
// CHECK-NEXT:     b: [[SLATERECORD_1]],
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, packed)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_3:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     Count: i32,
// CHECK-NEXT:     List: [[[SLATERECORD_2]]; 0],
// CHECK-NEXT: }
// CHECK-NEXT: fn Sum(mut instrs: *mut [[SLATERECORD_3]]) -> u64 {
// CHECK-NEXT:     let mut count: u64 = ((0 as i32) as i64) as u64;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < unsafe { (*instrs).Count } {
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: u64 = count;
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: u64 = [[V_4]]
// CHECK-NEXT:             .wrapping_add(unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { (*instrs).List.as_mut_ptr() as *mut [[SLATERECORD_2]] }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .a
// CHECK-NEXT:                     .Count
// CHECK-NEXT:             });
// CHECK-NEXT:         count = [[V_5]];
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: i32 = [[V_6]] + (1 as i32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT:     count
// CHECK-NEXT: }
// CHECK-NEXT: fn Sum2(mut instrs: *mut [[SLATERECORD_3]]) -> i64 {
// CHECK-NEXT:     let mut count: i64 = (0 as i32) as i64;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < unsafe { (*instrs).Count } {
// CHECK-NEXT:         let mut [[V_8:__v[0-9]+]]: i64 = count;
// CHECK-NEXT:         let mut [[V_9:__v[0-9]+]]: i64 = [[V_8]]
// CHECK-NEXT:             + unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     unsafe { (*instrs).List.as_mut_ptr() as *mut [[SLATERECORD_2]] }
// CHECK-NEXT:                         .offset(i as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .a
// CHECK-NEXT:                     .Count2
// CHECK-NEXT:             };
// CHECK-NEXT:         count = [[V_9]];
// CHECK-NEXT:         let mut [[V_10:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_11:__v[0-9]+]]: i32 = [[V_10]] + (1 as i32);
// CHECK-NEXT:         i = [[V_11]];
// CHECK-NEXT:     }
// CHECK-NEXT:     count
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut p: *mut [[SLATERECORD_3]] = (unsafe {
// CHECK-NEXT:         malloc(
// CHECK-NEXT:             (4 as u64).wrapping_add((((3 as i32) as i64) as u64).wrapping_mul(30 as u64)),
// CHECK-NEXT:         )
// CHECK-NEXT:     }) as *mut [[SLATERECORD_3]];
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         memset(
// CHECK-NEXT:             p as *mut (),
// CHECK-NEXT:             0 as i32,
// CHECK-NEXT:             (4 as u64).wrapping_add((((3 as i32) as i64) as u64).wrapping_mul(30 as u64)),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*p).Count = 3 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*p).List.as_mut_ptr() as *mut [[SLATERECORD_2]] }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .a
// CHECK-NEXT:             .Count = ((555 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*p).List.as_mut_ptr() as *mut [[SLATERECORD_2]] }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .a
// CHECK-NEXT:             .Count = ((999 as i32) as i64) as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*p).List.as_mut_ptr() as *mut [[SLATERECORD_2]] }
// CHECK-NEXT:                 .offset((2 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .a
// CHECK-NEXT:             .Count = 4311810305 as u64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*p).List.as_mut_ptr() as *mut [[SLATERECORD_2]] }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .a
// CHECK-NEXT:             .Count2 = (555 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*p).List.as_mut_ptr() as *mut [[SLATERECORD_2]] }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .a
// CHECK-NEXT:             .Count2 = (999 as i32) as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         (*unsafe {
// CHECK-NEXT:             unsafe { (*p).List.as_mut_ptr() as *mut [[SLATERECORD_2]] }
// CHECK-NEXT:                 .offset((2 as i32) as isize)
// CHECK-NEXT:         })
// CHECK-NEXT:             .a
// CHECK-NEXT:             .Count2 = 4311810305 as i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     if Sum(p)
// CHECK-NEXT:         != ((((555 as i32) + (999 as i32)) as i64) as u64)
// CHECK-NEXT:             .wrapping_add(4311810305 as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if Sum2(p) != (((555 as i32) + (999 as i32)) as i64) + (4311810305 as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
