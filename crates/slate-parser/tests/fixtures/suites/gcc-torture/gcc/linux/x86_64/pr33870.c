extern void abort(void);

typedef struct PgHdr  PgHdr;
typedef unsigned char u8;
struct PgHdr {
  unsigned int pgno;
  PgHdr       *pNextHash, *pPrevHash;
  PgHdr       *pNextFree, *pPrevFree;
  PgHdr       *pNextAll;
  u8           inJournal;
  short int    nRef;
  PgHdr       *pDirty, *pPrevDirty;
  unsigned int notUsed;
};

static inline PgHdr *merge_pagelist(PgHdr *pA, PgHdr *pB) {
  PgHdr  result;
  PgHdr *pTail;
  pTail = &result;
  while (pA && pB) {
    if (pA->pgno < pB->pgno) {
      pTail->pDirty = pA;
      pTail         = pA;
      pA            = pA->pDirty;
    } else {
      pTail->pDirty = pB;
      pTail         = pB;
      pB            = pB->pDirty;
    }
  }
  if (pA) {
    pTail->pDirty = pA;
  } else if (pB) {
    pTail->pDirty = pB;
  } else {
    pTail->pDirty = 0;
  }
  return result.pDirty;
}

PgHdr *__attribute__((noinline)) sort_pagelist(PgHdr *pIn) {
  PgHdr *a[25], *p;
  int    i;
  __builtin_memset(a, 0, sizeof(a));
  while (pIn) {
    p         = pIn;
    pIn       = p->pDirty;
    p->pDirty = 0;
    for (i = 0; i < 25 - 1; i++) {
      if (a[i] == 0) {
        a[i] = p;
        break;
      } else {
        p    = merge_pagelist(a[i], p);
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
  a[0].pgno   = 5;
  a[0].pDirty = &a[1];
  a[1].pgno   = 4;
  a[1].pDirty = &a[2];
  a[2].pgno   = 1;
  a[2].pDirty = &a[3];
  a[3].pgno   = 3;
  a[3].pDirty = 0;
  p           = sort_pagelist(&a[0]);
  if (p->pDirty == p)
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
// DEFAULT-NEXT:     type @type[[TYPE_PgHdr:[0-9]+]] PgHdr = struct {
// DEFAULT-NEXT:         field0 pgno: u32;
// DEFAULT-NEXT:         field1 pNextHash: ptr<@type[[TYPE_PgHdr]]>;
// DEFAULT-NEXT:         field2 pPrevHash: ptr<@type[[TYPE_PgHdr]]>;
// DEFAULT-NEXT:         field3 pNextFree: ptr<@type[[TYPE_PgHdr]]>;
// DEFAULT-NEXT:         field4 pPrevFree: ptr<@type[[TYPE_PgHdr]]>;
// DEFAULT-NEXT:         field5 pNextAll: ptr<@type[[TYPE_PgHdr]]>;
// DEFAULT-NEXT:         field6 inJournal: u8;
// DEFAULT-NEXT:         field7 nRef: i16;
// DEFAULT-NEXT:         field8 pDirty: ptr<@type[[TYPE_PgHdr]]>;
// DEFAULT-NEXT:         field9 pPrevDirty: ptr<@type[[TYPE_PgHdr]]>;
// DEFAULT-NEXT:         field10 notUsed: u32;
// DEFAULT-NEXT:     } [size=80, align=8, offsets=[0, 8, 16, 24, 32, 40, 48, 50, 56, 64, 72]];
// DEFAULT-NEXT:     type @type[[TYPE_PgHdr_2:[0-9]+]] PgHdr = @type[[TYPE_PgHdr]];
// DEFAULT-NEXT:     type @type[[TYPE_u8:[0-9]+]] u8 = u8;
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_merge_pagelist:[0-9]+]] @merge_pagelist(%[[VALUE_pA:[0-9]+]] pA: ptr<@type[[TYPE_PgHdr]]>, %[[VALUE_pB:[0-9]+]] pB: ptr<@type[[TYPE_PgHdr]]>) -> ptr<@type[[TYPE_PgHdr]]> [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_result:[0-9]+]] result: @type[[TYPE_PgHdr]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_pTail:[0-9]+]] pTail: ptr<@type[[TYPE_PgHdr]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pTail]], addr_of<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_result]]));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] logical_and<bool>(ne<ptr<@type[[TYPE_PgHdr]]>>(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pA]]), null<ptr<@type[[TYPE_PgHdr]]>>), ne<ptr<@type[[TYPE_PgHdr]]>>(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pB]]), null<ptr<@type[[TYPE_PgHdr]]>>))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if lt<u32>(read<u32>(field0(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pA]])))), read<u32>(field0(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pB]])))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pTail]]))), read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pA]]));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pTail]], read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pA]]));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pA]], read<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pA]])))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pTail]]))), read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pB]]));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pTail]], read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pB]]));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pB]], read<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pB]])))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_PgHdr]]>>(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pA]]), null<ptr<@type[[TYPE_PgHdr]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pTail]]))), read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pA]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if ne<ptr<@type[[TYPE_PgHdr]]>>(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pB]]), null<ptr<@type[[TYPE_PgHdr]]>>)
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pTail]]))), read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pB]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pTail]]))), null<ptr<@type[[TYPE_PgHdr]]>>);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_PgHdr]]>>(field8(%[[VALUE_result]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: i32, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sort_pagelist:[0-9]+]] @sort_pagelist(%[[VALUE_pIn:[0-9]+]] pIn: ptr<@type[[TYPE_PgHdr]]>) -> ptr<@type[[TYPE_PgHdr]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_a:[0-9]+]] a: array<ptr<@type[[TYPE_PgHdr]]>, 25> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_PgHdr]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<ptr<@type[[TYPE_PgHdr]]>>, length=Some(25)>(%[[VALUE_a]])), const<i32>(0), const<u64>(200));
// DEFAULT-NEXT:         while %[[VALUE4:[0-9]+]] ne<ptr<@type[[TYPE_PgHdr]]>>(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pIn]]), null<ptr<@type[[TYPE_PgHdr]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]], read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pIn]]));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_pIn]], read<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]])))));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]]))), null<ptr<@type[[TYPE_PgHdr]]>>);
// DEFAULT-NEXT:                 for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_i]]), sub<i32, overflow=ub>(const<i32>(25), const<i32>(1)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if eq<ptr<@type[[TYPE_PgHdr]]>>(read<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_PgHdr]]>>, subtract=false, element=ptr<@type[[TYPE_PgHdr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_PgHdr]]>>, length=Some(25)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]])))), null<ptr<@type[[TYPE_PgHdr]]>>)
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_PgHdr]]>>, subtract=false, element=ptr<@type[[TYPE_PgHdr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_PgHdr]]>>, length=Some(25)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]]))), read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]]));
// DEFAULT-NEXT:                                     break %[[VALUE5]];
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                             else
// DEFAULT-NEXT:                                 {
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]], call<ptr<@type[[TYPE_PgHdr]]>, signature=fn(ptr<@type[[TYPE_PgHdr]]>, ptr<@type[[TYPE_PgHdr]]>) -> ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_merge_pagelist]], read<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_PgHdr]]>>, subtract=false, element=ptr<@type[[TYPE_PgHdr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_PgHdr]]>>, length=Some(25)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]])))), read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]])));
// DEFAULT-NEXT:                                     write<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_PgHdr]]>>, subtract=false, element=ptr<@type[[TYPE_PgHdr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_PgHdr]]>>, length=Some(25)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]]))), null<ptr<@type[[TYPE_PgHdr]]>>);
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 if eq<i32>(read<i32>(%[[VALUE_i]]), sub<i32, overflow=ub>(const<i32>(25), const<i32>(1)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_PgHdr]]>>, subtract=false, element=ptr<@type[[TYPE_PgHdr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_PgHdr]]>>, length=Some(25)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]]))), call<ptr<@type[[TYPE_PgHdr]]>, signature=fn(ptr<@type[[TYPE_PgHdr]]>, ptr<@type[[TYPE_PgHdr]]>) -> ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_merge_pagelist]], read<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_PgHdr]]>>, subtract=false, element=ptr<@type[[TYPE_PgHdr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_PgHdr]]>>, length=Some(25)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]])))), read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]])));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]], read<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_PgHdr]]>>, subtract=false, element=ptr<@type[[TYPE_PgHdr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_PgHdr]]>>, length=Some(25)>(%[[VALUE_a]]), const<i32>(0)))));
// DEFAULT-NEXT:         for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(25))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]], call<ptr<@type[[TYPE_PgHdr]]>, signature=fn(ptr<@type[[TYPE_PgHdr]]>, ptr<@type[[TYPE_PgHdr]]>) -> ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_merge_pagelist]], read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]]), read<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_PgHdr]]>>, subtract=false, element=ptr<@type[[TYPE_PgHdr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_PgHdr]]>>, length=Some(25)>(%[[VALUE_a]]), read<i32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_a_2:[0-9]+]] a: array<@type[[TYPE_PgHdr]], 5> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_PgHdr]]> [storage=automatic];
// DEFAULT-NEXT:         write<u32>(field0(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(0)))), reinterpret<u32, reason=assign, fits=always>(const<i32>(5)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(0)))), addr_of<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(1)))));
// DEFAULT-NEXT:         write<u32>(field0(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(1)))), reinterpret<u32, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(1)))), addr_of<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(2)))));
// DEFAULT-NEXT:         write<u32>(field0(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(2)))), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(2)))), addr_of<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(3)))));
// DEFAULT-NEXT:         write<u32>(field0(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(3)))), reinterpret<u32, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(3)))), null<ptr<@type[[TYPE_PgHdr]]>>);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p_2]], call<ptr<@type[[TYPE_PgHdr]]>, signature=fn(ptr<@type[[TYPE_PgHdr]]>) -> ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_sort_pagelist]], addr_of<ptr<@type[[TYPE_PgHdr]]>>(deref(ptr_offset<ptr<@type[[TYPE_PgHdr]]>, subtract=false, element=@type[[TYPE_PgHdr]], overflow=ub>(array_decay<ptr<@type[[TYPE_PgHdr]]>, length=Some(5)>(%[[VALUE_a_2]]), const<i32>(0))))));
// DEFAULT-NEXT:         if eq<ptr<@type[[TYPE_PgHdr]]>>(read<ptr<@type[[TYPE_PgHdr]]>>(field8(deref(read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p_2]])))), read<ptr<@type[[TYPE_PgHdr]]>>(%[[VALUE_p_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
