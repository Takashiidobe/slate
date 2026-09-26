void abort(void);
void exit(int);

typedef unsigned long int  st;
typedef unsigned long long dt;
typedef union {
  dt d;
  struct {
    st h, l;
  } s;
} t_be;

typedef union {
  dt d;
  struct {
    st l, h;
  } s;
} t_le;

#define df(f, t)                                                               \
  int f(t afh, t bfh) {                                                        \
    t   hh;                                                                    \
    t   hp, lp, dp, m;                                                         \
    st  ad, bd;                                                                \
    int s;                                                                     \
    s  = 0;                                                                    \
    ad = afh.s.h - afh.s.l;                                                    \
    bd = bfh.s.l - bfh.s.h;                                                    \
    if (bd > bfh.s.l) {                                                        \
      bd = -bd;                                                                \
      s  = ~s;                                                                 \
    }                                                                          \
    lp.d  = (dt)afh.s.l * bfh.s.l;                                             \
    hp.d  = (dt)afh.s.h * bfh.s.h;                                             \
    dp.d  = (dt)ad * bd;                                                       \
    dp.d ^= s;                                                                 \
    hh.d  = hp.d + hp.s.h + lp.s.h + dp.s.h;                                   \
    m.d   = (dt)lp.s.h + hp.s.l + lp.s.l + dp.s.l;                             \
    return hh.s.l + m.s.l;                                                     \
  }

df(f_le, t_le) df(f_be, t_be)

    int main(void) {
  t_be x;
  x.s.h = 0x10000000U;
  x.s.l = 0xe0000000U;
  if (x.d == 0x10000000e0000000ULL &&
      f_be((t_be)0x100000000ULL, (t_be)0x100000000ULL) != -1)
    abort();
  if (x.d == 0xe000000010000000ULL &&
      f_le((t_le)0x100000000ULL, (t_le)0x100000000ULL) != -1)
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
// DEFAULT-NEXT:     type @type0 st = u64;
// DEFAULT-NEXT:     type @type1 dt = u64;
// DEFAULT-NEXT:     type @type2 = union {
// DEFAULT-NEXT:         field0 d: u64;
// DEFAULT-NEXT:         field1 s: @type3;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 h: u64;
// DEFAULT-NEXT:         field1 l: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type4 t_be = @type2;
// DEFAULT-NEXT:     type @type5 = union {
// DEFAULT-NEXT:         field0 d: u64;
// DEFAULT-NEXT:         field1 s: @type6;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type6 = struct {
// DEFAULT-NEXT:         field0 l: u64;
// DEFAULT-NEXT:         field1 h: u64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type7 t_le = @type5;
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%34 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %10 @f_le(%11 afh: @type5, %12 bfh: @type5) -> i32 [linkage=external] [abi=sysv64(native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %13 hh: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %14 hp: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %15 lp: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %16 dp: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %17 m: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %18 ad: u64 [storage=automatic];
// DEFAULT-NEXT:         let %19 bd: u64 [storage=automatic];
// DEFAULT-NEXT:         let %20 s: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%20, const<i32>(0));
// DEFAULT-NEXT:         write<u64>(%18, sub<u64, overflow=wrap>(read<u64>(field1(field1(%11))), read<u64>(field0(field1(%11)))));
// DEFAULT-NEXT:         write<u64>(%19, sub<u64, overflow=wrap>(read<u64>(field0(field1(%12))), read<u64>(field1(field1(%12)))));
// DEFAULT-NEXT:         if gt<u64>(read<u64>(%19), read<u64>(field0(field1(%12))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%19, neg<u64, overflow=wrap>(read<u64>(%19)));
// DEFAULT-NEXT:                 write<i32>(%20, not<i32>(read<i32>(%20)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u64>(field0(%15), mul<u64, overflow=wrap>(read<u64>(field0(field1(%11))), read<u64>(field0(field1(%12)))));
// DEFAULT-NEXT:         write<u64>(field0(%14), mul<u64, overflow=wrap>(read<u64>(field1(field1(%11))), read<u64>(field1(field1(%12)))));
// DEFAULT-NEXT:         write<u64>(field0(%16), mul<u64, overflow=wrap>(read<u64>(%18), read<u64>(%19)));
// DEFAULT-NEXT:         let %35: u64 [synthetic] = read<u64>(field0(%16));
// DEFAULT-NEXT:         let %36: u64 [synthetic] = xor<u64>(read<u64>(%35), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%20))));
// DEFAULT-NEXT:         write<u64>(field0(%16), read<u64>(%36));
// DEFAULT-NEXT:         write<u64>(field0(%13), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(read<u64>(field0(%14)), read<u64>(field1(field1(%14)))), read<u64>(field1(field1(%15)))), read<u64>(field1(field1(%16)))));
// DEFAULT-NEXT:         write<u64>(field0(%17), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(read<u64>(field1(field1(%15))), read<u64>(field0(field1(%14)))), read<u64>(field0(field1(%15)))), read<u64>(field0(field1(%16)))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(read<u64>(field0(field1(%13))), read<u64>(field0(field1(%17))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @f_be(%22 afh: @type2, %23 bfh: @type2) -> i32 [linkage=external] [abi=sysv64(native_c, native_c) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24 hh: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %25 hp: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %26 lp: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %27 dp: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %28 m: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %29 ad: u64 [storage=automatic];
// DEFAULT-NEXT:         let %30 bd: u64 [storage=automatic];
// DEFAULT-NEXT:         let %31 s: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%31, const<i32>(0));
// DEFAULT-NEXT:         write<u64>(%29, sub<u64, overflow=wrap>(read<u64>(field0(field1(%22))), read<u64>(field1(field1(%22)))));
// DEFAULT-NEXT:         write<u64>(%30, sub<u64, overflow=wrap>(read<u64>(field1(field1(%23))), read<u64>(field0(field1(%23)))));
// DEFAULT-NEXT:         if gt<u64>(read<u64>(%30), read<u64>(field1(field1(%23))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%30, neg<u64, overflow=wrap>(read<u64>(%30)));
// DEFAULT-NEXT:                 write<i32>(%31, not<i32>(read<i32>(%31)));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u64>(field0(%26), mul<u64, overflow=wrap>(read<u64>(field1(field1(%22))), read<u64>(field1(field1(%23)))));
// DEFAULT-NEXT:         write<u64>(field0(%25), mul<u64, overflow=wrap>(read<u64>(field0(field1(%22))), read<u64>(field0(field1(%23)))));
// DEFAULT-NEXT:         write<u64>(field0(%27), mul<u64, overflow=wrap>(read<u64>(%29), read<u64>(%30)));
// DEFAULT-NEXT:         let %37: u64 [synthetic] = read<u64>(field0(%27));
// DEFAULT-NEXT:         let %38: u64 [synthetic] = xor<u64>(read<u64>(%37), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%31))));
// DEFAULT-NEXT:         write<u64>(field0(%27), read<u64>(%38));
// DEFAULT-NEXT:         write<u64>(field0(%24), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(read<u64>(field0(%25)), read<u64>(field0(field1(%25)))), read<u64>(field0(field1(%26)))), read<u64>(field0(field1(%27)))));
// DEFAULT-NEXT:         write<u64>(field0(%28), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(read<u64>(field0(field1(%26))), read<u64>(field1(field1(%25)))), read<u64>(field1(field1(%26)))), read<u64>(field1(field1(%27)))));
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(read<u64>(field1(field1(%24))), read<u64>(field1(field1(%28))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %33 x: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<u64>(field0(field1(%33)), widen<u64, reason=assign>(const<u32>(268435456)));
// DEFAULT-NEXT:         write<u64>(field1(field1(%33)), widen<u64, reason=assign>(const<u32>(3758096384)));
// DEFAULT-NEXT:         let %39: bool [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(field0(%33)), const<u64>(1152921508364943360))
// DEFAULT-NEXT:             write<bool>(%39, ne<i32>(call<i32, signature=fn(@type2, @type2) -> i32, abi=sysv64(native_c, native_c) -> scalar>(%21, copy<@type2, reason=arg>(aggregate<@type2, zero_fill=false>(field0 = const<u64>(4294967296))), copy<@type2, reason=arg>(aggregate<@type2, zero_fill=false>(field0 = const<u64>(4294967296)))), neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%39, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%39)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         let %40: bool [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(field0(%33)), const<u64>(16140901064764293120))
// DEFAULT-NEXT:             write<bool>(%40, ne<i32>(call<i32, signature=fn(@type5, @type5) -> i32, abi=sysv64(native_c, native_c) -> scalar>(%10, copy<@type5, reason=arg>(aggregate<@type5, zero_fill=false>(field0 = const<u64>(4294967296))), copy<@type5, reason=arg>(aggregate<@type5, zero_fill=false>(field0 = const<u64>(4294967296)))), neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%40, const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%40)
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
