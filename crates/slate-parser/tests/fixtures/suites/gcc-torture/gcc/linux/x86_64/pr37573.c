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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: ptr<u32>;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: array<u32, 624>;
// DEFAULT-NEXT:     } [size=2512, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: array<u8, 23> [storage=static] [align=16] = aggregate<array<u8, 23>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(192))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(73))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(23))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(50))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(98))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(30))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(46))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(213))), index8 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(76))), index9 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(25))), index10 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(40))), index11 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(73))), index12 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(145))), index13 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(228))), index14 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(114))), index15 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(131))), index16 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(145))), index17 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(61))), index18 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(147))), index19 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(131))), index20 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(const<i32>(179))), index21 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(97))), index22 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(56)))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_q:[0-9]+]] q: array<u8, 23> [storage=static] [align=16] = aggregate<array<u8, 23>, zero_fill=false>(index0 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(62))), index1 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(65))), index2 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(85))), index3 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(84))), index4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(79))), index5 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(73))), index6 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(84))), index7 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(32))), index8 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(85))), index9 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(78))), index10 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(73))), index11 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(67))), index12 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(79))), index13 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(68))), index14 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(69))), index15 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(32))), index16 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(83))), index17 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(67))), index18 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(82))), index19 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(73))), index20 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(80))), index21 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(84))), index22 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(60)))) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_S]]>) -> u8 [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<@type[[TYPE_S]]> [synthetic] = read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u32 [synthetic] = read<u32>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE0]]))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%[[VALUE1]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<u32>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE0]]))), read<u32>(%[[VALUE2]]));
// DEFAULT-NEXT:         if not<bool>(ne<u32>(read<u32>(%[[VALUE2]]), const<u32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_c:[0-9]+]] c: ptr<u32> [storage=automatic] = array_decay<ptr<u32>, length=Some(624)>(field2(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]))));
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: u32 [storage=automatic];
// DEFAULT-NEXT:                 write<ptr<u32>>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]))), read<ptr<u32>>(%[[VALUE_c]]));
// DEFAULT-NEXT:                 for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_i]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                     condition: lt<u32>(read<u32>(%[[VALUE_i]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(227)))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE4:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE4]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                         write<u32>(%[[VALUE_i]], read<u32>(%[[VALUE5]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_c]]), read<u32>(%[[VALUE_i]]))), xor<u32>(xor<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(xor<u32>(and<u32>(xor<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_c]]), read<u32>(%[[VALUE_i]])))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_c]]), add<u32, overflow=wrap>(read<u32>(%[[VALUE_i]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(2147483646))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_c]]), read<u32>(%[[VALUE_i]]))))), const<i32>(1)), and<u32>(sub<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)), and<u32>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_c]]), add<u32, overflow=wrap>(read<u32>(%[[VALUE_i]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))), const<u32>(2567483615))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_c]]), add<u32, overflow=wrap>(read<u32>(%[[VALUE_i]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(397))))))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: ptr<@type[[TYPE_S]]> [synthetic] = read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: ptr<u32> [synthetic] = read<ptr<u32>>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE6]]))));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: ptr<u32> [synthetic] = ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<u32>>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE6]]))), read<ptr<u32>>(%[[VALUE8]]));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r]], read<u32>(deref(read<ptr<u32>>(%[[VALUE7]]))));
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: u32 [synthetic] = xor<u32>(read<u32>(%[[VALUE9]]), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_r]]), const<i32>(11)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r]], read<u32>(%[[VALUE10]]));
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: u32 [synthetic] = xor<u32>(read<u32>(%[[VALUE11]]), shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(%[[VALUE_r]]), const<u32>(4282013869)), const<i32>(7)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r]], read<u32>(%[[VALUE12]]));
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: u32 [synthetic] = xor<u32>(read<u32>(%[[VALUE13]]), shl<u32, overflow=wrap, amount_out_of_range=ub>(and<u32>(read<u32>(%[[VALUE_r]]), const<u32>(4294958988)), const<i32>(15)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r]], read<u32>(%[[VALUE14]]));
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_r]]);
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: u32 [synthetic] = xor<u32>(read<u32>(%[[VALUE15]]), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_r]]), const<i32>(18)));
// DEFAULT-NEXT:         write<u32>(%[[VALUE_r]], read<u32>(%[[VALUE16]]));
// DEFAULT-NEXT:         return truncate<u8, reason=explicit, fits=unknown>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_r]]), const<i32>(1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p_2:[0-9]+]] p: ptr<u8>, %[[VALUE_q_2:[0-9]+]] q: u32, %[[VALUE_r_2:[0-9]+]] r: u32) -> void [linkage=internal] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: ptr<u32> [storage=automatic] = array_decay<ptr<u32>, length=Some(624)>(field2(%[[VALUE_s_2]]));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE_c_2]])), read<u32>(%[[VALUE_r_2]]));
// DEFAULT-NEXT:         for %[[VALUE17:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(624)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE18]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], read<u32>(%[[VALUE19]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_c_2]]), read<u32>(%[[VALUE_i_2]]))), add<u32, overflow=wrap>(read<u32>(%[[VALUE_i_2]]), mul<u32, overflow=wrap>(reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1812433253)), xor<u32>(shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_c_2]]), sub<u32, overflow=wrap>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))))), const<i32>(30)), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%[[VALUE_c_2]]), sub<u32, overflow=wrap>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1))))))))));
// DEFAULT-NEXT:         write<u32>(field1(%[[VALUE_s_2]]), reinterpret<u32, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         while %[[VALUE20:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE21:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_q_2]]);
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: u32 [synthetic] = sub<u32, overflow=wrap>(read<u32>(%[[VALUE21]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:             write<u32>(%[[VALUE_q_2]], read<u32>(%[[VALUE22]]));
// DEFAULT-NEXT:             yield ne<u32>(read<u32>(%[[VALUE21]]), const<u32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE_p_2]]);
// DEFAULT-NEXT:             let %[[VALUE24:[0-9]+]]: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%[[VALUE23]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<u8>>(%[[VALUE_p_2]], read<ptr<u8>>(%[[VALUE24]]));
// DEFAULT-NEXT:             let %[[VALUE25:[0-9]+]]: ptr<u8> [synthetic] = read<ptr<u8>>(%[[VALUE23]]);
// DEFAULT-NEXT:             let %[[VALUE26:[0-9]+]]: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%[[VALUE25]])));
// DEFAULT-NEXT:             let %[[VALUE27:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(xor<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE26]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u8, signature=fn(ptr<@type[[TYPE_S]]>) -> u8>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_S]]>>(%[[VALUE_s_2]])))))));
// DEFAULT-NEXT:             write<u8>(deref(read<ptr<u8>>(%[[VALUE25]])), read<u8>(%[[VALUE27]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memcmp:[0-9]+]] @__builtin_memcmp(%[[VALUE28:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE29:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE30:[0-9]+]] <unnamed>: u64) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_s_3:[0-9]+]] s: u32 [storage=automatic];
// DEFAULT-NEXT:         write<u32>(%[[VALUE_s_3]], reinterpret<u32, reason=assign, fits=always>(const<i32>(23)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<u8>, u32, u32) -> void>(%[[VALUE_bar]], array_decay<ptr<u8>, length=Some(23)>(%[[VALUE_p]]), read<u32>(%[[VALUE_s_3]]), add<u32, overflow=wrap>(read<u32>(%[[VALUE_s_3]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(41566))));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<const void>, ptr<const void>, u64) -> i32>(%[[VALUE___builtin_memcmp]], pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(23)>(%[[VALUE_p]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(23)>(%[[VALUE_q]])), widen<u64, reason=arg>(read<u32>(%[[VALUE_s_3]]))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
