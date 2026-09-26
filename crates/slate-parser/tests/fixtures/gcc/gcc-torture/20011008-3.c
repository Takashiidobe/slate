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


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 u_int32_t = u32;
// DEFAULT-NEXT:     type @type1 u_int8_t = u8;
// DEFAULT-NEXT:     type @type2 int32_t = i32;
// DEFAULT-NEXT:     type @type3 = enum : u32 {
// DEFAULT-NEXT:         %0 TXNLIST_DELETE = const<i32>(0);
// DEFAULT-NEXT:         %1 TXNLIST_LSN = const<i32>(1);
// DEFAULT-NEXT:         %2 TXNLIST_TXNID = const<i32>(2);
// DEFAULT-NEXT:         %3 TXNLIST_PGNO = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type4 db_txnlist_type = @type3;
// DEFAULT-NEXT:     type @type5 __db_lsn = struct {
// DEFAULT-NEXT:         field0 file: u32;
// DEFAULT-NEXT:         field1 offset: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type6 DB_LSN = @type5;
// DEFAULT-NEXT:     type @type7 __db_txnlist = struct {
// DEFAULT-NEXT:         field0 type: @type3;
// DEFAULT-NEXT:         field1 links: @type9;
// DEFAULT-NEXT:         field2 u: @type10;
// DEFAULT-NEXT:     } [size=80, align=8, offsets=[0, 8, 24]];
// DEFAULT-NEXT:     type @type8 DB_TXNLIST = @type7;
// DEFAULT-NEXT:     type @type9 = struct {
// DEFAULT-NEXT:         field0 le_next: ptr<@type7>;
// DEFAULT-NEXT:         field1 le_prev: ptr<ptr<@type7>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type10 = union {
// DEFAULT-NEXT:         field0 t: @type11;
// DEFAULT-NEXT:         field1 d: @type12;
// DEFAULT-NEXT:         field2 l: @type13;
// DEFAULT-NEXT:         field3 p: @type14;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type11 = struct {
// DEFAULT-NEXT:         field0 txnid: u32;
// DEFAULT-NEXT:         field1 generation: i32;
// DEFAULT-NEXT:         field2 aborted: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type12 = struct {
// DEFAULT-NEXT:         field0 flags: u32;
// DEFAULT-NEXT:         field1 fileid: i32;
// DEFAULT-NEXT:         field2 count: u32;
// DEFAULT-NEXT:         field3 fname: ptr<i8>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 8, 16]];
// DEFAULT-NEXT:     type @type13 = struct {
// DEFAULT-NEXT:         field0 ntxns: i32;
// DEFAULT-NEXT:         field1 maxn: i32;
// DEFAULT-NEXT:         field2 lsn_array: ptr<@type5>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type14 = struct {
// DEFAULT-NEXT:         field0 nentries: i32;
// DEFAULT-NEXT:         field1 maxentry: i32;
// DEFAULT-NEXT:         field2 fname: ptr<i8>;
// DEFAULT-NEXT:         field3 fileid: i32;
// DEFAULT-NEXT:         field4 pgno_array: ptr<void>;
// DEFAULT-NEXT:         field5 uid: array<u8, 20>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 4, 8, 16, 24, 32]];
// DEFAULT-NEXT:     fn %0 @exit(%36 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %21 @log_compare(%22 a: ptr<const @type5>, %23 b: ptr<const @type5>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @__db_txnlist_lsnadd(%25 val: i32, %26 elp: ptr<@type7>, %27 lsnp: ptr<@type5>, %28 flags: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %29 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %37
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%29, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%29), conditional<i32>(not<bool>(ne<u32>(and<u32>(read<u32>(%28), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), const<u32>(0))), const<i32>(1), read<i32>(field0(field2(field2(deref(read<ptr<@type7>>(%26))))))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = read<i32>(%29);
// DEFAULT-NEXT:                 let %40: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%39), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%29, read<i32>(%40));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %30 __j: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %31 __tmp: @type5 [storage=automatic];
// DEFAULT-NEXT:                     let %41: i32 [synthetic] = read<i32>(%25);
// DEFAULT-NEXT:                     let %42: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%41), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%25, read<i32>(%42));
// DEFAULT-NEXT:                     for %38
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%30, const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%30), sub<i32, overflow=ub>(read<i32>(field0(field2(field2(deref(read<ptr<@type7>>(%26)))))), const<i32>(1)))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %43: i32 [synthetic] = read<i32>(%30);
// DEFAULT-NEXT:                             let %44: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%43), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%30, read<i32>(%44));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             if lt<i32>(call<i32, signature=fn(ptr<const @type5>, ptr<const @type5>) -> i32>(%21, pointer_cast<ptr<const @type5>, reason=arg>(addr_of<ptr<@type5>>(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(read<ptr<@type5>>(field2(field2(field2(deref(read<ptr<@type7>>(%26)))))), read<i32>(%30))))), pointer_cast<ptr<const @type5>, reason=arg>(addr_of<ptr<@type5>>(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(read<ptr<@type5>>(field2(field2(field2(deref(read<ptr<@type7>>(%26)))))), add<i32, overflow=ub>(read<i32>(%30), const<i32>(1))))))), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<@type5>(%31, copy<@type5, reason=assign>(read<@type5>(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(read<ptr<@type5>>(field2(field2(field2(deref(read<ptr<@type7>>(%26)))))), read<i32>(%30))))));
// DEFAULT-NEXT:                                     write<@type5>(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(read<ptr<@type5>>(field2(field2(field2(deref(read<ptr<@type7>>(%26)))))), read<i32>(%30))), copy<@type5, reason=assign>(read<@type5>(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(read<ptr<@type5>>(field2(field2(field2(deref(read<ptr<@type7>>(%26)))))), add<i32, overflow=ub>(read<i32>(%30), const<i32>(1)))))));
// DEFAULT-NEXT:                                     write<@type5>(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(read<ptr<@type5>>(field2(field2(field2(deref(read<ptr<@type7>>(%26)))))), add<i32, overflow=ub>(read<i32>(%30), const<i32>(1)))), copy<@type5, reason=assign>(read<@type5>(%31)));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<@type5>(deref(read<ptr<@type5>>(%27)), copy<@type5, reason=assign>(read<@type5>(deref(ptr_offset<ptr<@type5>, subtract=false, element=@type5, overflow=ub>(read<ptr<@type5>>(field2(field2(field2(deref(read<ptr<@type7>>(%26)))))), const<i32>(0))))));
// DEFAULT-NEXT:         return read<i32>(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %33 el: @type7 [storage=automatic];
// DEFAULT-NEXT:         let %34 lsn: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %35 lsn_a: array<@type5, 1235> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<i32>(field0(field2(field2(%33))), sub<i32, overflow=ub>(const<i32>(1235), const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<@type5>>(field2(field2(field2(%33))), array_decay<ptr<@type5>, length=Some(1235)>(%35));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<@type7>, ptr<@type5>, u32) -> i32>(%24, const<i32>(0), addr_of<ptr<@type7>>(%33), addr_of<ptr<@type5>>(%34), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<@type7>, ptr<@type5>, u32) -> i32>(%24, const<i32>(0), addr_of<ptr<@type7>>(%33), addr_of<ptr<@type5>>(%34), reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(1235), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
