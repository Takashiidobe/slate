extern void abort(void);
#define A(x)                                                                   \
  if (!(x))                                                                    \
  abort()

static union at6 {
} vv6 = {};
static struct et6 {
  struct bt6 {
    signed        av6 : 6;
    signed        bv6 : 7;
    signed        cv6 : 6;
    signed        dv6 : 5;
    unsigned char ev6;
    unsigned int  fv6;
    long int      gv6;
  } mv6;
  unsigned long int nv6;
  signed            ov6 : 12;
  signed            pv6 : 3;
  signed            qv6 : 2;
  signed            rv6 : 10;
  union ct6 {
    long int hv6;
    float    iv6;
    float    jv6;
  } sv6;
  int *tv6;
  union dt6 {
    double kv6;
    float  lv6;
  } uv6;
} wv6                    = {{8, 9, 2, 4, '\x10', 67426805U, 1047191860L},
                            1366022414UL,
                            858,
                            1,
                            1,
                            305,
                            {1069379046L},
                            (int *)358273621U,
                            {3318.041978}};
static double        xv6 = 19239.101269;
static long long int yv6 = 1207859169L;
static int           zv6 = 660195606;

static union at6 callee_af6(struct et6 ap6, double bp6, long long int cp6,
                            int dp6) {
  A(wv6.mv6.av6 == ap6.mv6.av6);
  A(wv6.mv6.bv6 == ap6.mv6.bv6);
  A(wv6.mv6.cv6 == ap6.mv6.cv6);
  A(wv6.mv6.dv6 == ap6.mv6.dv6);
  A(wv6.mv6.ev6 == ap6.mv6.ev6);
  A(wv6.mv6.fv6 == ap6.mv6.fv6);
  A(wv6.mv6.gv6 == ap6.mv6.gv6);
  A(wv6.nv6 == ap6.nv6);
  A(wv6.ov6 == ap6.ov6);
  A(wv6.pv6 == ap6.pv6);
  A(wv6.qv6 == ap6.qv6);
  A(wv6.rv6 == ap6.rv6);
  A(wv6.sv6.hv6 == ap6.sv6.hv6);
  A(wv6.tv6 == ap6.tv6);
  A(wv6.uv6.kv6 == ap6.uv6.kv6);
  A(xv6 == bp6);
  A(yv6 == cp6);
  A(zv6 == dp6);
  return vv6;
}

static void caller_bf6(void) {
  union at6 bav6;
  bav6 = callee_af6(wv6, xv6, yv6, zv6);
}

static unsigned char uv7 = '\x46';
static float         vv7 = 96636.982442;
static double        wv7 = 28450.711801;
static union ct7 {
} xv7 = {};
static struct et7 {
  struct dt7 {
    float              iv7;
    unsigned short int jv7;
  } kv7;
  float     lv7[0];
  signed    mv7 : 9;
  short int nv7;
  double    ov7;
  float     pv7;
} yv7 = {{30135.996213, 42435}, {}, 170, 22116, 26479.628148, 4082.960685};
static union ft7 {
  float         qv7;
  float        *rv7;
  unsigned int *sv7;
} zv7           = {5042.227886};
static int bav7 = 1345451862;
static struct gt7 {
  double tv7;
} bbv7                       = {47875.491954};
static long int      bcv7[1] = {1732133482L};
static long long int bdv7    = 381678602L;

static unsigned char callee_af7(float ap7, double bp7, union ct7 cp7,
                                struct et7 dp7, union ft7 ep7, int fp7,
                                struct gt7 gp7, long int hp7[1],
                                long long int ip7) {
  A(vv7 == ap7);
  A(wv7 == bp7);
  A(yv7.kv7.iv7 == dp7.kv7.iv7);
  A(yv7.kv7.jv7 == dp7.kv7.jv7);
  A(yv7.mv7 == dp7.mv7);
  A(yv7.nv7 == dp7.nv7);
  A(yv7.ov7 == dp7.ov7);
  A(yv7.pv7 == dp7.pv7);
  A(zv7.qv7 == ep7.qv7);
  A(bav7 == fp7);
  A(bbv7.tv7 == gp7.tv7);
  A(bcv7[0] == hp7[0]);
  A(bdv7 == ip7);
  return uv7;
}

static void caller_bf7(void) {
  unsigned char bev7;

  bev7 = callee_af7(vv7, wv7, xv7, yv7, zv7, bav7, bbv7, bcv7, bdv7);
  A(uv7 == bev7);
}

int main() {
  caller_bf6();
  caller_bf7();
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
// DEFAULT-NEXT:     type @type[[TYPE_at6:[0-9]+]] at6 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_et6:[0-9]+]] et6 = struct {
// DEFAULT-NEXT:         field0 mv6: @type[[TYPE_bt6:[0-9]+]];
// DEFAULT-NEXT:         field1 nv6: u64;
// DEFAULT-NEXT:         field2 ov6: i32 : 12;
// DEFAULT-NEXT:         field3 pv6: i32 : 3;
// DEFAULT-NEXT:         field4 qv6: i32 : 2;
// DEFAULT-NEXT:         field5 rv6: i32 : 10;
// DEFAULT-NEXT:         field6 sv6: @type[[TYPE_ct6:[0-9]+]];
// DEFAULT-NEXT:         field7 tv6: ptr<i32>;
// DEFAULT-NEXT:         field8 uv6: @type[[TYPE_dt6:[0-9]+]];
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 16, 24, 25, 25, 26, 32, 40, 48], bit_offsets=[None, None, Some(192), Some(204), Some(207), Some(209), None, None, None], bit_units=[(24, 4)], field_units=[None, None, Some(0), Some(0), Some(0), Some(0), None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_bt6]] bt6 = struct {
// DEFAULT-NEXT:         field0 av6: i32 : 6;
// DEFAULT-NEXT:         field1 bv6: i32 : 7;
// DEFAULT-NEXT:         field2 cv6: i32 : 6;
// DEFAULT-NEXT:         field3 dv6: i32 : 5;
// DEFAULT-NEXT:         field4 ev6: u8;
// DEFAULT-NEXT:         field5 fv6: u32;
// DEFAULT-NEXT:         field6 gv6: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 1, 2, 3, 4, 8], bit_offsets=[Some(0), Some(6), Some(13), Some(19), None, None, None], bit_units=[(0, 3)], field_units=[Some(0), Some(0), Some(0), Some(0), None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_ct6]] ct6 = union {
// DEFAULT-NEXT:         field0 hv6: i64;
// DEFAULT-NEXT:         field1 iv6: f32;
// DEFAULT-NEXT:         field2 jv6: f32;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_dt6]] dt6 = union {
// DEFAULT-NEXT:         field0 kv6: f64;
// DEFAULT-NEXT:         field1 lv6: f32;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_ct7:[0-9]+]] ct7 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type[[TYPE_et7:[0-9]+]] et7 = struct {
// DEFAULT-NEXT:         field0 kv7: @type[[TYPE_dt7:[0-9]+]];
// DEFAULT-NEXT:         field1 lv7: array<f32, 0>;
// DEFAULT-NEXT:         field2 mv7: i32 : 9;
// DEFAULT-NEXT:         field3 nv7: i16;
// DEFAULT-NEXT:         field4 ov7: f64;
// DEFAULT-NEXT:         field5 pv7: f32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 8, 10, 16, 24], bit_offsets=[None, None, Some(64), None, None, None], bit_units=[(8, 2)], field_units=[None, None, Some(0), None, None, None]];
// DEFAULT-NEXT:     type @type[[TYPE_dt7]] dt7 = struct {
// DEFAULT-NEXT:         field0 iv7: f32;
// DEFAULT-NEXT:         field1 jv7: u16;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_ft7:[0-9]+]] ft7 = union {
// DEFAULT-NEXT:         field0 qv7: f32;
// DEFAULT-NEXT:         field1 rv7: ptr<f32>;
// DEFAULT-NEXT:         field2 sv7: ptr<u32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_gt7:[0-9]+]] gt7 = struct {
// DEFAULT-NEXT:         field0 tv7: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_vv6:[0-9]+]] vv6: @type[[TYPE_at6]] [storage=static] = aggregate<@type[[TYPE_at6]], zero_fill=false>() [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_wv6:[0-9]+]] wv6: @type[[TYPE_et6]] [storage=static] = aggregate<@type[[TYPE_et6]], zero_fill=false>(field0 = aggregate<@type[[TYPE_bt6]], zero_fill=false>(field0 = const<i32>(8), field1 = const<i32>(9), field2 = const<i32>(2), field3 = const<i32>(4), field4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(16))), field5 = const<u32>(67426805), field6 = const<i64>(1047191860)), field1 = const<u64>(1366022414), field2 = const<i32>(858), field3 = const<i32>(1), field4 = const<i32>(1), field5 = const<i32>(305), field6 = aggregate<@type[[TYPE_ct6]], zero_fill=false>(field0 = const<i64>(1069379046)), field7 = int_to_ptr<ptr<i32>, reason=explicit>(const<u32>(358273621)), field8 = aggregate<@type[[TYPE_dt6]], zero_fill=false>(field0 = const<f64>(3318.041978))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_xv6:[0-9]+]] xv6: f64 [storage=static] = const<f64>(19239.101269) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_yv6:[0-9]+]] yv6: i64 [storage=static] = const<i64>(1207859169) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_zv6:[0-9]+]] zv6: i32 [storage=static] = const<i32>(660195606) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_uv7:[0-9]+]] uv7: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(70))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_vv7:[0-9]+]] vv7: f32 [storage=static] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(96636.982442)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_wv7:[0-9]+]] wv7: f64 [storage=static] = const<f64>(28450.711801) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_xv7:[0-9]+]] xv7: @type[[TYPE_ct7]] [storage=static] = aggregate<@type[[TYPE_ct7]], zero_fill=false>() [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_yv7:[0-9]+]] yv7: @type[[TYPE_et7]] [storage=static] = aggregate<@type[[TYPE_et7]], zero_fill=false>(field0 = aggregate<@type[[TYPE_dt7]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(30135.996213)), field1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(42435)))), field1 = aggregate<array<f32, 0>, zero_fill=false>(), field2 = const<i32>(170), field3 = truncate<i16, reason=assign, fits=always>(const<i32>(22116)), field4 = const<f64>(26479.628148), field5 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(4082.960685))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_zv7:[0-9]+]] zv7: @type[[TYPE_ft7]] [storage=static] = aggregate<@type[[TYPE_ft7]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(5042.227886))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_bav7:[0-9]+]] bav7: i32 [storage=static] = const<i32>(1345451862) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_bbv7:[0-9]+]] bbv7: @type[[TYPE_gt7]] [storage=static] = aggregate<@type[[TYPE_gt7]], zero_fill=false>(field0 = const<f64>(47875.491954)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_bcv7:[0-9]+]] bcv7: array<i64, 1> [storage=static] = aggregate<array<i64, 1>, zero_fill=false>(index0 = const<i64>(1732133482)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_bdv7:[0-9]+]] bdv7: i64 [storage=static] = const<i64>(381678602) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_callee_af6:[0-9]+]] @callee_af6(%[[VALUE_ap6:[0-9]+]] ap6: @type[[TYPE_et6]], %[[VALUE_bp6:[0-9]+]] bp6: f64, %[[VALUE_cp6:[0-9]+]] cp6: i64, %[[VALUE_dp6:[0-9]+]] dp6: i32) -> @type[[TYPE_at6]] [linkage=internal] [abi=sysv64(native_c, scalar, scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..3, bits=0..6>(field0(%[[VALUE_wv6]]))), read<i32>(bitfield0<unit=0, bytes=0..3, bits=0..6>(field0(%[[VALUE_ap6]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield1<unit=0, bytes=0..3, bits=6..13>(field0(%[[VALUE_wv6]]))), read<i32>(bitfield1<unit=0, bytes=0..3, bits=6..13>(field0(%[[VALUE_ap6]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..3, bits=13..19>(field0(%[[VALUE_wv6]]))), read<i32>(bitfield2<unit=0, bytes=0..3, bits=13..19>(field0(%[[VALUE_ap6]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield3<unit=0, bytes=0..3, bits=19..24>(field0(%[[VALUE_wv6]]))), read<i32>(bitfield3<unit=0, bytes=0..3, bits=19..24>(field0(%[[VALUE_ap6]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field4(field0(%[[VALUE_wv6]]))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field4(field0(%[[VALUE_ap6]])))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<u32>(read<u32>(field5(field0(%[[VALUE_wv6]]))), read<u32>(field5(field0(%[[VALUE_ap6]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i64>(read<i64>(field6(field0(%[[VALUE_wv6]]))), read<i64>(field6(field0(%[[VALUE_ap6]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<u64>(read<u64>(field1(%[[VALUE_wv6]])), read<u64>(field1(%[[VALUE_ap6]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=24..28, bits=0..12>(%[[VALUE_wv6]])), read<i32>(bitfield2<unit=0, bytes=24..28, bits=0..12>(%[[VALUE_ap6]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield3<unit=0, bytes=24..28, bits=12..15>(%[[VALUE_wv6]])), read<i32>(bitfield3<unit=0, bytes=24..28, bits=12..15>(%[[VALUE_ap6]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield4<unit=0, bytes=24..28, bits=15..17>(%[[VALUE_wv6]])), read<i32>(bitfield4<unit=0, bytes=24..28, bits=15..17>(%[[VALUE_ap6]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield5<unit=0, bytes=24..28, bits=17..27>(%[[VALUE_wv6]])), read<i32>(bitfield5<unit=0, bytes=24..28, bits=17..27>(%[[VALUE_ap6]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i64>(read<i64>(field0(field6(%[[VALUE_wv6]]))), read<i64>(field0(field6(%[[VALUE_ap6]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<ptr<i32>>(read<ptr<i32>>(field7(%[[VALUE_wv6]])), read<ptr<i32>>(field7(%[[VALUE_ap6]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<f64, exceptions=observable>(read<f64>(field0(field8(%[[VALUE_wv6]]))), read<f64>(field0(field8(%[[VALUE_ap6]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<f64, exceptions=observable>(read<f64>(%[[VALUE_xv6]]), read<f64>(%[[VALUE_bp6]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i64>(read<i64>(%[[VALUE_yv6]]), read<i64>(%[[VALUE_cp6]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(%[[VALUE_zv6]]), read<i32>(%[[VALUE_dp6]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return copy<@type[[TYPE_at6]], reason=return>(read<@type[[TYPE_at6]]>(%[[VALUE_vv6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_caller_bf6:[0-9]+]] @caller_bf6() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_bav6:[0-9]+]] bav6: @type[[TYPE_at6]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE_at6]]>(%[[VALUE_bav6]], copy<@type[[TYPE_at6]], reason=assign>(call<@type[[TYPE_at6]], signature=fn(@type[[TYPE_et6]], f64, i64, i32) -> @type[[TYPE_at6]], abi=sysv64(native_c, scalar, scalar, scalar) -> native_c>(%[[VALUE_callee_af6]], copy<@type[[TYPE_et6]], reason=arg>(read<@type[[TYPE_et6]]>(%[[VALUE_wv6]])), read<f64>(%[[VALUE_xv6]]), read<i64>(%[[VALUE_yv6]]), read<i32>(%[[VALUE_zv6]]))));
// DEFAULT-NEXT:         copy<@type[[TYPE_at6]], reason=assign>(call<@type[[TYPE_at6]], signature=fn(@type[[TYPE_et6]], f64, i64, i32) -> @type[[TYPE_at6]], abi=sysv64(native_c, scalar, scalar, scalar) -> native_c>(%[[VALUE_callee_af6]], copy<@type[[TYPE_et6]], reason=arg>(read<@type[[TYPE_et6]]>(%[[VALUE_wv6]])), read<f64>(%[[VALUE_xv6]]), read<i64>(%[[VALUE_yv6]]), read<i32>(%[[VALUE_zv6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_callee_af7:[0-9]+]] @callee_af7(%[[VALUE_ap7:[0-9]+]] ap7: f32, %[[VALUE_bp7:[0-9]+]] bp7: f64, %[[VALUE_cp7:[0-9]+]] cp7: @type[[TYPE_ct7]], %[[VALUE_dp7:[0-9]+]] dp7: @type[[TYPE_et7]], %[[VALUE_ep7:[0-9]+]] ep7: @type[[TYPE_ft7]], %[[VALUE_fp7:[0-9]+]] fp7: i32, %[[VALUE_gp7:[0-9]+]] gp7: @type[[TYPE_gt7]], %[[VALUE_hp7:[0-9]+]] hp7: ptr<i64> [array=1], %[[VALUE_ip7:[0-9]+]] ip7: i64) -> u8 [linkage=internal] [abi=sysv64(scalar, scalar, native_c, native_c, native_c, scalar, native_c, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32>(%[[VALUE_vv7]]), read<f32>(%[[VALUE_ap7]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<f64, exceptions=observable>(read<f64>(%[[VALUE_wv7]]), read<f64>(%[[VALUE_bp7]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32>(field0(field0(%[[VALUE_yv7]]))), read<f32>(field0(field0(%[[VALUE_dp7]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(field0(%[[VALUE_yv7]]))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(field0(%[[VALUE_dp7]])))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=8..10, bits=0..9>(%[[VALUE_yv7]])), read<i32>(bitfield2<unit=0, bytes=8..10, bits=0..9>(%[[VALUE_dp7]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(widen<i32, reason=promotion>(read<i16>(field3(%[[VALUE_yv7]]))), widen<i32, reason=promotion>(read<i16>(field3(%[[VALUE_dp7]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<f64, exceptions=observable>(read<f64>(field4(%[[VALUE_yv7]])), read<f64>(field4(%[[VALUE_dp7]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32>(field5(%[[VALUE_yv7]])), read<f32>(field5(%[[VALUE_dp7]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=observable>(read<f32>(field0(%[[VALUE_zv7]])), read<f32>(field0(%[[VALUE_ep7]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(%[[VALUE_bav7]]), read<i32>(%[[VALUE_fp7]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<f64, exceptions=observable>(read<f64>(field0(%[[VALUE_bbv7]])), read<f64>(field0(%[[VALUE_gp7]]))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1)>(%[[VALUE_bcv7]]), const<i32>(0)))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE_hp7]]), const<i32>(0))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if not<bool>(eq<i64>(read<i64>(%[[VALUE_bdv7]]), read<i64>(%[[VALUE_ip7]])))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return read<u8>(%[[VALUE_uv7]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_caller_bf7:[0-9]+]] @caller_bf7() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_bev7:[0-9]+]] bev7: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%[[VALUE_bev7]], call<u8, signature=fn(f32, f64, @type[[TYPE_ct7]], @type[[TYPE_et7]], @type[[TYPE_ft7]], i32, @type[[TYPE_gt7]], ptr<i64>, i64) -> u8, abi=sysv64(scalar, scalar, native_c, native_c, native_c, scalar, native_c, scalar, scalar) -> scalar>(%[[VALUE_callee_af7]], read<f32>(%[[VALUE_vv7]]), read<f64>(%[[VALUE_wv7]]), copy<@type[[TYPE_ct7]], reason=arg>(read<@type[[TYPE_ct7]]>(%[[VALUE_xv7]])), copy<@type[[TYPE_et7]], reason=arg>(read<@type[[TYPE_et7]]>(%[[VALUE_yv7]])), copy<@type[[TYPE_ft7]], reason=arg>(read<@type[[TYPE_ft7]]>(%[[VALUE_zv7]])), read<i32>(%[[VALUE_bav7]]), copy<@type[[TYPE_gt7]], reason=arg>(read<@type[[TYPE_gt7]]>(%[[VALUE_bbv7]])), array_decay<ptr<i64>, length=Some(1)>(%[[VALUE_bcv7]]), read<i64>(%[[VALUE_bdv7]])));
// DEFAULT-NEXT:         call<u8, signature=fn(f32, f64, @type[[TYPE_ct7]], @type[[TYPE_et7]], @type[[TYPE_ft7]], i32, @type[[TYPE_gt7]], ptr<i64>, i64) -> u8, abi=sysv64(scalar, scalar, native_c, native_c, native_c, scalar, native_c, scalar, scalar) -> scalar>(%[[VALUE_callee_af7]], read<f32>(%[[VALUE_vv7]]), read<f64>(%[[VALUE_wv7]]), copy<@type[[TYPE_ct7]], reason=arg>(read<@type[[TYPE_ct7]]>(%[[VALUE_xv7]])), copy<@type[[TYPE_et7]], reason=arg>(read<@type[[TYPE_et7]]>(%[[VALUE_yv7]])), copy<@type[[TYPE_ft7]], reason=arg>(read<@type[[TYPE_ft7]]>(%[[VALUE_zv7]])), read<i32>(%[[VALUE_bav7]]), copy<@type[[TYPE_gt7]], reason=arg>(read<@type[[TYPE_gt7]]>(%[[VALUE_bbv7]])), array_decay<ptr<i64>, length=Some(1)>(%[[VALUE_bcv7]]), read<i64>(%[[VALUE_bdv7]]));
// DEFAULT-NEXT:         if not<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_uv7]]))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE_bev7]])))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_caller_bf6]]);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_caller_bf7]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
