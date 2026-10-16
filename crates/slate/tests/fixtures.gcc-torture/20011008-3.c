/* { dg-add-options stack_size } */

extern void exit(int);
extern void abort(void);

typedef unsigned int  u_int32_t;
typedef unsigned char u_int8_t;
typedef int           int32_t;

typedef enum {
  TXNLIST_DELETE,
  TXNLIST_LSN,
  TXNLIST_TXNID,
  TXNLIST_PGNO
} db_txnlist_type;

struct __db_lsn;
typedef struct __db_lsn DB_LSN;
struct __db_lsn {
  u_int32_t file;
  u_int32_t offset;
};
struct __db_txnlist;
typedef struct __db_txnlist DB_TXNLIST;

struct __db_txnlist {
  db_txnlist_type type;
  struct {
    struct __db_txnlist  *le_next;
    struct __db_txnlist **le_prev;
  } links;
  union {
    struct {
      u_int32_t txnid;
      int32_t   generation;
      int32_t   aborted;
    } t;
    struct {

      u_int32_t flags;
      int32_t   fileid;
      u_int32_t count;
      char     *fname;
    } d;
    struct {
      int32_t ntxns;
      int32_t maxn;
      DB_LSN *lsn_array;
    } l;
    struct {
      int32_t  nentries;
      int32_t  maxentry;
      char    *fname;
      int32_t  fileid;
      void    *pgno_array;
      u_int8_t uid[20];
    } p;
  } u;
};

int log_compare(const DB_LSN *a, const DB_LSN *b) { return 1; }

int __db_txnlist_lsnadd(int val, DB_TXNLIST *elp, DB_LSN *lsnp,
                        u_int32_t flags) {
  int i;

  for (i = 0; i < (!(flags & (0x1)) ? 1 : elp->u.l.ntxns); i++) {
    int    __j;
    DB_LSN __tmp;
    val++;
    for (__j = 0; __j < elp->u.l.ntxns - 1; __j++)
      if (log_compare(&elp->u.l.lsn_array[__j], &elp->u.l.lsn_array[__j + 1]) <
          0) {
        __tmp                       = elp->u.l.lsn_array[__j];
        elp->u.l.lsn_array[__j]     = elp->u.l.lsn_array[__j + 1];
        elp->u.l.lsn_array[__j + 1] = __tmp;
      }
  }

  *lsnp = elp->u.l.lsn_array[0];
  return val;
}

#if defined(STACK_SIZE) && STACK_SIZE < 12350
#define VLEN (STACK_SIZE / 10)
#else
#define VLEN 1235
#endif

int main(void) {
  DB_TXNLIST el;
  DB_LSN     lsn, lsn_a[VLEN];

  el.u.l.ntxns     = VLEN - 1;
  el.u.l.lsn_array = lsn_a;

  if (__db_txnlist_lsnadd(0, &el, &lsn, 0) != 1)
    abort();

  if (__db_txnlist_lsnadd(0, &el, &lsn, 1) != VLEN - 1)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-add-options stack_size }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __db_lsn {
// CHECK-NEXT:     file: u32,
// CHECK-NEXT:     offset: u32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __db_txnlist {
// CHECK-NEXT:     r#type: u32,
// CHECK-NEXT:     links: [[SLATERECORD_0:__SlateRecord[0-9]+]],
// CHECK-NEXT:     u: [[SLATERECORD_1:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_0]] {
// CHECK-NEXT:     le_next: *mut __db_txnlist,
// CHECK-NEXT:     le_prev: *mut *mut __db_txnlist,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1]] {
// CHECK-NEXT:     t: [[SLATERECORD_2:__SlateRecord[0-9]+]],
// CHECK-NEXT:     d: [[SLATERECORD_3:__SlateRecord[0-9]+]],
// CHECK-NEXT:     l: [[SLATERECORD_4:__SlateRecord[0-9]+]],
// CHECK-NEXT:     p: [[SLATERECORD_5:__SlateRecord[0-9]+]],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_2]] {
// CHECK-NEXT:     txnid: u32,
// CHECK-NEXT:     generation: i32,
// CHECK-NEXT:     aborted: i32,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_3]] {
// CHECK-NEXT:     flags: u32,
// CHECK-NEXT:     fileid: i32,
// CHECK-NEXT:     count: u32,
// CHECK-NEXT:     fname: *mut i8,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_4]] {
// CHECK-NEXT:     ntxns: i32,
// CHECK-NEXT:     maxn: i32,
// CHECK-NEXT:     lsn_array: *mut __db_lsn,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct [[SLATERECORD_5]] {
// CHECK-NEXT:     nentries: i32,
// CHECK-NEXT:     maxentry: i32,
// CHECK-NEXT:     fname: *mut i8,
// CHECK-NEXT:     fileid: i32,
// CHECK-NEXT:     pgno_array: *mut (),
// CHECK-NEXT:     uid: [u8; 20],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn log_compare(mut a: *const __db_lsn, mut b: *const __db_lsn) -> i32 {
// CHECK-NEXT:     1 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __db_txnlist_lsnadd(
// CHECK-NEXT:     mut val: i32,
// CHECK-NEXT:     mut elp: &__db_txnlist,
// CHECK-NEXT:     mut lsnp: &mut __db_lsn,
// CHECK-NEXT:     mut flags: u32,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < if !(flags & ((1 as i32) as u32) != (0 as u32)) {
// CHECK-NEXT:         1 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { (*(elp as *const __db_txnlist)).u.l.ntxns }
// CHECK-NEXT:     } {
// CHECK-NEXT:         let mut __j: i32 = 0 as i32;
// CHECK-NEXT:         let mut __tmp: __db_lsn = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = val;
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = [[V_1]] + (1 as i32);
// CHECK-NEXT:         val = [[V_2]];
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while __j
// CHECK-NEXT:             < (unsafe { (*(elp as *const __db_txnlist)).u.l.ntxns }) - (1 as i32)
// CHECK-NEXT:         {
// CHECK-NEXT:             if log_compare(
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { (*(elp as *const __db_txnlist)).u.l.lsn_array }.offset(__j as isize)
// CHECK-NEXT:                 }) as *const __db_lsn,
// CHECK-NEXT:                 (unsafe {
// CHECK-NEXT:                     unsafe { (*(elp as *const __db_txnlist)).u.l.lsn_array }
// CHECK-NEXT:                         .offset((__j + (1 as i32)) as isize)
// CHECK-NEXT:                 }) as *const __db_lsn,
// CHECK-NEXT:             ) < (0 as i32)
// CHECK-NEXT:             {
// CHECK-NEXT:                 __tmp = unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { (*(elp as *const __db_txnlist)).u.l.lsn_array }
// CHECK-NEXT:                             .offset(__j as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 };
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { (*(elp as *const __db_txnlist)).u.l.lsn_array }
// CHECK-NEXT:                             .offset(__j as isize)
// CHECK-NEXT:                     } = unsafe {
// CHECK-NEXT:                         *unsafe {
// CHECK-NEXT:                             unsafe { (*(elp as *const __db_txnlist)).u.l.lsn_array }
// CHECK-NEXT:                                 .offset((__j + (1 as i32)) as isize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe { (*(elp as *const __db_txnlist)).u.l.lsn_array }
// CHECK-NEXT:                             .offset((__j + (1 as i32)) as isize)
// CHECK-NEXT:                     } = __tmp;
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: i32 = __j;
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: i32 = [[V_3]] + (1 as i32);
// CHECK-NEXT:             __j = [[V_4]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: i32 = [[V_5]] + (1 as i32);
// CHECK-NEXT:         i = [[V_6]];
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(lsnp as *mut __db_lsn) = unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { (*(elp as *const __db_txnlist)).u.l.lsn_array }.offset((0 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     val
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut el: __db_txnlist = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut lsn: __db_lsn = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut lsn_a: __SlateAlign16<[__db_lsn; 1235]> = __SlateAlign16(unsafe { std::mem::zeroed() });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         el.u.l.ntxns = (1235 as i32) - (1 as i32);
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         el.u.l.lsn_array = lsn_a.0.as_mut_ptr() as *mut __db_lsn;
// CHECK-NEXT:     }
// CHECK-NEXT:     if __db_txnlist_lsnadd(
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(el)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(lsn)) },
// CHECK-NEXT:         (0 as i32) as u32,
// CHECK-NEXT:     ) != (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if __db_txnlist_lsnadd(
// CHECK-NEXT:         0 as i32,
// CHECK-NEXT:         unsafe { &(*std::ptr::addr_of_mut!(el)) },
// CHECK-NEXT:         unsafe { &mut (*std::ptr::addr_of_mut!(lsn)) },
// CHECK-NEXT:         (1 as i32) as u32,
// CHECK-NEXT:     ) != (1235 as i32) - (1 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
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
