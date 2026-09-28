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
// DEFAULT-NEXT:     type @type0 rtattr = struct {
// DEFAULT-NEXT:         field0 rta_len: u16;
// DEFAULT-NEXT:         field1 rta_type: u16;
// DEFAULT-NEXT:     } [size=4, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     fn %1 @inet_check_attr(%2 r: ptr<void>, %3 rta: ptr<ptr<@type0>>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %11
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%4, const<i32>(1));
// DEFAULT-NEXT:             condition: le<i32>(read<i32>(%4), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %16: i32 [synthetic] = read<i32>(%4);
// DEFAULT-NEXT:                 let %17: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%16), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%4, read<i32>(%17));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %5 attr: ptr<@type0> [storage=automatic] = read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(read<ptr<ptr<@type0>>>(%3), sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1)))));
// DEFAULT-NEXT:                     if ne<ptr<@type0>>(read<ptr<@type0>>(%5), null<ptr<@type0>>)
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             if lt<u64>(sub<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field0(deref(read<ptr<@type0>>(%5)))))))), const<u64>(4)), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))))
// DEFAULT-NEXT:                                 return neg<i32, overflow=ub>(const<i32>(22));
// DEFAULT-NEXT:                             if logical_and<bool>(ne<i32>(read<i32>(%4), const<i32>(9)), ne<i32>(read<i32>(%4), const<i32>(8)))
// DEFAULT-NEXT:                                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(read<ptr<ptr<@type0>>>(%3), sub<i32, overflow=ub>(read<i32>(%4), const<i32>(1)))), ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%5), const<i32>(1)));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 rt: array<@type0, 2> [storage=automatic];
// DEFAULT-NEXT:         let %9 rta: array<ptr<@type0>, 14> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %10 i: i32 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), const<i32>(0)))), truncate<u16, reason=assign, fits=unknown>(add<u64, overflow=wrap>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(8))))));
// DEFAULT-NEXT:         write<u16>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), const<i32>(0)))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         write<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), const<i32>(1))), copy<@type0, reason=assign>(read<@type0>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), const<i32>(0))))));
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %18: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %19: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%19));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(14)>(%9), read<i32>(%10))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), const<i32>(0)))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, ptr<ptr<@type0>>) -> i32>(%1, null<ptr<void>>, array_decay<ptr<ptr<@type0>>, length=Some(14)>(%9)), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %20: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %21: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%20), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%21));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<ptr<@type0>>(read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(14)>(%9), read<i32>(%10)))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), from_bool<i32, reason=promotion>(logical_and<bool>(ne<i32>(read<i32>(%10), const<i32>(7)), ne<i32>(read<i32>(%10), const<i32>(8))))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %22: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%23));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(14)>(%9), read<i32>(%10))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), const<i32>(0)))));
// DEFAULT-NEXT:         write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(14)>(%9), const<i32>(1))), null<ptr<@type0>>);
// DEFAULT-NEXT:         let %24: ptr<@type0> [synthetic] = ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), const<i32>(1));
// DEFAULT-NEXT:         let %25: u16 [synthetic] = read<u16>(field0(deref(read<ptr<@type0>>(%24))));
// DEFAULT-NEXT:         let %26: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%25))), const<i32>(8))));
// DEFAULT-NEXT:         write<u16>(field0(deref(read<ptr<@type0>>(%24))), read<u16>(%26));
// DEFAULT-NEXT:         write<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(14)>(%9), const<i32>(5))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), const<i32>(1)))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<void>, ptr<ptr<@type0>>) -> i32>(%1, null<ptr<void>>, array_decay<ptr<ptr<@type0>>, length=Some(14)>(%9)), neg<i32, overflow=ub>(const<i32>(22)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%10), const<i32>(14))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %27: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                 let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%10, read<i32>(%28));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_and<bool>(eq<i32>(read<i32>(%10), const<i32>(1)), ne<ptr<@type0>>(read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(14)>(%9), read<i32>(%10)))), null<ptr<@type0>>))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     if logical_and<bool>(logical_and<bool>(ne<i32>(read<i32>(%10), const<i32>(1)), le<i32>(read<i32>(%10), const<i32>(5))), ne<ptr<@type0>>(read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(14)>(%9), read<i32>(%10)))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), const<i32>(1))))))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if logical_and<bool>(gt<i32>(read<i32>(%10), const<i32>(5)), ne<ptr<@type0>>(read<ptr<@type0>>(deref(ptr_offset<ptr<ptr<@type0>>, subtract=false, element=ptr<@type0>, overflow=ub>(array_decay<ptr<ptr<@type0>>, length=Some(14)>(%9), read<i32>(%10)))), addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(2)>(%8), const<i32>(0))))))
// DEFAULT-NEXT:                             call<void, signature=fn() -> void>(%6);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
