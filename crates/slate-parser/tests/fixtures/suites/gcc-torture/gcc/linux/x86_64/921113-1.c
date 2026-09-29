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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 wsx: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_struct_list:[0-9]+]] struct_list = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_list_t:[0-9]+]] list_t = ptr<@type[[TYPE0]]>;
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 x: f32;
// DEFAULT-NEXT:         field1 y: f32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_vector_t:[0-9]+]] vector_t = @type[[TYPE1]];
// DEFAULT-NEXT:     global %[[VALUE_pos:[0-9]+]] pos: array<@type[[TYPE1]], 1> [storage=static] = aggregate<array<@type[[TYPE1]], 1>, zero_fill=false>(index0 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_limit:[0-9]+]] limit: array<@type[[TYPE1]], 2> [storage=static] [align=16] = aggregate<array<@type[[TYPE1]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(0.0))), index1 = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)), field1 = float_narrow<f32, reason=assign, rounding=nearest_even, exceptions=ignore>(const<f64>(1.0)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_w:[0-9]+]] @w(%[[VALUE_x:[0-9]+]] x: f32, %[[VALUE_y:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x_2:[0-9]+]] x: f32, %[[VALUE_y_2:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32>(%[[VALUE_x_2]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))), ne<f32, exceptions=observable>(read<f32>(%[[VALUE_y_2]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(0))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_3:[0-9]+]] x: f32, %[[VALUE_y_3:[0-9]+]] y: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<f32, exceptions=observable>(read<f32>(%[[VALUE_x_3]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))), ne<f32, exceptions=observable>(read<f32>(%[[VALUE_y_3]]), int_to_float<f32, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_gitter:[0-9]+]] @gitter(%[[VALUE_count:[0-9]+]] count: i32, %[[VALUE_pos_2:[0-9]+]] pos: ptr<@type[[TYPE1]]>, %[[VALUE_list:[0-9]+]] list: ptr<@type[[TYPE0]]>, %[[VALUE_nww:[0-9]+]] nww: ptr<i32>, %[[VALUE_limit_2:[0-9]+]] limit: ptr<@type[[TYPE1]]> [array=2], %[[VALUE_r:[0-9]+]] r: f32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: f32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_gitt:[0-9]+]] gitt: array<array<i32, 128>, 128> [storage=automatic] [align=16];
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_f1]], read<f32>(field0(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false, element=@type[[TYPE1]], overflow=ub>(read<ptr<@type[[TYPE1]]>>(%[[VALUE_limit_2]]), const<i32>(0))))), read<f32>(field1(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false, element=@type[[TYPE1]], overflow=ub>(read<ptr<@type[[TYPE1]]>>(%[[VALUE_limit_2]]), const<i32>(0))))));
// DEFAULT-NEXT:         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_f2]], read<f32>(field0(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false, element=@type[[TYPE1]], overflow=ub>(read<ptr<@type[[TYPE1]]>>(%[[VALUE_limit_2]]), const<i32>(1))))), read<f32>(field1(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false, element=@type[[TYPE1]], overflow=ub>(read<ptr<@type[[TYPE1]]>>(%[[VALUE_limit_2]]), const<i32>(1))))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_nww]])), const<i32>(0));
// DEFAULT-NEXT:         write<f32>(%[[VALUE_d]], read<f32>(field0(deref(ptr_offset<ptr<@type[[TYPE1]]>, subtract=false, element=@type[[TYPE1]], overflow=ub>(read<ptr<@type[[TYPE1]]>>(%[[VALUE_pos_2]]), const<i32>(0))))));
// DEFAULT-NEXT:         if le<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE_d]])), const<f64>(0.0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 call<void, signature=fn(f32, f32) -> void>(%[[VALUE_w]], read<f32>(%[[VALUE_d]]), read<f32>(%[[VALUE_r]]));
// DEFAULT-NEXT:                 if le<f64, exceptions=observable>(float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE_d]])), mul<f64, rounding=nearest_even, exceptions=observable, contract=fast>(float_widen<f64, reason=usual_arith>(read<f32>(%[[VALUE_r]])), const<f64>(0.5)))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         call<void, signature=fn(f32, f32) -> void>(%[[VALUE_w]], read<f32>(%[[VALUE_d]]), read<f32>(%[[VALUE_r]]));
// DEFAULT-NEXT:                         write<f32>(field0(deref(ptr_offset<ptr<@type[[TYPE0]]>, subtract=false, element=@type[[TYPE0]], overflow=ub>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_list]]), const<i32>(0)))), int_to_float<f32, reason=assign, exact=false, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_nww_2:[0-9]+]] nww: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_list_2:[0-9]+]] list: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<@type[[TYPE1]]>, ptr<@type[[TYPE0]]>, ptr<i32>, ptr<@type[[TYPE1]]>, f32) -> void>(%[[VALUE_gitter]], const<i32>(1), array_decay<ptr<@type[[TYPE1]]>, length=Some(1)>(%[[VALUE_pos]]), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_list_2]]), addr_of<ptr<i32>>(%[[VALUE_nww_2]]), array_decay<ptr<@type[[TYPE1]]>, length=Some(2)>(%[[VALUE_limit]]), float_narrow<f32, reason=arg, rounding=nearest_even, exceptions=observable>(const<f64>(1.0)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
