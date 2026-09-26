extern void abort(void);

typedef struct PgHdr  PgHdr;
typedef unsigned char u8;
struct PgHdr {
  int y;
  struct {
    unsigned int pgno;
    PgHdr       *pNextHash, *pPrevHash;
    PgHdr       *pNextFree, *pPrevFree;
    PgHdr       *pNextAll;
    u8           inJournal;
    short int    nRef;
    PgHdr       *pDirty, *pPrevDirty;
    unsigned int notUsed;
  } x;
};
PgHdr              **xx;
volatile int         vx;
static inline PgHdr *merge_pagelist(PgHdr *pA, PgHdr *pB) {
  PgHdr  result;
  PgHdr *pTail;
  xx    = &result.x.pDirty;
  pTail = &result;
  while (pA && pB) {
    if (pA->x.pgno < pB->x.pgno) {
      pTail->x.pDirty = pA;
      pTail           = pA;
      pA              = pA->x.pDirty;
    } else {
      pTail->x.pDirty = pB;
      pTail           = pB;
      pB              = pB->x.pDirty;
    }
    vx = (*xx)->y;
  }
  if (pA) {
    pTail->x.pDirty = pA;
  } else if (pB) {
    pTail->x.pDirty = pB;
  } else {
    pTail->x.pDirty = 0;
  }
  return result.x.pDirty;
}

PgHdr *__attribute__((noinline)) sort_pagelist(PgHdr *pIn) {
  PgHdr *a[25], *p;
  int    i;
  __builtin_memset(a, 0, sizeof(a));
  while (pIn) {
    p           = pIn;
    pIn         = p->x.pDirty;
    p->x.pDirty = 0;
    for (i = 0; i < 25 - 1; i++) {
      if (a[i] == 0) {
        a[i] = p;
        break;
      } else {
        p    = merge_pagelist(a[i], p);
        a[i] = 0;
        a[i] = 0;
      }
    }
    if (i == 25 - 1) {
      a[i] = merge_pagelist(a[i], p);
    }
  }
  p = a[0];
  for (i = 1; i < 25; i++) {
    p = merge_pagelist(p, a[i]);
  }
  return p;
}

int main() {
  PgHdr  a[5];
  PgHdr *p;
  a[0].x.pgno   = 5;
  a[0].x.pDirty = &a[1];
  a[1].x.pgno   = 4;
  a[1].x.pDirty = &a[2];
  a[2].x.pgno   = 1;
  a[2].x.pDirty = &a[3];
  a[3].x.pgno   = 3;
  a[3].x.pDirty = 0;
  p             = sort_pagelist(&a[0]);
  if (p->x.pDirty == p)
    abort();
  return 0;
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
// DEFAULT-NEXT:     type @type0 PgHdr = struct incomplete;
// DEFAULT-NEXT:     type @type1 PgHdr = @type0;
// DEFAULT-NEXT:     type @type2 u8 = u8;
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 pgno: u32;
// DEFAULT-NEXT:         field1 pNextHash: ptr<@type0>;
// DEFAULT-NEXT:         field2 pPrevHash: ptr<@type0>;
// DEFAULT-NEXT:         field3 pNextFree: ptr<@type0>;
// DEFAULT-NEXT:         field4 pPrevFree: ptr<@type0>;
// DEFAULT-NEXT:         field5 pNextAll: ptr<@type0>;
// DEFAULT-NEXT:         field6 inJournal: u8;
// DEFAULT-NEXT:         field7 nRef: i16;
// DEFAULT-NEXT:         field8 pDirty: ptr<@type0>;
// DEFAULT-NEXT:         field9 pPrevDirty: ptr<@type0>;
// DEFAULT-NEXT:         field10 notUsed: u32;
// DEFAULT-NEXT:     } [size=80, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 50, 56, 64, 72]];
// DEFAULT-NEXT:     global %5 xx: ptr<ptr<@type0>> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 vx: volatile i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @merge_pagelist(%8 pA: ptr<@type0>, %9 pB: ptr<@type0>) -> ptr<@type0> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %10 result: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %11 pTail: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<ptr<@type0>>>(%5, addr_of<ptr<ptr<@type0>>>(field8(field1(%10))));
// DEFAULT-NEXT:         write<ptr<@type0>>(%11, addr_of<ptr<@type0>>(%10));
// DEFAULT-NEXT:         while %20 logical_and<bool>(ne<ptr<@type0>>(read<ptr<@type0>>(%8), null<ptr<@type0>>), ne<ptr<@type0>>(read<ptr<@type0>>(%9), null<ptr<@type0>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if lt<u32>(read<u32>(field0(field1(deref(read<ptr<@type0>>(%8))))), read<u32>(field0(field1(deref(read<ptr<@type0>>(%9))))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<ptr<@type0>>(field8(field1(deref(read<ptr<@type0>>(%11)))), read<ptr<@type0>>(%8));
// DEFAULT-NEXT:                         write<ptr<@type0>>(%11, read<ptr<@type0>>(%8));
// DEFAULT-NEXT:                         write<ptr<@type0>>(%8, read<ptr<@type0>>(field8(field1(deref(read<ptr<@type0>>(%8))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<ptr<@type0>>(field8(field1(deref(read<ptr<@type0>>(%11)))), read<ptr<@type0>>(%9));
// DEFAULT-NEXT:                         write<ptr<@type0>>(%11, read<ptr<@type0>>(%9));
// DEFAULT-NEXT:                         write<ptr<@type0>>(%9, read<ptr<@type0>>(field8(field1(deref(read<ptr<@type0>>(%9))))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 write<i32, volatile>(%6, read<i32>(field0(deref(read<ptr<@type0>>(deref(read<ptr<ptr<@type0>>>(%5)))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<ptr<@type0>>(read<ptr<@type0>>(%8), null<ptr<@type0>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type0>>(field8(field1(deref(read<ptr<@type0>>(%11)))), read<ptr<@type0>>(%8));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<ptr<@type0>>(read<ptr<@type0>>(%9), null<ptr<@type0>>)
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type0>>(field8(field1(deref(read<ptr<@type0>>(%11)))), read<ptr<@type0>>(%9));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type0>>(field8(field1(deref(read<ptr<@type0>>(%11)))), null<ptr<@type0>>);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type0>>(field8(field1(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @sort_pagelist(%13 pIn: ptr<@type0>) -> ptr<@type0> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %14 a: array<ptr<@type0>, 25> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %15 p: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %16 i: i32 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14)), const<i32>(0), const<u64>(200));
// DEFAULT-NEXT:         while %21 ne<ptr<@type0>>(read<ptr<@type0>>(%13), null<ptr<@type0>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type0>>(%15, read<ptr<@type0>>(%13));
// DEFAULT-NEXT:                 write<ptr<@type0>>(%13, read<ptr<@type0>>(field8(field1(deref(read<ptr<@type0>>(%15))))));
// DEFAULT-NEXT:                 write<ptr<@type0>>(field8(field1(deref(read<ptr<@type0>>(%15)))), null<ptr<@type0>>);
// DEFAULT-NEXT:                 for %22
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%16, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%16), sub<i32, overflow=ub>(const<i32>(25), const<i32>(1)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %24: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%16, read<i32>(%25));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if eq<ptr<@type0>>(read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16)))), null<ptr<@type0>>)
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                     break %22;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<ptr<@type0>>(%15, call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%7, read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16)))), read<ptr<@type0>>(%15)));
// DEFAULT-NEXT:                                     call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%7, read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16)))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16))), null<ptr<@type0>>);
// DEFAULT-NEXT:                                     write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16))), null<ptr<@type0>>);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%16), sub<i32, overflow=ub>(const<i32>(25), const<i32>(1)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16))), call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%7, read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16)))), read<ptr<@type0>>(%15)));
// DEFAULT-NEXT:                         call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%7, read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16)))), read<ptr<@type0>>(%15));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<@type0>>(%15, read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), const<i32>(0)))));
// DEFAULT-NEXT:         for %23
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%16, const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%16), const<i32>(25))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = read<i32>(%16);
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%16, read<i32>(%27));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type0>>(%15, call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%7, read<ptr<@type0>>(%15), read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16))))));
// DEFAULT-NEXT:                     call<ptr<@type0>, signature=fn(ptr<@type0>, ptr<@type0>) -> ptr<@type0>>(%7, read<ptr<@type0>>(%15), read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(25)>(%14), read<i32>(%16)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type0>>(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %18 a: array<@type0, 5> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %19 p: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         write<u32>(field0(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(0))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         write<ptr<@type0>>(field8(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(0))))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(field0(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(1))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         write<ptr<@type0>>(field8(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(1))))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(2)))));
// DEFAULT-NEXT:         write<u32>(field0(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(2))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<@type0>>(field8(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(2))))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(3)))));
// DEFAULT-NEXT:         write<u32>(field0(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(3))))), reinterpret<u32, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<ptr<@type0>>(field8(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(3))))), null<ptr<@type0>>);
// DEFAULT-NEXT:         write<ptr<@type0>>(%19, call<ptr<@type0>, signature=fn(ptr<@type0>) -> ptr<@type0>>(%12, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(0))))));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn(ptr<@type0>) -> ptr<@type0>>(%12, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(5)>(%18), const<i32>(0)))));
// DEFAULT-NEXT:         if eq<ptr<@type0>>(read<ptr<@type0>>(field8(field1(deref(read<ptr<@type0>>(%19))))), read<ptr<@type0>>(%19))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
