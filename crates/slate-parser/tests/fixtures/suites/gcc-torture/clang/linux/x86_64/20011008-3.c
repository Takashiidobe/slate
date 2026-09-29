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
// DEFAULT-NEXT:     type @type[[TYPE_u_int32_t:[0-9]+]] u_int32_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_u_int8_t:[0-9]+]] u_int8_t = u8;
// DEFAULT-NEXT:     type @type[[TYPE_int32_t:[0-9]+]] int32_t = i32;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_TXNLIST_DELETE:[0-9]+]] TXNLIST_DELETE = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_TXNLIST_LSN:[0-9]+]] TXNLIST_LSN = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_TXNLIST_TXNID:[0-9]+]] TXNLIST_TXNID = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_TXNLIST_PGNO:[0-9]+]] TXNLIST_PGNO = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_db_txnlist_type:[0-9]+]] db_txnlist_type = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE___db_lsn:[0-9]+]] __db_lsn = struct {
// DEFAULT-NEXT:         field0 file: u32;
// DEFAULT-NEXT:         field1 offset: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_DB_LSN:[0-9]+]] DB_LSN = @type[[TYPE___db_lsn]];
// DEFAULT-NEXT:     type @type[[TYPE___db_txnlist:[0-9]+]] __db_txnlist = struct {
// DEFAULT-NEXT:         field0 type: @type[[TYPE0]];
// DEFAULT-NEXT:         field1 links: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:         field2 u: @type[[TYPE2:[0-9]+]];
// DEFAULT-NEXT:     } [size=80, align=8, offsets=[0, 8, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_DB_TXNLIST:[0-9]+]] DB_TXNLIST = @type[[TYPE___db_txnlist]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = struct {
// DEFAULT-NEXT:         field0 le_next: ptr<@type[[TYPE___db_txnlist]]>;
// DEFAULT-NEXT:         field1 le_prev: ptr<ptr<@type[[TYPE___db_txnlist]]>>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE2]] = union {
// DEFAULT-NEXT:         field0 t: @type[[TYPE3:[0-9]+]];
// DEFAULT-NEXT:         field1 d: @type[[TYPE4:[0-9]+]];
// DEFAULT-NEXT:         field2 l: @type[[TYPE5:[0-9]+]];
// DEFAULT-NEXT:         field3 p: @type[[TYPE6:[0-9]+]];
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE3]] = struct {
// DEFAULT-NEXT:         field0 txnid: u32;
// DEFAULT-NEXT:         field1 generation: i32;
// DEFAULT-NEXT:         field2 aborted: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE4]] = struct {
// DEFAULT-NEXT:         field0 flags: u32;
// DEFAULT-NEXT:         field1 fileid: i32;
// DEFAULT-NEXT:         field2 count: u32;
// DEFAULT-NEXT:         field3 fname: ptr<i8>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 4, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE5]] = struct {
// DEFAULT-NEXT:         field0 ntxns: i32;
// DEFAULT-NEXT:         field1 maxn: i32;
// DEFAULT-NEXT:         field2 lsn_array: ptr<@type[[TYPE___db_lsn]]>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE6]] = struct {
// DEFAULT-NEXT:         field0 nentries: i32;
// DEFAULT-NEXT:         field1 maxentry: i32;
// DEFAULT-NEXT:         field2 fname: ptr<i8>;
// DEFAULT-NEXT:         field3 fileid: i32;
// DEFAULT-NEXT:         field4 pgno_array: ptr<void>;
// DEFAULT-NEXT:         field5 uid: array<u8, 20>;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 4, 8, 16, 24, 32]];
// DEFAULT-NEXT:     fn %[[VALUE_TXNLIST_DELETE]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_TXNLIST_LSN]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_log_compare:[0-9]+]] @log_compare(%[[VALUE_a:[0-9]+]] a: ptr<const @type[[TYPE___db_lsn]]>, %[[VALUE_b:[0-9]+]] b: ptr<const @type[[TYPE___db_lsn]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___db_txnlist_lsnadd:[0-9]+]] @__db_txnlist_lsnadd(%[[VALUE_val:[0-9]+]] val: i32, %[[VALUE_elp:[0-9]+]] elp: ptr<@type[[TYPE___db_txnlist]]>, %[[VALUE_lsnp:[0-9]+]] lsnp: ptr<@type[[TYPE___db_lsn]]>, %[[VALUE_flags:[0-9]+]] flags: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), conditional<i32>(not<bool>(ne<u32>(and<u32>(read<u32>(%[[VALUE_flags]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))), const<u32>(0))), const<i32>(1), read<i32>(field0(field2(field2(deref(read<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_elp]]))))))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE___j:[0-9]+]] __j: i32 [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE___tmp:[0-9]+]] __tmp: @type[[TYPE___db_lsn]] [storage=automatic];
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_val]]);
// DEFAULT-NEXT:                     let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_val]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                     for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<i32>(%[[VALUE___j]], const<i32>(0));
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE___j]]), sub<i32, overflow=ub>(read<i32>(field0(field2(field2(deref(read<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_elp]])))))), const<i32>(1)))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE___j]]);
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE___j]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             if lt<i32>(call<i32, signature=fn(ptr<const @type[[TYPE___db_lsn]]>, ptr<const @type[[TYPE___db_lsn]]>) -> i32>(%[[VALUE_log_compare]], pointer_cast<ptr<const @type[[TYPE___db_lsn]]>, reason=arg>(addr_of<ptr<@type[[TYPE___db_lsn]]>>(deref(ptr_offset<ptr<@type[[TYPE___db_lsn]]>, subtract=false, element=@type[[TYPE___db_lsn]], overflow=ub>(read<ptr<@type[[TYPE___db_lsn]]>>(field2(field2(field2(deref(read<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_elp]])))))), read<i32>(%[[VALUE___j]]))))), pointer_cast<ptr<const @type[[TYPE___db_lsn]]>, reason=arg>(addr_of<ptr<@type[[TYPE___db_lsn]]>>(deref(ptr_offset<ptr<@type[[TYPE___db_lsn]]>, subtract=false, element=@type[[TYPE___db_lsn]], overflow=ub>(read<ptr<@type[[TYPE___db_lsn]]>>(field2(field2(field2(deref(read<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_elp]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE___j]]), const<i32>(1))))))), const<i32>(0))
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<@type[[TYPE___db_lsn]]>(%[[VALUE___tmp]], copy<@type[[TYPE___db_lsn]], reason=assign>(read<@type[[TYPE___db_lsn]]>(deref(ptr_offset<ptr<@type[[TYPE___db_lsn]]>, subtract=false, element=@type[[TYPE___db_lsn]], overflow=ub>(read<ptr<@type[[TYPE___db_lsn]]>>(field2(field2(field2(deref(read<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_elp]])))))), read<i32>(%[[VALUE___j]]))))));
// DEFAULT-NEXT:                                     write<@type[[TYPE___db_lsn]]>(deref(ptr_offset<ptr<@type[[TYPE___db_lsn]]>, subtract=false, element=@type[[TYPE___db_lsn]], overflow=ub>(read<ptr<@type[[TYPE___db_lsn]]>>(field2(field2(field2(deref(read<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_elp]])))))), read<i32>(%[[VALUE___j]]))), copy<@type[[TYPE___db_lsn]], reason=assign>(read<@type[[TYPE___db_lsn]]>(deref(ptr_offset<ptr<@type[[TYPE___db_lsn]]>, subtract=false, element=@type[[TYPE___db_lsn]], overflow=ub>(read<ptr<@type[[TYPE___db_lsn]]>>(field2(field2(field2(deref(read<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_elp]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE___j]]), const<i32>(1)))))));
// DEFAULT-NEXT:                                     write<@type[[TYPE___db_lsn]]>(deref(ptr_offset<ptr<@type[[TYPE___db_lsn]]>, subtract=false, element=@type[[TYPE___db_lsn]], overflow=ub>(read<ptr<@type[[TYPE___db_lsn]]>>(field2(field2(field2(deref(read<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_elp]])))))), add<i32, overflow=ub>(read<i32>(%[[VALUE___j]]), const<i32>(1)))), copy<@type[[TYPE___db_lsn]], reason=assign>(read<@type[[TYPE___db_lsn]]>(%[[VALUE___tmp]])));
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<@type[[TYPE___db_lsn]]>(deref(read<ptr<@type[[TYPE___db_lsn]]>>(%[[VALUE_lsnp]])), copy<@type[[TYPE___db_lsn]], reason=assign>(read<@type[[TYPE___db_lsn]]>(deref(ptr_offset<ptr<@type[[TYPE___db_lsn]]>, subtract=false, element=@type[[TYPE___db_lsn]], overflow=ub>(read<ptr<@type[[TYPE___db_lsn]]>>(field2(field2(field2(deref(read<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_elp]])))))), const<i32>(0))))));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_val]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_el:[0-9]+]] el: @type[[TYPE___db_txnlist]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_lsn:[0-9]+]] lsn: @type[[TYPE___db_lsn]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_lsn_a:[0-9]+]] lsn_a: array<@type[[TYPE___db_lsn]], 1235> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<i32>(field0(field2(field2(%[[VALUE_el]]))), sub<i32, overflow=ub>(const<i32>(1235), const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE___db_lsn]]>>(field2(field2(field2(%[[VALUE_el]]))), array_decay<ptr<@type[[TYPE___db_lsn]]>, length=Some(1235)>(%[[VALUE_lsn_a]]));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<@type[[TYPE___db_txnlist]]>, ptr<@type[[TYPE___db_lsn]]>, u32) -> i32>(%[[VALUE___db_txnlist_lsnadd]], const<i32>(0), addr_of<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_el]]), addr_of<ptr<@type[[TYPE___db_lsn]]>>(%[[VALUE_lsn]]), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_TXNLIST_LSN]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, ptr<@type[[TYPE___db_txnlist]]>, ptr<@type[[TYPE___db_lsn]]>, u32) -> i32>(%[[VALUE___db_txnlist_lsnadd]], const<i32>(0), addr_of<ptr<@type[[TYPE___db_txnlist]]>>(%[[VALUE_el]]), addr_of<ptr<@type[[TYPE___db_lsn]]>>(%[[VALUE_lsn]]), reinterpret<u32, reason=arg, fits=always>(const<i32>(1))), sub<i32, overflow=ub>(const<i32>(1235), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_TXNLIST_LSN]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_TXNLIST_DELETE]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
