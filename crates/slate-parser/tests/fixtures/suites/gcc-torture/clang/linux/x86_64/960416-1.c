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
// DEFAULT-NEXT:     type @type[[TYPE_st:[0-9]+]] st = u64;
// DEFAULT-NEXT:     type @type[[TYPE_dt:[0-9]+]] dt = u64;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 d: u64 [c="dt"];
// DEFAULT-NEXT:         field1 s: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = struct {
// DEFAULT-NEXT:         field0 h: u64 [c="st"];
// DEFAULT-NEXT:         field1 l: u64 [c="st"];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_t_be:[0-9]+]] t_be = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 d: u64 [c="dt"];
// DEFAULT-NEXT:         field1 s: @type[[TYPE3:[0-9]+]];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE3]] = struct {
// DEFAULT-NEXT:         field0 l: u64 [c="st"];
// DEFAULT-NEXT:         field1 h: u64 [c="st"];
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_t_le:[0-9]+]] t_le = @type[[TYPE2]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f_le:[0-9]+]] @f_le(%[[VALUE_afh:[0-9]+]] afh: @type[[TYPE2]] [c="t_le"], %[[VALUE_bfh:[0-9]+]] bfh: @type[[TYPE2]] [c="t_le"]) -> i32 [linkage=external] [abi=sysv64(native_c, native_c) -> scalar] [fallthrough=ub_if_used] [c="int(t_le, t_le)"] {
// DEFAULT-NEXT:         let %[[VALUE_hh:[0-9]+]] hh: @type[[TYPE2]] [storage=automatic] [c="t_le"];
// DEFAULT-NEXT:         let %[[VALUE_hp:[0-9]+]] hp: @type[[TYPE2]] [storage=automatic] [c="t_le"];
// DEFAULT-NEXT:         let %[[VALUE_lp:[0-9]+]] lp: @type[[TYPE2]] [storage=automatic] [c="t_le"];
// DEFAULT-NEXT:         let %[[VALUE_dp:[0-9]+]] dp: @type[[TYPE2]] [storage=automatic] [c="t_le"];
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: @type[[TYPE2]] [storage=automatic] [c="t_le"];
// DEFAULT-NEXT:         let %[[VALUE_ad:[0-9]+]] ad: u64 [storage=automatic] [c="st"];
// DEFAULT-NEXT:         let %[[VALUE_bd:[0-9]+]] bd: u64 [storage=automatic] [c="st"];
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_s]], const<i32>(0));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_ad]], sub<u64, overflow=wrap>(read<u64>(field1(field1(%[[VALUE_afh]]))) [c="st"], read<u64>(field0(field1(%[[VALUE_afh]]))) [c="st"]) [c="st"]) [c="st"];
// DEFAULT-NEXT:         write<u64>(%[[VALUE_bd]], sub<u64, overflow=wrap>(read<u64>(field0(field1(%[[VALUE_bfh]]))) [c="st"], read<u64>(field1(field1(%[[VALUE_bfh]]))) [c="st"]) [c="st"]) [c="st"];
// DEFAULT-NEXT:         if gt<u64>(read<u64>(%[[VALUE_bd]]) [c="st"], read<u64>(field0(field1(%[[VALUE_bfh]]))) [c="st"])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_bd]], neg<u64, overflow=wrap>(read<u64>(%[[VALUE_bd]]) [c="st"]) [c="st"]) [c="st"];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_s]], not<i32>(read<i32>(%[[VALUE_s]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_lp]]), mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit>(read<u64>(field0(field1(%[[VALUE_afh]]))) [c="st"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field0(field1(%[[VALUE_bfh]]))) [c="st"]) [c="dt"]) [c="dt"]) [c="dt"];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_hp]]), mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit>(read<u64>(field1(field1(%[[VALUE_afh]]))) [c="st"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field1(field1(%[[VALUE_bfh]]))) [c="st"]) [c="dt"]) [c="dt"]) [c="dt"];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_dp]]), mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit>(read<u64>(%[[VALUE_ad]]) [c="st"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(%[[VALUE_bd]]) [c="st"]) [c="dt"]) [c="dt"]) [c="dt"];
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(field0(%[[VALUE_dp]])) [c="dt"] [c="dt"];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = xor<u64>(read<u64>(%[[VALUE1]]) [c="dt"], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_s]]))) [c="dt"]) [c="dt"] [c="dt"];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_dp]]), read<u64>(%[[VALUE2]]) [c="dt"]) [c="dt"] [c="dt"];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_hh]]), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(read<u64>(field0(%[[VALUE_hp]])) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field1(field1(%[[VALUE_hp]]))) [c="st"]) [c="dt"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field1(field1(%[[VALUE_lp]]))) [c="st"]) [c="dt"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field1(field1(%[[VALUE_dp]]))) [c="st"]) [c="dt"]) [c="dt"]) [c="dt"];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_m]]), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=explicit>(read<u64>(field1(field1(%[[VALUE_lp]]))) [c="st"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field0(field1(%[[VALUE_hp]]))) [c="st"]) [c="dt"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field0(field1(%[[VALUE_lp]]))) [c="st"]) [c="dt"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field0(field1(%[[VALUE_dp]]))) [c="st"]) [c="dt"]) [c="dt"]) [c="dt"];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(read<u64>(field0(field1(%[[VALUE_hh]]))) [c="st"], read<u64>(field0(field1(%[[VALUE_m]]))) [c="st"]) [c="st"]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f_be:[0-9]+]] @f_be(%[[VALUE_afh_2:[0-9]+]] afh: @type[[TYPE0]] [c="t_be"], %[[VALUE_bfh_2:[0-9]+]] bfh: @type[[TYPE0]] [c="t_be"]) -> i32 [linkage=external] [abi=sysv64(native_c, native_c) -> scalar] [fallthrough=ub_if_used] [c="int(t_be, t_be)"] {
// DEFAULT-NEXT:         let %[[VALUE_hh_2:[0-9]+]] hh: @type[[TYPE0]] [storage=automatic] [c="t_be"];
// DEFAULT-NEXT:         let %[[VALUE_hp_2:[0-9]+]] hp: @type[[TYPE0]] [storage=automatic] [c="t_be"];
// DEFAULT-NEXT:         let %[[VALUE_lp_2:[0-9]+]] lp: @type[[TYPE0]] [storage=automatic] [c="t_be"];
// DEFAULT-NEXT:         let %[[VALUE_dp_2:[0-9]+]] dp: @type[[TYPE0]] [storage=automatic] [c="t_be"];
// DEFAULT-NEXT:         let %[[VALUE_m_2:[0-9]+]] m: @type[[TYPE0]] [storage=automatic] [c="t_be"];
// DEFAULT-NEXT:         let %[[VALUE_ad_2:[0-9]+]] ad: u64 [storage=automatic] [c="st"];
// DEFAULT-NEXT:         let %[[VALUE_bd_2:[0-9]+]] bd: u64 [storage=automatic] [c="st"];
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_s_2]], const<i32>(0));
// DEFAULT-NEXT:         write<u64>(%[[VALUE_ad_2]], sub<u64, overflow=wrap>(read<u64>(field0(field1(%[[VALUE_afh_2]]))) [c="st"], read<u64>(field1(field1(%[[VALUE_afh_2]]))) [c="st"]) [c="st"]) [c="st"];
// DEFAULT-NEXT:         write<u64>(%[[VALUE_bd_2]], sub<u64, overflow=wrap>(read<u64>(field1(field1(%[[VALUE_bfh_2]]))) [c="st"], read<u64>(field0(field1(%[[VALUE_bfh_2]]))) [c="st"]) [c="st"]) [c="st"];
// DEFAULT-NEXT:         if gt<u64>(read<u64>(%[[VALUE_bd_2]]) [c="st"], read<u64>(field1(field1(%[[VALUE_bfh_2]]))) [c="st"])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_bd_2]], neg<u64, overflow=wrap>(read<u64>(%[[VALUE_bd_2]]) [c="st"]) [c="st"]) [c="st"];
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_s_2]], not<i32>(read<i32>(%[[VALUE_s_2]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_lp_2]]), mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit>(read<u64>(field1(field1(%[[VALUE_afh_2]]))) [c="st"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field1(field1(%[[VALUE_bfh_2]]))) [c="st"]) [c="dt"]) [c="dt"]) [c="dt"];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_hp_2]]), mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit>(read<u64>(field0(field1(%[[VALUE_afh_2]]))) [c="st"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field0(field1(%[[VALUE_bfh_2]]))) [c="st"]) [c="dt"]) [c="dt"]) [c="dt"];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_dp_2]]), mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit>(read<u64>(%[[VALUE_ad_2]]) [c="st"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(%[[VALUE_bd_2]]) [c="st"]) [c="dt"]) [c="dt"]) [c="dt"];
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = read<u64>(field0(%[[VALUE_dp_2]])) [c="dt"] [c="dt"];
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: u64 [synthetic] = xor<u64>(read<u64>(%[[VALUE3]]) [c="dt"], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_s_2]]))) [c="dt"]) [c="dt"] [c="dt"];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_dp_2]]), read<u64>(%[[VALUE4]]) [c="dt"]) [c="dt"] [c="dt"];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_hh_2]]), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(read<u64>(field0(%[[VALUE_hp_2]])) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field0(field1(%[[VALUE_hp_2]]))) [c="st"]) [c="dt"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field0(field1(%[[VALUE_lp_2]]))) [c="st"]) [c="dt"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field0(field1(%[[VALUE_dp_2]]))) [c="st"]) [c="dt"]) [c="dt"]) [c="dt"];
// DEFAULT-NEXT:         write<u64>(field0(%[[VALUE_m_2]]), add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(reinterpret<u64, reason=explicit>(read<u64>(field0(field1(%[[VALUE_lp_2]]))) [c="st"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field1(field1(%[[VALUE_hp_2]]))) [c="st"]) [c="dt"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field1(field1(%[[VALUE_lp_2]]))) [c="st"]) [c="dt"]) [c="dt"], reinterpret<u64, reason=usual_arith>(read<u64>(field1(field1(%[[VALUE_dp_2]]))) [c="st"]) [c="dt"]) [c="dt"]) [c="dt"];
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(read<u64>(field1(field1(%[[VALUE_hh_2]]))) [c="st"], read<u64>(field1(field1(%[[VALUE_m_2]]))) [c="st"]) [c="st"]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: @type[[TYPE0]] [storage=automatic] [c="t_be"];
// DEFAULT-NEXT:         write<u64>(field0(field1(%[[VALUE_x]])), widen<u64, reason=assign>(const<u32>(268435456)) [c="st"]) [c="st"];
// DEFAULT-NEXT:         write<u64>(field1(field1(%[[VALUE_x]])), widen<u64, reason=assign>(const<u32>(3758096384)) [c="st"]) [c="st"];
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(field0(%[[VALUE_x]])) [c="dt"], reinterpret<u64, reason=usual_arith>(const<u64>(1152921508364943360)) [c="dt"])
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], ne<i32>(call<i32, signature=fn(@type[[TYPE0]], @type[[TYPE0]]) -> i32, abi=sysv64(native_c, native_c) -> scalar>(%[[VALUE_f_be]], copy<@type[[TYPE0]], reason=arg>(aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<u64>(4294967296)) [c="t_be"]) [c="t_be"], copy<@type[[TYPE0]], reason=arg>(aggregate<@type[[TYPE0]], zero_fill=false>(field0 = const<u64>(4294967296)) [c="t_be"]) [c="t_be"]), neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE5]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if eq<u64>(read<u64>(field0(%[[VALUE_x]])) [c="dt"], reinterpret<u64, reason=usual_arith>(const<u64>(16140901064764293120)) [c="dt"])
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], ne<i32>(call<i32, signature=fn(@type[[TYPE2]], @type[[TYPE2]]) -> i32, abi=sysv64(native_c, native_c) -> scalar>(%[[VALUE_f_le]], copy<@type[[TYPE2]], reason=arg>(aggregate<@type[[TYPE2]], zero_fill=false>(field0 = const<u64>(4294967296)) [c="t_le"]) [c="t_le"], copy<@type[[TYPE2]], reason=arg>(aggregate<@type[[TYPE2]], zero_fill=false>(field0 = const<u64>(4294967296)) [c="t_le"]) [c="t_le"]), neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE6]], const<bool>(false));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE6]])
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
