/* PR tree-optimization/114396 */
/* { dg-additional-options "-fwrapv -fno-vect-cost-model" } */

short          a = 0xF;
short          b[16];
unsigned short ua = 0xF;
unsigned short ub[16];

short __attribute__((noipa)) foo(short a) {
  for (int e = 0; e < 9; e += 1)
    b[e] = a *= 5;
  return a;
}

short __attribute__((noipa)) foo1(short a) {
  for (int e = 0; e < 9; e += 1)
    b[e] = a *= -5;
  return a;
}

unsigned short __attribute__((noipa)) foou(unsigned short a) {
  for (int e = 0; e < 9; e += 1)
    ub[e] = a *= -5;
  return a;
}

unsigned short __attribute__((noipa)) foou1(unsigned short a) {
  for (int e = 0; e < 9; e += 1)
    ub[e] = a *= 5;
  return a;
}

short __attribute__((noipa, optimize("O3"))) foo_o3(short a) {
  for (int e = 0; e < 9; e += 1)
    b[e] = a *= 5;
  return a;
}

short __attribute__((noipa, optimize("O3"))) foo1_o3(short a) {
  for (int e = 0; e < 9; e += 1)
    b[e] = a *= -5;
  return a;
}

unsigned short __attribute__((noipa, optimize("O3")))
foou_o3(unsigned short a) {
  for (int e = 0; e < 9; e += 1)
    ub[e] = a *= -5;
  return a;
}

unsigned short __attribute__((noipa, optimize("O3")))
foou1_o3(unsigned short a) {
  for (int e = 0; e < 9; e += 1)
    ub[e] = a *= 5;
  return a;
}

int main() {
  unsigned short uexp, ures;
  short          exp, res;
  exp = foo(a);
  res = foo_o3(a);
  if (exp != res)
    __builtin_abort();

  exp = foo1(a);
  res = foo1_o3(a);
  if (exp != res)
    __builtin_abort();

  uexp = foou(a);
  ures = foou_o3(a);
  if (uexp != ures)
    __builtin_abort();

  uexp = foou1(a);
  ures = foou1_o3(a);
  if (uexp != ures)
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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(15)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: array<i16, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ua:[0-9]+]] ua: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(15))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ub:[0-9]+]] ub: array<u16, 16> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_a_2:[0-9]+]] a: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_e:[0-9]+]] e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE3]])), const<i32>(5)));
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_a_2]], read<i16>(%[[VALUE4]]));
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(16)>(%[[VALUE_b]]), read<i32>(%[[VALUE_e]]))), read<i16>(%[[VALUE4]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE_a_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1:[0-9]+]] @foo1(%[[VALUE_a_3:[0-9]+]] a: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_e_2:[0-9]+]] e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e_2]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_a_3]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE8]])), neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_a_3]], read<i16>(%[[VALUE9]]));
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(16)>(%[[VALUE_b]]), read<i32>(%[[VALUE_e_2]]))), read<i16>(%[[VALUE9]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE_a_3]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foou:[0-9]+]] @foou(%[[VALUE_a_4:[0-9]+]] a: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_e_3:[0-9]+]] e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e_3]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e_3]]);
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE11]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e_3]], read<i32>(%[[VALUE12]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_a_4]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE13]]))), neg<i32, overflow=ub>(const<i32>(5)))));
// DEFAULT-NEXT:                 write<u16>(%[[VALUE_a_4]], read<u16>(%[[VALUE14]]));
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(16)>(%[[VALUE_ub]]), read<i32>(%[[VALUE_e_3]]))), read<u16>(%[[VALUE14]]));
// DEFAULT-NEXT:         return read<u16>(%[[VALUE_a_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foou1:[0-9]+]] @foou1(%[[VALUE_a_5:[0-9]+]] a: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_e_4:[0-9]+]] e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e_4]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e_4]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e_4]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_a_5]]);
// DEFAULT-NEXT:                 let %[[VALUE19:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE18]]))), const<i32>(5))));
// DEFAULT-NEXT:                 write<u16>(%[[VALUE_a_5]], read<u16>(%[[VALUE19]]));
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(16)>(%[[VALUE_ub]]), read<i32>(%[[VALUE_e_4]]))), read<u16>(%[[VALUE19]]));
// DEFAULT-NEXT:         return read<u16>(%[[VALUE_a_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo_o3:[0-9]+]] @foo_o3(%[[VALUE_a_6:[0-9]+]] a: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_e_5:[0-9]+]] e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e_5]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e_5]]);
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e_5]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE23:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_a_6]]);
// DEFAULT-NEXT:                 let %[[VALUE24:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE23]])), const<i32>(5)));
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_a_6]], read<i16>(%[[VALUE24]]));
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(16)>(%[[VALUE_b]]), read<i32>(%[[VALUE_e_5]]))), read<i16>(%[[VALUE24]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE_a_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo1_o3:[0-9]+]] @foo1_o3(%[[VALUE_a_7:[0-9]+]] a: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE25:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_e_6:[0-9]+]] e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e_6]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e_6]]);
// DEFAULT-NEXT:                 let %[[VALUE27:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e_6]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE28:[0-9]+]]: i16 [synthetic] = read<i16>(%[[VALUE_a_7]]);
// DEFAULT-NEXT:                 let %[[VALUE29:[0-9]+]]: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%[[VALUE28]])), neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:                 write<i16>(%[[VALUE_a_7]], read<i16>(%[[VALUE29]]));
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(16)>(%[[VALUE_b]]), read<i32>(%[[VALUE_e_6]]))), read<i16>(%[[VALUE29]]));
// DEFAULT-NEXT:         return read<i16>(%[[VALUE_a_7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foou_o3:[0-9]+]] @foou_o3(%[[VALUE_a_8:[0-9]+]] a: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE30:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_e_7:[0-9]+]] e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e_7]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e_7]]);
// DEFAULT-NEXT:                 let %[[VALUE32:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE31]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e_7]], read<i32>(%[[VALUE32]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE33:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_a_8]]);
// DEFAULT-NEXT:                 let %[[VALUE34:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE33]]))), neg<i32, overflow=ub>(const<i32>(5)))));
// DEFAULT-NEXT:                 write<u16>(%[[VALUE_a_8]], read<u16>(%[[VALUE34]]));
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(16)>(%[[VALUE_ub]]), read<i32>(%[[VALUE_e_7]]))), read<u16>(%[[VALUE34]]));
// DEFAULT-NEXT:         return read<u16>(%[[VALUE_a_8]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foou1_o3:[0-9]+]] @foou1_o3(%[[VALUE_a_9:[0-9]+]] a: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %[[VALUE35:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_e_8:[0-9]+]] e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_e_8]]), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE36:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_e_8]]);
// DEFAULT-NEXT:                 let %[[VALUE37:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE36]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_e_8]], read<i32>(%[[VALUE37]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE38:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_a_9]]);
// DEFAULT-NEXT:                 let %[[VALUE39:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE38]]))), const<i32>(5))));
// DEFAULT-NEXT:                 write<u16>(%[[VALUE_a_9]], read<u16>(%[[VALUE39]]));
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(16)>(%[[VALUE_ub]]), read<i32>(%[[VALUE_e_8]]))), read<u16>(%[[VALUE39]]));
// DEFAULT-NEXT:         return read<u16>(%[[VALUE_a_9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_uexp:[0-9]+]] uexp: u16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ures:[0-9]+]] ures: u16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_exp:[0-9]+]] exp: i16 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_res:[0-9]+]] res: i16 [storage=automatic];
// DEFAULT-NEXT:         write<i16>(%[[VALUE_exp]], call<i16, signature=fn(i16) -> i16>(%[[VALUE_foo]], read<i16>(%[[VALUE_a]])));
// DEFAULT-NEXT:         call<i16, signature=fn(i16) -> i16>(%[[VALUE_foo]], read<i16>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], call<i16, signature=fn(i16) -> i16>(%[[VALUE_foo_o3]], read<i16>(%[[VALUE_a]])));
// DEFAULT-NEXT:         call<i16, signature=fn(i16) -> i16>(%[[VALUE_foo_o3]], read<i16>(%[[VALUE_a]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_exp]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_res]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<i16>(%[[VALUE_exp]], call<i16, signature=fn(i16) -> i16>(%[[VALUE_foo1]], read<i16>(%[[VALUE_a]])));
// DEFAULT-NEXT:         call<i16, signature=fn(i16) -> i16>(%[[VALUE_foo1]], read<i16>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<i16>(%[[VALUE_res]], call<i16, signature=fn(i16) -> i16>(%[[VALUE_foo1_o3]], read<i16>(%[[VALUE_a]])));
// DEFAULT-NEXT:         call<i16, signature=fn(i16) -> i16>(%[[VALUE_foo1_o3]], read<i16>(%[[VALUE_a]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%[[VALUE_exp]])), widen<i32, reason=promotion>(read<i16>(%[[VALUE_res]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u16>(%[[VALUE_uexp]], call<u16, signature=fn(u16) -> u16>(%[[VALUE_foou]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         call<u16, signature=fn(u16) -> u16>(%[[VALUE_foou]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_ures]], call<u16, signature=fn(u16) -> u16>(%[[VALUE_foou_o3]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         call<u16, signature=fn(u16) -> u16>(%[[VALUE_foou_o3]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_a]])));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_uexp]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_ures]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<u16>(%[[VALUE_uexp]], call<u16, signature=fn(u16) -> u16>(%[[VALUE_foou1]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         call<u16, signature=fn(u16) -> u16>(%[[VALUE_foou1]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_a]])));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_ures]], call<u16, signature=fn(u16) -> u16>(%[[VALUE_foou1_o3]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         call<u16, signature=fn(u16) -> u16>(%[[VALUE_foou1_o3]], reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%[[VALUE_a]])));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_uexp]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_ures]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
