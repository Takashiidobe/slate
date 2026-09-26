/* { dg-require-stack-size "128 * 128 * 4 + 1024" } */

void abort(void);
void exit(int);

typedef struct {
  float wsx;
} struct_list;

typedef struct_list *list_t;

typedef struct {
  float x, y;
} vector_t;

void w(float x, float y) {}

void f1(float x, float y) {
  if (x != 0 || y != 0)
    abort();
}
void f2(float x, float y) {
  if (x != 1 || y != 1)
    abort();
}

void gitter(int count, vector_t pos[], list_t list, int *nww, vector_t limit[2],
            float r) {
  float d;
  int   gitt[128][128];

  f1(limit[0].x, limit[0].y);
  f2(limit[1].x, limit[1].y);

  *nww = 0;

  d = pos[0].x;
  if (d <= 0.) {
    w(d, r);
    if (d <= r * 0.5) {
      w(d, r);
      list[0].wsx = 1;
    }
  }
}

vector_t pos[1]   = {{0., 0.}};
vector_t limit[2] = {{0., 0.}, {1., 1.}};

int main(void) {
  int         nww;
  struct_list list;

  gitter(1, pos, &list, &nww, limit, 1.);
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 wsx: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 struct_list = @type0;
// DEFAULT-NEXT:     type @type2 list_t = ptr<@type0>;
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 x: f32;
// DEFAULT-NEXT:         field1 y: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type4 vector_t = @type3;
// DEFAULT-NEXT:     global %25 pos: array<@type3, 1> [storage=static] = aggregate<array<@type3, 1>, zero_fill=false>(index0 = aggregate<@type3, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)))) [linkage=external];
// DEFAULT-NEXT:     global %26 limit: array<@type3, 2> [storage=static] [align=16] = aggregate<array<@type3, 2>, zero_fill=false>(index0 = aggregate<@type3, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0))), index1 = aggregate<@type3, zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)))) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%30 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %7 @w(%8 x: f32, %9 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @f1(%11 x: f32, %12 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=ignore>(read<f32>(%11), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<f32, exceptions=ignore>(read<f32>(%12), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @f2(%14 x: f32, %15 y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=ignore>(read<f32>(%14), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))), ne<f32, exceptions=ignore>(read<f32>(%15), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @gitter(%17 count: i32, %18 pos: ptr<@type3>, %19 list: ptr<@type0>, %20 nww: ptr<i32>, %21 limit: ptr<@type3> [array=2], %22 r: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %23 d: f32 [storage=automatic];
// DEFAULT-NEXT:         let %24 gitt: array<array<i32, 128>, 128> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%10, read<f32>(field0(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(read<ptr<@type3>>(%21), const<i32>(0))))), read<f32>(field1(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(read<ptr<@type3>>(%21), const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%13, read<f32>(field0(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(read<ptr<@type3>>(%21), const<i32>(1))))), read<f32>(field1(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(read<ptr<@type3>>(%21), const<i32>(1))))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%20)), const<i32>(0));
// DEFAULT-NEXT:         write<f32>(%23, read<f32>(field0(deref(ptr_offset<ptr<@type3>, subtract=false, element=@type3, overflow=ub>(read<ptr<@type3>>(%18), const<i32>(0))))));
// DEFAULT-NEXT:         if le<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(%23)), const<f64>(0.0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(f32, f32) -> void>(%7, read<f32>(%23), read<f32>(%22));
// DEFAULT-NEXT:                 if le<f64, exceptions=ignore>(float_widen<f64, reason=usual_arith>(read<f32>(%23)), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(float_widen<f64, reason=usual_arith>(read<f32>(%22)), const<f64>(0.5)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         call<void, signature=fn(f32, f32) -> void>(%7, read<f32>(%23), read<f32>(%22));
// DEFAULT-NEXT:                         write<f32>(field0(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%19), const<i32>(0)))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %27 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %28 nww: i32 [storage=automatic];
// DEFAULT-NEXT:         let %29 list: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<@type3>, ptr<@type0>, ptr<i32>, ptr<@type3>, f32) -> void>(%16, const<i32>(1), array_decay<ptr<@type3>, length=Some(1)>(%25), addr_of<ptr<@type0>>(%29), addr_of<ptr<i32>>(%28), array_decay<ptr<@type3>, length=Some(2)>(%26), float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
