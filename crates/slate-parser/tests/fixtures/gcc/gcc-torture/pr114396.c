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
// DEFAULT-NEXT:     global %0 a: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(15)) [linkage=external];
// DEFAULT-NEXT:     global %1 b: array<i16, 16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 ua: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(15))) [linkage=external];
// DEFAULT-NEXT:     global %3 ub: array<u16, 16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo(%5 a: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %33
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %6 e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %41: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %42: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%41), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%42));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %43: i16 [synthetic] = read<i16>(%5);
// DEFAULT-NEXT:                 let %44: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%43)), const<i32>(5)));
// DEFAULT-NEXT:                 write<i16>(%5, read<i16>(%44));
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(16)>(%1), read<i32>(%6))), read<i16>(%44));
// DEFAULT-NEXT:         return read<i16>(%5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @foo1(%8 a: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %34
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %9 e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%9), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %45: i32 [synthetic] = read<i32>(%9);
// DEFAULT-NEXT:                 let %46: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%45), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%9, read<i32>(%46));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %47: i16 [synthetic] = read<i16>(%8);
// DEFAULT-NEXT:                 let %48: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%47)), neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:                 write<i16>(%8, read<i16>(%48));
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(16)>(%1), read<i32>(%9))), read<i16>(%48));
// DEFAULT-NEXT:         return read<i16>(%8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @foou(%11 a: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %35
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %12 e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%12), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %49: i32 [synthetic] = read<i32>(%12);
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%49), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%12, read<i32>(%50));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %51: u16 [synthetic] = read<u16>(%11);
// DEFAULT-NEXT:                 let %52: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%51))), neg<i32, overflow=ub>(const<i32>(5)))));
// DEFAULT-NEXT:                 write<u16>(%11, read<u16>(%52));
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(16)>(%3), read<i32>(%12))), read<u16>(%52));
// DEFAULT-NEXT:         return read<u16>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @foou1(%14 a: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %36
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %15 e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%15), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %53: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                 let %54: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%53), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%15, read<i32>(%54));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %55: u16 [synthetic] = read<u16>(%14);
// DEFAULT-NEXT:                 let %56: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%55))), const<i32>(5))));
// DEFAULT-NEXT:                 write<u16>(%14, read<u16>(%56));
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(16)>(%3), read<i32>(%15))), read<u16>(%56));
// DEFAULT-NEXT:         return read<u16>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @foo_o3(%17 a: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %37
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %18 e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%18), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %57: i32 [synthetic] = read<i32>(%18);
// DEFAULT-NEXT:                 let %58: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%57), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%18, read<i32>(%58));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %59: i16 [synthetic] = read<i16>(%17);
// DEFAULT-NEXT:                 let %60: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%59)), const<i32>(5)));
// DEFAULT-NEXT:                 write<i16>(%17, read<i16>(%60));
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(16)>(%1), read<i32>(%18))), read<i16>(%60));
// DEFAULT-NEXT:         return read<i16>(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @foo1_o3(%20 a: i16) -> i16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %38
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %21 e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%21), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %61: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                 let %62: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%61), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%21, read<i32>(%62));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %63: i16 [synthetic] = read<i16>(%20);
// DEFAULT-NEXT:                 let %64: i16 [synthetic] = truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(widen<i32, reason=promotion>(read<i16>(%63)), neg<i32, overflow=ub>(const<i32>(5))));
// DEFAULT-NEXT:                 write<i16>(%20, read<i16>(%64));
// DEFAULT-NEXT:                 write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(16)>(%1), read<i32>(%21))), read<i16>(%64));
// DEFAULT-NEXT:         return read<i16>(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @foou_o3(%23 a: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %39
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %24 e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%24), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %65: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:                 let %66: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%65), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%24, read<i32>(%66));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %67: u16 [synthetic] = read<u16>(%23);
// DEFAULT-NEXT:                 let %68: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%67))), neg<i32, overflow=ub>(const<i32>(5)))));
// DEFAULT-NEXT:                 write<u16>(%23, read<u16>(%68));
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(16)>(%3), read<i32>(%24))), read<u16>(%68));
// DEFAULT-NEXT:         return read<u16>(%23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @foou1_o3(%26 a: u16) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         for %40
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %27 e: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%27), const<i32>(9))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %69: i32 [synthetic] = read<i32>(%27);
// DEFAULT-NEXT:                 let %70: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%69), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%27, read<i32>(%70));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %71: u16 [synthetic] = read<u16>(%26);
// DEFAULT-NEXT:                 let %72: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(mul<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%71))), const<i32>(5))));
// DEFAULT-NEXT:                 write<u16>(%26, read<u16>(%72));
// DEFAULT-NEXT:                 write<u16>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(array_decay<ptr<u16>, length=Some(16)>(%3), read<i32>(%27))), read<u16>(%72));
// DEFAULT-NEXT:         return read<u16>(%26);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %28 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %29 uexp: u16 [storage=automatic];
// DEFAULT-NEXT:         let %30 ures: u16 [storage=automatic];
// DEFAULT-NEXT:         let %31 exp: i16 [storage=automatic];
// DEFAULT-NEXT:         let %32 res: i16 [storage=automatic];
// DEFAULT-NEXT:         write<i16>(%31, call<i16, signature=fn(i16) -> i16>(%4, read<i16>(%0)));
// DEFAULT-NEXT:         call<i16, signature=fn(i16) -> i16>(%4, read<i16>(%0));
// DEFAULT-NEXT:         write<i16>(%32, call<i16, signature=fn(i16) -> i16>(%16, read<i16>(%0)));
// DEFAULT-NEXT:         call<i16, signature=fn(i16) -> i16>(%16, read<i16>(%0));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%31)), widen<i32, reason=promotion>(read<i16>(%32)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<i16>(%31, call<i16, signature=fn(i16) -> i16>(%7, read<i16>(%0)));
// DEFAULT-NEXT:         call<i16, signature=fn(i16) -> i16>(%7, read<i16>(%0));
// DEFAULT-NEXT:         write<i16>(%32, call<i16, signature=fn(i16) -> i16>(%19, read<i16>(%0)));
// DEFAULT-NEXT:         call<i16, signature=fn(i16) -> i16>(%19, read<i16>(%0));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(%31)), widen<i32, reason=promotion>(read<i16>(%32)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<u16>(%29, call<u16, signature=fn(u16) -> u16>(%10, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%0))));
// DEFAULT-NEXT:         call<u16, signature=fn(u16) -> u16>(%10, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%0)));
// DEFAULT-NEXT:         write<u16>(%30, call<u16, signature=fn(u16) -> u16>(%22, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%0))));
// DEFAULT-NEXT:         call<u16, signature=fn(u16) -> u16>(%22, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%0)));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%29))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%30))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<u16>(%29, call<u16, signature=fn(u16) -> u16>(%13, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%0))));
// DEFAULT-NEXT:         call<u16, signature=fn(u16) -> u16>(%13, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%0)));
// DEFAULT-NEXT:         write<u16>(%30, call<u16, signature=fn(u16) -> u16>(%25, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%0))));
// DEFAULT-NEXT:         call<u16, signature=fn(u16) -> u16>(%25, reinterpret<u16, reason=arg, fits=unknown>(read<i16>(%0)));
// DEFAULT-NEXT:         if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%29))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%30))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
