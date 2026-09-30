/* PR rtl-optimization/23560 */

struct rtattr {
  unsigned short rta_len;
  unsigned short rta_type;
};

__attribute__((noinline)) int inet_check_attr(void *r, struct rtattr **rta) {
  int i;

  for (i = 1; i <= 14; i++) {
    struct rtattr *attr = rta[i - 1];
    if (attr) {
      if (attr->rta_len - sizeof(struct rtattr) < 4)
        return -22;
      if (i != 9 && i != 8)
        rta[i - 1] = attr + 1;
    }
  }
  return 0;
}

extern void abort(void);

int main(void) {
  struct rtattr  rt[2];
  struct rtattr *rta[14];
  int            i;

  rt[0].rta_len  = sizeof(struct rtattr) + 8;
  rt[0].rta_type = 0;
  rt[1]          = rt[0];
  for (i = 0; i < 14; i++)
    rta[i] = &rt[0];
  if (inet_check_attr(0, rta) != 0)
    abort();
  for (i = 0; i < 14; i++)
    if (rta[i] != &rt[i != 7 && i != 8])
      abort();
  for (i = 0; i < 14; i++)
    rta[i] = &rt[0];
  rta[1]         = 0;
  rt[1].rta_len -= 8;
  rta[5]         = &rt[1];
  if (inet_check_attr(0, rta) != -22)
    abort();
  for (i = 0; i < 14; i++)
    if (i == 1 && rta[i] != 0)
      abort();
    else if (i != 1 && i <= 5 && rta[i] != &rt[1])
      abort();
    else if (i > 5 && rta[i] != &rt[0])
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
// DEFAULT-NEXT:     type @type[[TYPE_rtattr:[0-9]+]] rtattr = struct {
// DEFAULT-NEXT:         field0 rta_len: u16;
// DEFAULT-NEXT:         field1 rta_type: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     fn %[[VALUE_inet_check_attr:[0-9]+]] @inet_check_attr(%[[VALUE_r:[0-9]+]] r: ptr<void>, %[[VALUE_rta:[0-9]+]] rta: ptr<ptr<@type[[TYPE_rtattr]]>>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%[[VALUE_i]]), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_attr:[0-9]+]] attr: ptr<@type[[TYPE_rtattr]]> [storage=automatic] = read<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_rtattr]]>>, subtract=false, element=ptr<@type[[TYPE_rtattr]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_rtattr]]>>>(%[[VALUE_rta]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1)))));
// DEFAULT-NEXT:                     if ne<ptr<@type[[TYPE_rtattr]]>>(read<ptr<@type[[TYPE_rtattr]]>>(%[[VALUE_attr]]), null<ptr<@type[[TYPE_rtattr]]>>)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if lt<u64>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(deref(read<ptr<@type[[TYPE_rtattr]]>>(%[[VALUE_attr]])))))))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                                 return neg<i32, overflow=ub>(const<i32>(22));
// DEFAULT-NEXT:                             if logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(9)), ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(8)))
// DEFAULT-NEXT:                                 write<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_rtattr]]>>, subtract=false, element=ptr<@type[[TYPE_rtattr]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_rtattr]]>>>(%[[VALUE_rta]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(1)))), ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(read<ptr<@type[[TYPE_rtattr]]>>(%[[VALUE_attr]]), const<i32>(1)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_rt:[0-9]+]] rt: array<@type[[TYPE_rtattr]], 2> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_rta_2:[0-9]+]] rta: array<ptr<@type[[TYPE_rtattr]]>, 14> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(field0(deref(ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), const<i32>(0)))), truncate<u16, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         write<u16>(field1(deref(ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), const<i32>(0)))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<@type[[TYPE_rtattr]]>(deref(ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), const<i32>(1))), copy<@type[[TYPE_rtattr]], reason=assign>(read<@type[[TYPE_rtattr]]>(deref(ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), const<i32>(0))))));
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_rtattr]]>>, subtract=false, element=ptr<@type[[TYPE_rtattr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_rtattr]]>>, length=Some(14)>(%[[VALUE_rta_2]]), read<i32>(%[[VALUE_i_2]]))), addr_of<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, ptr<ptr<@type[[TYPE_rtattr]]>>) -> i32>(%[[VALUE_inet_check_attr]], null<ptr<void>>, array_decay<ptr<ptr<@type[[TYPE_rtattr]]>>, length=Some(14)>(%[[VALUE_rta_2]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<ptr<@type[[TYPE_rtattr]]>>(read<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_rtattr]]>>, subtract=false, element=ptr<@type[[TYPE_rtattr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_rtattr]]>>, length=Some(14)>(%[[VALUE_rta_2]]), read<i32>(%[[VALUE_i_2]])))), addr_of<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(7)), ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(8))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_rtattr]]>>, subtract=false, element=ptr<@type[[TYPE_rtattr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_rtattr]]>>, length=Some(14)>(%[[VALUE_rta_2]]), read<i32>(%[[VALUE_i_2]]))), addr_of<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), const<i32>(0)))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_rtattr]]>>, subtract=false, element=ptr<@type[[TYPE_rtattr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_rtattr]]>>, length=Some(14)>(%[[VALUE_rta_2]]), const<i32>(1))), null<ptr<@type[[TYPE_rtattr]]>>);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: ptr<@type[[TYPE_rtattr]]> [synthetic] = ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: u16 [synthetic] = read<u16>(field0(deref(read<ptr<@type[[TYPE_rtattr]]>>(%[[VALUE12]]))));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE13]]))), const<i32>(8))));
// DEFAULT-NEXT:         write<u16>(field0(deref(read<ptr<@type[[TYPE_rtattr]]>>(%[[VALUE12]]))), read<u16>(%[[VALUE14]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_rtattr]]>>, subtract=false, element=ptr<@type[[TYPE_rtattr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_rtattr]]>>, length=Some(14)>(%[[VALUE_rta_2]]), const<i32>(5))), addr_of<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), const<i32>(1)))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, ptr<ptr<@type[[TYPE_rtattr]]>>) -> i32>(%[[VALUE_inet_check_attr]], null<ptr<void>>, array_decay<ptr<ptr<@type[[TYPE_rtattr]]>>, length=Some(14)>(%[[VALUE_rta_2]])), neg<i32, overflow=ub>(const<i32>(22)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(1)), ne<ptr<@type[[TYPE_rtattr]]>>(read<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_rtattr]]>>, subtract=false, element=ptr<@type[[TYPE_rtattr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_rtattr]]>>, length=Some(14)>(%[[VALUE_rta_2]]), read<i32>(%[[VALUE_i_2]])))), null<ptr<@type[[TYPE_rtattr]]>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(1)), le<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(5))), ne<ptr<@type[[TYPE_rtattr]]>>(read<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_rtattr]]>>, subtract=false, element=ptr<@type[[TYPE_rtattr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_rtattr]]>>, length=Some(14)>(%[[VALUE_rta_2]]), read<i32>(%[[VALUE_i_2]])))), addr_of<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), const<i32>(1))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if logical_and<bool>(gt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(5)), ne<ptr<@type[[TYPE_rtattr]]>>(read<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_rtattr]]>>, subtract=false, element=ptr<@type[[TYPE_rtattr]]>, overflow=ub>(array_decay<ptr<ptr<@type[[TYPE_rtattr]]>>, length=Some(14)>(%[[VALUE_rta_2]]), read<i32>(%[[VALUE_i_2]])))), addr_of<ptr<@type[[TYPE_rtattr]]>>(deref(ptr_offset<ptr<@type[[TYPE_rtattr]]>, subtract=false, element=@type[[TYPE_rtattr]], overflow=ub>(array_decay<ptr<@type[[TYPE_rtattr]]>, length=Some(2)>(%[[VALUE_rt]]), const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
