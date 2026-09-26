/* PR tree-optimization/20100
   Pure function being treated as const.
   Author: Hans-Peter Nilsson.  */

static unsigned short g = 0;
static unsigned short p = 0;
unsigned char         e;

static unsigned short next_g(void) { return g == e - 1 ? 0 : g + 1; }

static unsigned short curr_p(void) { return p; }

static unsigned short inc_g(void) { return g = next_g(); }

static unsigned short curr_g(void) { return g; }

static char ring_empty(void) {
  if (curr_p() == curr_g())
    return 1;
  else
    return 0;
}

char frob(unsigned short a, unsigned short b) {
  g = a;
  p = b;
  inc_g();
  return ring_empty();
}

unsigned short get_n(void) {
  unsigned short n = 0;
  unsigned short org_g;
  org_g = curr_g();
  while (!ring_empty() && n < 5) {
    inc_g();
    n++;
  }

  return n;
}

void abort(void);
void exit(int);
int  main(void) {
  e = 3;
  if (frob(0, 2) != 0 || g != 1 || p != 2 || e != 3 || get_n() != 1 || g != 2 ||
      p != 2)
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
// DEFAULT-NEXT:     global %0 g: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %1 p: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %2 e: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @next_g() -> u16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(conditional<i32>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%0))), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%2))), const<i32>(1))), const<i32>(0), add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%0))), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @curr_p() -> u16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u16>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @inc_g() -> u16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u16>(%0, call<u16, signature=fn() -> u16>(%3));
// DEFAULT-NEXT:         return call<u16, signature=fn() -> u16>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @curr_g() -> u16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u16>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @ring_empty() -> i8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn() -> u16>(%4))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn() -> u16>(%6))))
// DEFAULT-NEXT:             return truncate<i8, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return truncate<i8, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @frob(%9 a: u16, %10 b: u16) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u16>(%0, read<u16>(%9));
// DEFAULT-NEXT:         write<u16>(%1, read<u16>(%10));
// DEFAULT-NEXT:         call<u16, signature=fn() -> u16>(%5);
// DEFAULT-NEXT:         return call<i8, signature=fn() -> i8>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @get_n() -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 n: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %13 org_g: u16 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(%13, call<u16, signature=fn() -> u16>(%6));
// DEFAULT-NEXT:         call<u16, signature=fn() -> u16>(%6);
// DEFAULT-NEXT:         while %17 logical_and<bool>(not<bool>(ne<i8>(call<i8, signature=fn() -> i8>(%7), const<i8>(0))), lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%12))), const<i32>(5)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<u16, signature=fn() -> u16>(%5);
// DEFAULT-NEXT:                 let %19: u16 [synthetic] = read<u16>(%12);
// DEFAULT-NEXT:                 let %20: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%19))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u16>(%12, read<u16>(%20));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u16>(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %15 @exit(%18 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u8>(%2, reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         let %21: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(u16, u16) -> i8>(%8, reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(2))))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%0))), const<i32>(1))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1))), const<i32>(2))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%2))), const<i32>(3)))
// DEFAULT-NEXT:             write<bool>(%21, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%21, ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn() -> u16>(%11))), const<i32>(1)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%21), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%0))), const<i32>(2))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%1))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%15, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
