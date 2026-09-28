/* PR rtl-optimization/65401 */

struct S {
  unsigned short s[64];
};

__attribute__((noinline, noclone)) void foo(struct S *x) {
  unsigned int   i;
  unsigned char *s;

  s = (unsigned char *)x->s;
  for (i = 0; i < 64; i++)
    x->s[i] = s[i * 2] | (s[i * 2 + 1] << 8);
}

__attribute__((noinline, noclone)) void bar(struct S *x) {
  unsigned int   i;
  unsigned char *s;

  s = (unsigned char *)x->s;
  for (i = 0; i < 64; i++)
    x->s[i] = (s[i * 2] << 8) | s[i * 2 + 1];
}

int main() {
  unsigned int i;
  struct S     s;
  if (sizeof(unsigned short) != 2)
    return 0;
  for (i = 0; i < 64; i++)
    s.s[i] = i + ((64 - i) << 8);
  foo(&s);
#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  for (i = 0; i < 64; i++)
    if (s.s[i] != (64 - i) + (i << 8))
      __builtin_abort();
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  for (i = 0; i < 64; i++)
    if (s.s[i] != i + ((64 - i) << 8))
      __builtin_abort();
#endif
  for (i = 0; i < 64; i++)
    s.s[i] = i + ((64 - i) << 8);
  bar(&s);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  for (i = 0; i < 64; i++)
    if (s.s[i] != (64 - i) + (i << 8))
      __builtin_abort();
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
  for (i = 0; i < 64; i++)
    if (s.s[i] != i + ((64 - i) << 8))
      __builtin_abort();
#endif
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 s: array<u16, 64>;
// DEFAULT-NEXT:     } [size=128, align=2, offsets=[0]];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 i: u32 [storage=automatic];
// DEFAULT-NEXT:         let %4 s: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<u8>>(%4, pointer_cast<ptr<u8>, reason=explicit>(array_decay<ptr<u16>, length=Some(64)>(field0(deref(read<ptr<@type0>>(%2))))));
// DEFAULT-NEXT:         for %12
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%3, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %19: u32 [synthetic] = read<u32>(%3);
// DEFAULT-NEXT:                 let %20: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%19), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%3, read<u32>(%20));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(64)>(field0(deref(read<ptr<@type0>>(%2)))), read<u32>(%3))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%4), mul<u32, overflow=wrap>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))))))), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%4), add<u32, overflow=wrap>(mul<u32, overflow=wrap>(read<u32>(%3), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))))), const<i32>(8))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bar(%6 x: ptr<@type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 i: u32 [storage=automatic];
// DEFAULT-NEXT:         let %8 s: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<u8>>(%8, pointer_cast<ptr<u8>, reason=explicit>(array_decay<ptr<u16>, length=Some(64)>(field0(deref(read<ptr<@type0>>(%6))))));
// DEFAULT-NEXT:         for %13
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%7, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %21: u32 [synthetic] = read<u32>(%7);
// DEFAULT-NEXT:                 let %22: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%21), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%7, read<u32>(%22));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(64)>(field0(deref(read<ptr<@type0>>(%6)))), read<u32>(%7))), reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%8), mul<u32, overflow=wrap>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2)))))))), const<i32>(8)), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%8), add<u32, overflow=wrap>(mul<u32, overflow=wrap>(read<u32>(%7), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 i: u32 [storage=automatic];
// DEFAULT-NEXT:         let %11 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         if ne<u64>(const<u64>(2), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         for %14
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%10, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %23: u32 [synthetic] = read<u32>(%10);
// DEFAULT-NEXT:                 let %24: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%23), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%10, read<u32>(%24));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(64)>(field0(%11)), read<u32>(%10))), truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%10), shl<u32, overflow=wrap, amount_out_of_range=ub>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)), read<u32>(%10)), const<i32>(8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%1, addr_of<ptr<@type0>>(%11));
// DEFAULT-NEXT:         for %15
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%10, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %25: u32 [synthetic] = read<u32>(%10);
// DEFAULT-NEXT:                 let %26: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%25), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%10, read<u32>(%26));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(64)>(field0(%11)), read<u32>(%10))))))), add<u32, overflow=wrap>(read<u32>(%10), shl<u32, overflow=wrap, amount_out_of_range=ub>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)), read<u32>(%10)), const<i32>(8))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         for %17
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%10, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %27: u32 [synthetic] = read<u32>(%10);
// DEFAULT-NEXT:                 let %28: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%27), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%10, read<u32>(%28));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(64)>(field0(%11)), read<u32>(%10))), truncate<u16, reason=assign, fits=unknown>(add<u32, overflow=wrap>(read<u32>(%10), shl<u32, overflow=wrap, amount_out_of_range=ub>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)), read<u32>(%10)), const<i32>(8)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%5, addr_of<ptr<@type0>>(%11));
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%10, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%10), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %29: u32 [synthetic] = read<u32>(%10);
// DEFAULT-NEXT:                 let %30: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%29), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%10, read<u32>(%30));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<u32>(reinterpret<u32, reason=usual_arith, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(64)>(field0(%11)), read<u32>(%10))))))), add<u32, overflow=wrap>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(64)), read<u32>(%10)), shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%10), const<i32>(8))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
