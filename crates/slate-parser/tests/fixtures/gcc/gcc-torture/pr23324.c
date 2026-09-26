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
// DEFAULT-NEXT:     type @type0 at6 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type1 et6 = struct {
// DEFAULT-NEXT:         field0 mv6: @type2;
// DEFAULT-NEXT:         field1 nv6: u64;
// DEFAULT-NEXT:         field2 ov6: i32 : 12;
// DEFAULT-NEXT:         field3 pv6: i32 : 3;
// DEFAULT-NEXT:         field4 qv6: i32 : 2;
// DEFAULT-NEXT:         field5 rv6: i32 : 10;
// DEFAULT-NEXT:         field6 sv6: @type3;
// DEFAULT-NEXT:         field7 tv6: ptr<i32>;
// DEFAULT-NEXT:         field8 uv6: @type4;
// DEFAULT-NEXT:     } [size=56, align=8, offsets=[0, 16, 24, 25, 25, 26, 32, 40, 48], bit_offsets=[None, None, Some(192), Some(204), Some(207), Some(209), None, None, None], bit_units=[(24, 4)], field_units=[None, None, Some(0), Some(0), Some(0), Some(0), None, None, None]];
// DEFAULT-NEXT:     type @type2 bt6 = struct {
// DEFAULT-NEXT:         field0 av6: i32 : 6;
// DEFAULT-NEXT:         field1 bv6: i32 : 7;
// DEFAULT-NEXT:         field2 cv6: i32 : 6;
// DEFAULT-NEXT:         field3 dv6: i32 : 5;
// DEFAULT-NEXT:         field4 ev6: u8;
// DEFAULT-NEXT:         field5 fv6: u32;
// DEFAULT-NEXT:         field6 gv6: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 0, 1, 2, 3, 4, 8], bit_offsets=[Some(0), Some(6), Some(13), Some(19), None, None, None], bit_units=[(0, 3)], field_units=[Some(0), Some(0), Some(0), Some(0), None, None, None]];
// DEFAULT-NEXT:     type @type3 ct6 = union {
// DEFAULT-NEXT:         field0 hv6: i64;
// DEFAULT-NEXT:         field1 iv6: f32;
// DEFAULT-NEXT:         field2 jv6: f32;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type4 dt6 = union {
// DEFAULT-NEXT:         field0 kv6: f64;
// DEFAULT-NEXT:         field1 lv6: f32;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type5 ct7 = union {
// DEFAULT-NEXT:     } [size=0, align=1, offsets=[]];
// DEFAULT-NEXT:     type @type6 et7 = struct {
// DEFAULT-NEXT:         field0 kv7: @type7;
// DEFAULT-NEXT:         field1 lv7: array<f32, 0>;
// DEFAULT-NEXT:         field2 mv7: i32 : 9;
// DEFAULT-NEXT:         field3 nv7: i16;
// DEFAULT-NEXT:         field4 ov7: f64;
// DEFAULT-NEXT:         field5 pv7: f32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 8, 10, 16, 24], bit_offsets=[None, None, Some(64), None, None, None], bit_units=[(8, 2)], field_units=[None, None, Some(0), None, None, None]];
// DEFAULT-NEXT:     type @type7 dt7 = struct {
// DEFAULT-NEXT:         field0 iv7: f32;
// DEFAULT-NEXT:         field1 jv7: u16;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type8 ft7 = union {
// DEFAULT-NEXT:         field0 qv7: f32;
// DEFAULT-NEXT:         field1 rv7: ptr<f32>;
// DEFAULT-NEXT:         field2 sv7: ptr<u32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0]];
// DEFAULT-NEXT:     type @type9 gt7 = struct {
// DEFAULT-NEXT:         field0 tv7: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %2 vv6: @type0 [storage=static] = aggregate<@type0, zero_fill=false>() [linkage=internal];
// DEFAULT-NEXT:     global %7 wv6: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = aggregate<@type2, zero_fill=false>(field0 = const<i32>(8), field1 = const<i32>(9), field2 = const<i32>(2), field3 = const<i32>(4), field4 = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(16))), field5 = const<u32>(67426805), field6 = const<i64>(1047191860)), field1 = const<u64>(1366022414), field2 = const<i32>(858), field3 = const<i32>(1), field4 = const<i32>(1), field5 = const<i32>(305), field6 = aggregate<@type3, zero_fill=false>(field0 = const<i64>(1069379046)), field7 = int_to_ptr<ptr<i32>, reason=explicit>(const<u32>(358273621)), field8 = aggregate<@type4, zero_fill=false>(field0 = const<f64>(3318.041978))) [linkage=internal];
// DEFAULT-NEXT:     global %8 xv6: f64 [storage=static] = const<f64>(19239.101269) [linkage=internal];
// DEFAULT-NEXT:     global %9 yv6: i64 [storage=static] = const<i64>(1207859169) [linkage=internal];
// DEFAULT-NEXT:     global %10 zv6: i32 [storage=static] = const<i32>(660195606) [linkage=internal];
// DEFAULT-NEXT:     global %18 uv7: u8 [storage=static] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(70))) [linkage=internal];
// DEFAULT-NEXT:     global %19 vv7: f32 [storage=static] = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(96636.982442)) [linkage=internal];
// DEFAULT-NEXT:     global %20 wv7: f64 [storage=static] = const<f64>(28450.711801) [linkage=internal];
// DEFAULT-NEXT:     global %22 xv7: @type5 [storage=static] = aggregate<@type5, zero_fill=false>() [linkage=internal];
// DEFAULT-NEXT:     global %25 yv7: @type6 [storage=static] = aggregate<@type6, zero_fill=false>(field0 = aggregate<@type7, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(30135.996213)), field1 = reinterpret<u16, reason=assign, fits=unknown>(truncate<i16, reason=assign, fits=unknown>(const<i32>(42435)))), field1 = aggregate<array<f32, 0>, zero_fill=false>(), field2 = const<i32>(170), field3 = truncate<i16, reason=assign, fits=always>(const<i32>(22116)), field4 = const<f64>(26479.628148), field5 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(4082.960685))) [linkage=internal];
// DEFAULT-NEXT:     global %27 zv7: @type8 [storage=static] = aggregate<@type8, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(5042.227886))) [linkage=internal];
// DEFAULT-NEXT:     global %28 bav7: i32 [storage=static] = const<i32>(1345451862) [linkage=internal];
// DEFAULT-NEXT:     global %30 bbv7: @type9 [storage=static] = aggregate<@type9, zero_fill=false>(field0 = const<f64>(47875.491954)) [linkage=internal];
// DEFAULT-NEXT:     global %31 bcv7: array<i64, 1> [storage=static] = aggregate<array<i64, 1>, zero_fill=false>(index0 = const<i64>(1732133482)) [linkage=internal];
// DEFAULT-NEXT:     global %32 bdv7: i64 [storage=static] = const<i64>(381678602) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %11 @callee_af6(%12 ap6: @type1, %13 bp6: f64, %14 cp6: i64, %15 dp6: i32) -> @type0 [linkage=internal] [abi=sysv64(native_c, scalar, scalar, scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield0<unit=0, bytes=0..3, bits=0..6>(field0(%7))), read<i32>(bitfield0<unit=0, bytes=0..3, bits=0..6>(field0(%12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield1<unit=0, bytes=0..3, bits=6..13>(field0(%7))), read<i32>(bitfield1<unit=0, bytes=0..3, bits=6..13>(field0(%12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=0..3, bits=13..19>(field0(%7))), read<i32>(bitfield2<unit=0, bytes=0..3, bits=13..19>(field0(%12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield3<unit=0, bytes=0..3, bits=19..24>(field0(%7))), read<i32>(bitfield3<unit=0, bytes=0..3, bits=19..24>(field0(%12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field4(field0(%7))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(field4(field0(%12)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<u32>(read<u32>(field5(field0(%7))), read<u32>(field5(field0(%12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i64>(read<i64>(field6(field0(%7))), read<i64>(field6(field0(%12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<u64>(read<u64>(field1(%7)), read<u64>(field1(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=24..28, bits=0..12>(%7)), read<i32>(bitfield2<unit=0, bytes=24..28, bits=0..12>(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield3<unit=0, bytes=24..28, bits=12..15>(%7)), read<i32>(bitfield3<unit=0, bytes=24..28, bits=12..15>(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield4<unit=0, bytes=24..28, bits=15..17>(%7)), read<i32>(bitfield4<unit=0, bytes=24..28, bits=15..17>(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield5<unit=0, bytes=24..28, bits=17..27>(%7)), read<i32>(bitfield5<unit=0, bytes=24..28, bits=17..27>(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i64>(read<i64>(field0(field6(%7))), read<i64>(field0(field6(%12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<ptr<i32>>(read<ptr<i32>>(field7(%7)), read<ptr<i32>>(field7(%12))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<f64, exceptions=ignore>(read<f64>(field0(field8(%7))), read<f64>(field0(field8(%12)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<f64, exceptions=ignore>(read<f64>(%8), read<f64>(%13)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i64>(read<i64>(%9), read<i64>(%14)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(%10), read<i32>(%15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return copy<@type0, reason=return>(read<@type0>(%2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @caller_bf6() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %17 bav6: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<@type0>(%17, copy<@type0, reason=assign>(call<@type0, signature=fn(@type1, f64, i64, i32) -> @type0, abi=sysv64(native_c, scalar, scalar, scalar) -> native_c>(%11, copy<@type1, reason=arg>(read<@type1>(%7)), read<f64>(%8), read<i64>(%9), read<i32>(%10))));
// DEFAULT-NEXT:         copy<@type0, reason=assign>(call<@type0, signature=fn(@type1, f64, i64, i32) -> @type0, abi=sysv64(native_c, scalar, scalar, scalar) -> native_c>(%11, copy<@type1, reason=arg>(read<@type1>(%7)), read<f64>(%8), read<i64>(%9), read<i32>(%10)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %33 @callee_af7(%34 ap7: f32, %35 bp7: f64, %36 cp7: @type5, %37 dp7: @type6, %38 ep7: @type8, %39 fp7: i32, %40 gp7: @type9, %41 hp7: ptr<i64> [array=1], %42 ip7: i64) -> u8 [linkage=internal] [abi=sysv64(scalar, scalar, native_c, native_c, coerce<i64>, scalar, coerce<f64>, scalar, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=ignore>(read<f32>(%19), read<f32>(%34)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<f64, exceptions=ignore>(read<f64>(%20), read<f64>(%35)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=ignore>(read<f32>(field0(field0(%25))), read<f32>(field0(field0(%37)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(field0(%25))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u16>(field1(field0(%37)))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(bitfield2<unit=0, bytes=8..10, bits=0..9>(%25)), read<i32>(bitfield2<unit=0, bytes=8..10, bits=0..9>(%37))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(widen<i32, reason=promotion>(read<i16>(field3(%25))), widen<i32, reason=promotion>(read<i16>(field3(%37)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<f64, exceptions=ignore>(read<f64>(field4(%25)), read<f64>(field4(%37))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=ignore>(read<f32>(field5(%25)), read<f32>(field5(%37))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<f32, exceptions=ignore>(read<f32>(field0(%27)), read<f32>(field0(%38))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i32>(read<i32>(%28), read<i32>(%39)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<f64, exceptions=ignore>(read<f64>(field0(%30)), read<f64>(field0(%40))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(1)>(%31), const<i32>(0)))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%41), const<i32>(0))))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if not<bool>(eq<i64>(read<i64>(%32), read<i64>(%42)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return read<u8>(%18);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @caller_bf7() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %44 bev7: u8 [storage=automatic];
// DEFAULT-NEXT:         write<u8>(%44, call<u8, signature=fn(f32, f64, @type5, @type6, @type8, i32, @type9, ptr<i64>, i64) -> u8, abi=sysv64(scalar, scalar, native_c, native_c, coerce<i64>, scalar, coerce<f64>, scalar, scalar) -> scalar>(%33, read<f32>(%19), read<f64>(%20), copy<@type5, reason=arg>(read<@type5>(%22)), copy<@type6, reason=arg>(read<@type6>(%25)), copy<@type8, reason=arg>(read<@type8>(%27)), read<i32>(%28), copy<@type9, reason=arg>(read<@type9>(%30)), array_decay<ptr<i64>, length=Some(1)>(%31), read<i64>(%32)));
// DEFAULT-NEXT:         call<u8, signature=fn(f32, f64, @type5, @type6, @type8, i32, @type9, ptr<i64>, i64) -> u8, abi=sysv64(scalar, scalar, native_c, native_c, coerce<i64>, scalar, coerce<f64>, scalar, scalar) -> scalar>(%33, read<f32>(%19), read<f64>(%20), copy<@type5, reason=arg>(read<@type5>(%22)), copy<@type6, reason=arg>(read<@type6>(%25)), copy<@type8, reason=arg>(read<@type8>(%27)), read<i32>(%28), copy<@type9, reason=arg>(read<@type9>(%30)), array_decay<ptr<i64>, length=Some(1)>(%31), read<i64>(%32));
// DEFAULT-NEXT:         if not<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%18))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%44)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%16);
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%43);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
