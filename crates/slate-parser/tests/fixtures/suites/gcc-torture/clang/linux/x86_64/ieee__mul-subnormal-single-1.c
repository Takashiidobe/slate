/* { dg-do run }
   # C6X floating point hardware turns denormals to zero in multiplications.
   { dg-xfail-if "" { tic6x-*-* && ti_c67x } }

   # ColdFire FPUs require software handling of subnormals.  We are
   # not aware of any system that has this.
   { dg-xfail-if "" { m68k-*-* && coldfire_fpu } }

   # The C-SKY hardware FPU only supports flush-to-zero mode.
   { dg-xfail-if "" { csky-*-* && hard_float } }

   # The Epiphany single-precision floating point format does not
   # support subnormals.
   { dg-skip-if "" { epiphany-*-* } } */

/* Check that certain subnormal numbers (formerly known as denormalized
   numbers) are rounded to within 0.5 ulp.  PR other/14354.  */

/* This test requires that float and unsigned int are the same size and
   that the sign-bit of the float is at MSB of the unsigned int.  */

void abort(void);
void exit(int);

#if __INT_MAX__ != 2147483647L
int main() { exit(0); }
#else

union uf {
  unsigned int u;
  float        f;
};

static float u2f(unsigned int v) {
  union uf u;
  u.u = v;
  return u.f;
}

static unsigned int f2u(float v) {
  union uf u;
  u.f = v;
  return u.u;
}

int ok = 1;

static void tstmul(unsigned int ux, unsigned int uy, unsigned int ur) {
  float x = u2f(ux);
  float y = u2f(uy);

  if (f2u(x * y) != ur)
    /* Set a variable rather than aborting here, to simplify tracing when
       several computations are wrong.  */
    ok = 0;
}

/* We don't want to make this const and static, or else we risk inlining
   causing the test to fold as constants at compile-time.  */
struct {
  unsigned int p1, p2, res;
} expected[] = {{0xfff, 0x3f800400, 0xfff},
                {0xf, 0x3fc88888, 0x17},
                {0xf, 0x3f844444, 0xf}};

int main() {
  unsigned int i;

  for (i = 0; i < sizeof(expected) / sizeof(expected[0]); i++) {
    tstmul(expected[i].p1, expected[i].p2, expected[i].res);
    tstmul(expected[i].p2, expected[i].p1, expected[i].res);
  }

  if (!ok)
    abort();

  exit(0);
}
#endif


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
// DEFAULT-NEXT:     type @type[[TYPE_uf:[0-9]+]] uf = union {
// DEFAULT-NEXT:         field0 u: u32;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 p1: u32;
// DEFAULT-NEXT:         field1 p2: u32;
// DEFAULT-NEXT:         field2 res: u32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %[[VALUE_ok:[0-9]+]] ok: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_expected:[0-9]+]] expected: array<@type[[TYPE0]], 3> [storage=static] [align=16] = aggregate<array<@type[[TYPE0]], 3>, zero_fill=false>(index0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(4095)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1065354240)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(4095))), index1 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(15)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1070106760)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(23))), index2 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(15)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1065632836)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(15)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_u2f:[0-9]+]] @u2f(%[[VALUE_v:[0-9]+]] v: u32) -> f32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_uf]] [storage=automatic];
// DEFAULT-NEXT:         write<u32>(field0(%[[VALUE_u]]), read<u32>(%[[VALUE_v]]));
// DEFAULT-NEXT:         return read<f32>(field1(%[[VALUE_u]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2u:[0-9]+]] @f2u(%[[VALUE_v_2:[0-9]+]] v: f32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_u_2:[0-9]+]] u: @type[[TYPE_uf]] [storage=automatic];
// DEFAULT-NEXT:         write<f32>(field1(%[[VALUE_u_2]]), read<f32>(%[[VALUE_v_2]]));
// DEFAULT-NEXT:         return read<u32>(field0(%[[VALUE_u_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_tstmul:[0-9]+]] @tstmul(%[[VALUE_ux:[0-9]+]] ux: u32, %[[VALUE_uy:[0-9]+]] uy: u32, %[[VALUE_ur:[0-9]+]] ur: u32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: f32 [storage=automatic] = call<f32, signature=fn(u32) -> f32>(%[[VALUE_u2f]], read<u32>(%[[VALUE_ux]]));
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: f32 [storage=automatic] = call<f32, signature=fn(u32) -> f32>(%[[VALUE_u2f]], read<u32>(%[[VALUE_uy]]));
// DEFAULT-NEXT:         if ne<u32>(call<u32, signature=fn(f32) -> u32>(%[[VALUE_f2u]], mul<f32, rounding=nearest_even, exceptions=ignore, contract=on>(read<f32>(%[[VALUE_x]]), read<f32>(%[[VALUE_y]]))), read<u32>(%[[VALUE_ur]]))
// DEFAULT-NEXT:             write<i32>(%[[VALUE_ok]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: u32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u64>(widen<u64, reason=usual_arith>(read<u32>(%[[VALUE_i]])), div<u64, by_zero=ub>(const<u64>(36), const<u64>(12)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i]], read<u32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn(u32, u32, u32) -> void>(%[[VALUE_tstmul]], read<u32>(field0(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(3)>(%[[VALUE_expected]]), read<u32>(%[[VALUE_i]]))))), read<u32>(field1(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(3)>(%[[VALUE_expected]]), read<u32>(%[[VALUE_i]]))))), read<u32>(field2(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(3)>(%[[VALUE_expected]]), read<u32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                     call<void, signature=fn(u32, u32, u32) -> void>(%[[VALUE_tstmul]], read<u32>(field1(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(3)>(%[[VALUE_expected]]), read<u32>(%[[VALUE_i]]))))), read<u32>(field0(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(3)>(%[[VALUE_expected]]), read<u32>(%[[VALUE_i]]))))), read<u32>(field2(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(array_decay<ptr<@type[[TYPE0]]>, length=Some(3)>(%[[VALUE_expected]]), read<u32>(%[[VALUE_i]]))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if not<bool>(ne<i32>(read<i32>(%[[VALUE_ok]]), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
