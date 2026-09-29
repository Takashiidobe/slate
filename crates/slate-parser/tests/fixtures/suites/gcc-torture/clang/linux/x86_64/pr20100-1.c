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
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_p:[0-9]+]] p: u16 [storage=static] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: u8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_next_g:[0-9]+]] @next_g() -> u16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<u16, reason=return, fits=unknown>(truncate<i16, reason=return, fits=unknown>(conditional<i32>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_g]]))), sub<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_e]]))), const<i32>(1))), const<i32>(0), add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_g]]))), const<i32>(1)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_curr_p:[0-9]+]] @curr_p() -> u16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u16>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_inc_g:[0-9]+]] @inc_g() -> u16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u16>(%[[VALUE_g]], call<u16, signature=fn() -> u16>(%[[VALUE_next_g]]));
// DEFAULT-NEXT:         return call<u16, signature=fn() -> u16>(%[[VALUE_next_g]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_curr_g:[0-9]+]] @curr_g() -> u16 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u16>(%[[VALUE_g]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_ring_empty:[0-9]+]] @ring_empty() -> i8 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn() -> u16>(%[[VALUE_curr_p]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn() -> u16>(%[[VALUE_curr_g]]))))
// DEFAULT-NEXT:             return truncate<i8, reason=return, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return truncate<i8, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_frob:[0-9]+]] @frob(%[[VALUE_a:[0-9]+]] a: u16, %[[VALUE_b:[0-9]+]] b: u16) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u16>(%[[VALUE_g]], read<u16>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<u16>(%[[VALUE_p]], read<u16>(%[[VALUE_b]]));
// DEFAULT-NEXT:         call<u16, signature=fn() -> u16>(%[[VALUE_inc_g]]);
// DEFAULT-NEXT:         return call<i8, signature=fn() -> i8>(%[[VALUE_ring_empty]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_get_n:[0-9]+]] @get_n() -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: u16 [storage=automatic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_org_g:[0-9]+]] org_g: u16 [storage=automatic];
// DEFAULT-NEXT:         write<u16>(%[[VALUE_org_g]], call<u16, signature=fn() -> u16>(%[[VALUE_curr_g]]));
// DEFAULT-NEXT:         call<u16, signature=fn() -> u16>(%[[VALUE_curr_g]]);
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] logical_and<bool>(not<bool>(ne<i8>(call<i8, signature=fn() -> i8>(%[[VALUE_ring_empty]]), const<i8>(0))), lt<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_n]]))), const<i32>(5)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<u16, signature=fn() -> u16>(%[[VALUE_inc_g]]);
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u16 [synthetic] = read<u16>(%[[VALUE_n]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u16 [synthetic] = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE1]]))), const<i32>(1))));
// DEFAULT-NEXT:                 write<u16>(%[[VALUE_n]], read<u16>(%[[VALUE2]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return read<u16>(%[[VALUE_n]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE3:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<u8>(%[[VALUE_e]], reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(3))));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(call<i8, signature=fn(u16, u16) -> i8>(%[[VALUE_frob]], reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(0))), reinterpret<u16, reason=arg, fits=unknown>(truncate<i16, reason=arg, fits=always>(const<i32>(2))))), const<i32>(0)), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_g]]))), const<i32>(1))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_p]]))), const<i32>(2))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_e]]))), const<i32>(3)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(call<u16, signature=fn() -> u16>(%[[VALUE_get_n]]))), const<i32>(1)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(read<bool>(%[[VALUE4]]), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_g]]))), const<i32>(2))), ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(%[[VALUE_p]]))), const<i32>(2)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
