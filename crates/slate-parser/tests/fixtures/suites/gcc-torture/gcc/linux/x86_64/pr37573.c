/* PR tree-optimization/37573 */
/* { dg-require-effective-target int32plus } */

struct S {
  unsigned int *a;
  unsigned int  b;
  unsigned int  c[624];
};

static unsigned char __attribute__((noinline)) foo(struct S *s) {
  unsigned int r;
  if (!--s->b) {
    unsigned int *c = s->c;
    unsigned int  i;
    s->a = c;
    for (i = 0; i < 227; i++)
      c[i] = ((((c[i] ^ c[i + 1]) & 0x7ffffffe) ^ c[i]) >> 1) ^
             ((0 - (c[i + 1] & 1)) & 0x9908b0df) ^ c[i + 397];
  }
  r  = *(s->a++);
  r ^= (r >> 11);
  r ^= ((r & 0xff3a58ad) << 7);
  r ^= ((r & 0xffffdf8c) << 15);
  r ^= (r >> 18);
  return (unsigned char)(r >> 1);
}

static void __attribute__((noinline)) bar(unsigned char *p, unsigned int q,
                                          unsigned int r) {
  struct S      s;
  unsigned int  i;
  unsigned int *c = s.c;
  *c              = r;
  for (i = 1; i < 624; i++)
    c[i] = i + 0x6c078965 * ((c[i - 1] >> 30) ^ c[i - 1]);
  s.b = 1;
  while (q--)
    *p++ ^= foo(&s);
};

static unsigned char p[23] = {0xc0, 0x49, 0x17, 0x32, 0x62, 0x1e, 0x2e, 0xd5,
                              0x4c, 0x19, 0x28, 0x49, 0x91, 0xe4, 0x72, 0x83,
                              0x91, 0x3d, 0x93, 0x83, 0xb3, 0x61, 0x38};

static unsigned char q[23] = {0x3e, 0x41, 0x55, 0x54, 0x4f, 0x49, 0x54, 0x20,
                              0x55, 0x4e, 0x49, 0x43, 0x4f, 0x44, 0x45, 0x20,
                              0x53, 0x43, 0x52, 0x49, 0x50, 0x54, 0x3c};

int main(void) {
  unsigned int s;
  s = 23;
  bar(p, s, s + 0xa25e);
  if (__builtin_memcmp(p, q, s) != 0)
    __builtin_abort();
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
// DEFAULT-NEXT:         field0 a: ptr<u32>;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: array<u32, 624>;
// DEFAULT-NEXT:     } [size=2512, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     global %13 p: array<u8, 23> [storage=static] [align=16] = aggregate<array<u8, 23>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(192))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(73))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(23))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(50))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(30))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(46))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(213))), index8 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(76))), index9 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(25))), index10 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(40))), index11 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(73))), index12 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(145))), index13 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(228))), index14 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(114))), index15 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(131))), index16 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(145))), index17 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(61))), index18 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(147))), index19 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(131))), index20 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(179))), index21 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))), index22 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(56)))) [linkage=internal];
// DEFAULT-NEXT:     global %14 q: array<u8, 23> [storage=static] [align=16] = aggregate<array<u8, 23>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(62))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(65))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(85))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(84))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(79))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(73))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(84))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(32))), index8 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(85))), index9 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(78))), index10 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(73))), index11 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(67))), index12 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(79))), index13 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(68))), index14 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(69))), index15 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(32))), index16 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(83))), index17 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(67))), index18 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(82))), index19 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(73))), index20 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(80))), index21 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(84))), index22 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @foo(%2 s: ptr<@type0>) -> u8 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %25: ptr<@type0> [synthetic] = read<ptr<@type0>>(%2);
// DEFAULT-NEXT:         let %26: u32 [synthetic] = read<u32>(field1(deref(read<ptr<@type0>>(%25))));
// DEFAULT-NEXT:         let %27: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%26), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(field1(deref(read<ptr<@type0>>(%25))), read<u32>(%27));
// DEFAULT-NEXT:         if not<bool>(ne<u32>(read<u32>(%27), const<u32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %4 c: ptr<u32> [storage=automatic] = array_decay<ptr<u32>, length=Some(624)>(field2(deref(read<ptr<@type0>>(%2))));
// DEFAULT-NEXT:                 let %5 i: u32 [storage=automatic];
// DEFAULT-NEXT:                 write<ptr<u32>>(field0(deref(read<ptr<@type0>>(%2))), read<ptr<u32>>(%4));
// DEFAULT-NEXT:                 for %17
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u32>(%5, reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     condition: lt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(227)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %28: u32 [synthetic] = read<u32>(%5);
// DEFAULT-NEXT:                         let %29: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%28), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(%5, read<u32>(%29));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%4), read<u32>(%5))), xor<u32>(xor<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(xor<u32>(and<u32>(xor<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%4), read<u32>(%5)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%4), add<u32, overflow=wrap>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483646))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%4), read<u32>(%5))))), const<i32>(1)), and<u32>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), and<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%4), add<u32, overflow=wrap>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<u32>(2567483615))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%4), add<u32, overflow=wrap>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(397))))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %30: ptr<@type0> [synthetic] = read<ptr<@type0>>(%2);
// DEFAULT-NEXT:         let %31: ptr<u32> [synthetic] = read<ptr<u32>>(field0(deref(read<ptr<@type0>>(%30))));
// DEFAULT-NEXT:         let %32: ptr<u32> [synthetic] = ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%31), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<u32>>(field0(deref(read<ptr<@type0>>(%30))), read<ptr<u32>>(%32));
// DEFAULT-NEXT:         write<u32>(%3, read<u32>(deref(read<ptr<u32>>(%31))));
// DEFAULT-NEXT:         let %33: u32 [synthetic] = read<u32>(%3);
// DEFAULT-NEXT:         let %34: u32 [synthetic] = xor<u32>(read<u32>(%33), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%3), const<i32>(11)));
// DEFAULT-NEXT:         write<u32>(%3, read<u32>(%34));
// DEFAULT-NEXT:         let %35: u32 [synthetic] = read<u32>(%3);
// DEFAULT-NEXT:         let %36: u32 [synthetic] = xor<u32>(read<u32>(%35), shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(%3), const<u32>(4282013869)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(%3, read<u32>(%36));
// DEFAULT-NEXT:         let %37: u32 [synthetic] = read<u32>(%3);
// DEFAULT-NEXT:         let %38: u32 [synthetic] = xor<u32>(read<u32>(%37), shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(%3), const<u32>(4294958988)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(%3, read<u32>(%38));
// DEFAULT-NEXT:         let %39: u32 [synthetic] = read<u32>(%3);
// DEFAULT-NEXT:         let %40: u32 [synthetic] = xor<u32>(read<u32>(%39), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%3), const<i32>(18)));
// DEFAULT-NEXT:         write<u32>(%3, read<u32>(%40));
// DEFAULT-NEXT:         return truncate<u8, reason=explicit, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%3), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @bar(%7 p: ptr<u8>, %8 q: u32, %9 r: u32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %11 i: u32 [storage=automatic];
// DEFAULT-NEXT:         let %12 c: ptr<u32> [storage=automatic] = array_decay<ptr<u32>, length=Some(624)>(field2(%10));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%12)), read<u32>(%9));
// DEFAULT-NEXT:         for %18
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%11, reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(624)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %41: u32 [synthetic] = read<u32>(%11);
// DEFAULT-NEXT:                 let %42: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%41), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%11, read<u32>(%42));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%12), read<u32>(%11))), add<u32, overflow=wrap>(read<u32>(%11), mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1812433253)), xor<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%12), sub<u32, overflow=wrap>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))), const<i32>(30)), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%12), sub<u32, overflow=wrap>(read<u32>(%11), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))))))));
// DEFAULT-NEXT:         write<u32>(field1(%10), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         while %19 {
// DEFAULT-NEXT:             let %43: u32 [synthetic] = read<u32>(%8);
// DEFAULT-NEXT:             let %44: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%43), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u32>(%8, read<u32>(%44));
// DEFAULT-NEXT:             yield ne<u32>(read<u32>(%43), const<u32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             let %45: ptr<u8> [synthetic] = read<ptr<u8>>(%7);
// DEFAULT-NEXT:             let %46: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%45), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<u8>>(%7, read<ptr<u8>>(%46));
// DEFAULT-NEXT:             let %47: ptr<u8> [synthetic] = read<ptr<u8>>(%45);
// DEFAULT-NEXT:             let %48: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%47)));
// DEFAULT-NEXT:             let %49: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%48))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(ptr<@type0>) -> u8>(%1, addr_of<ptr<@type0>>(%10)))))));
// DEFAULT-NEXT:             write<u8>(deref(read<ptr<u8>>(%47)), read<u8>(%49));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @__builtin_memcmp(%20 <unnamed>: ptr<const void>, %21 <unnamed>: ptr<const void>, %22 <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %24 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16 s: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%16, reinterpret<u32, reason=assign, fits=always>(const<i32>(23)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u8>, u32, u32) -> void>(%6, array_decay<ptr<u8>, length=Some(23)>(%13), read<u32>(%16), add<u32, overflow=wrap>(read<u32>(%16), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(41566))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%23, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(23)>(%13)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(23)>(%14)), widen<u64, reason=arg>(read<u32>(%16))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%24);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
