typedef struct creal_T {
  double re;
  double im;
} creal_T;

#define N 16
int main() {
  int     k;
  int     i;
  int     j;
  creal_T t2[N];
  double  inval;

  inval = 1.0;
  for (j = 0; j < N; ++j) {
    t2[j].re = 0;
    t2[j].im = 0;
  }

  for (j = 0; j < N / 4; j++) {
    i        = j * 4;
    t2[i].re = inval;
    t2[i].im = inval;
    k        = i + 3;
    t2[k].re = inval;
    t2[k].im = inval;
    t2[i]    = t2[k];
    t2[k].re = inval;
  }

  for (i = 0; i < 2; ++i)
    if (t2[i].re != !i || t2[i].im != !i)
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
// DEFAULT-NEXT:     type @type[[TYPE_creal_T:[0-9]+]] creal_T = struct {
// DEFAULT-NEXT:         field0 re: f64;
// DEFAULT-NEXT:         field1 im: f64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_creal_T_2:[0-9]+]] creal_T = @type[[TYPE_creal_T]];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_t2:[0-9]+]] t2: array<@type[[TYPE_creal_T]], 16> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_inval:[0-9]+]] inval: f64 [storage=automatic];
// DEFAULT-NEXT:         write<f64>(%[[VALUE_inval]], const<f64>(1.0));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(16))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(field0(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_j]])))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:                     write<f64>(field1(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_j]])))), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(0)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j]]), div<i32, by_zero=ub, min_by_neg_one=ub>(const<i32>(16), const<i32>(4)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], mul<i32, overflow=ub>(read<i32>(%[[VALUE_j]]), const<i32>(4)));
// DEFAULT-NEXT:                     write<f64>(field0(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_i]])))), read<f64>(%[[VALUE_inval]]));
// DEFAULT-NEXT:                     write<f64>(field1(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_i]])))), read<f64>(%[[VALUE_inval]]));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_k]], add<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), const<i32>(3)));
// DEFAULT-NEXT:                     write<f64>(field0(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_k]])))), read<f64>(%[[VALUE_inval]]));
// DEFAULT-NEXT:                     write<f64>(field1(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_k]])))), read<f64>(%[[VALUE_inval]]));
// DEFAULT-NEXT:                     write<@type[[TYPE_creal_T]]>(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_i]]))), copy<@type[[TYPE_creal_T]], reason=assign>(read<@type[[TYPE_creal_T]]>(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_k]]))))));
// DEFAULT-NEXT:                     write<f64>(field0(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_k]])))), read<f64>(%[[VALUE_inval]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_i]]))))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=promotion>(not<bool>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))))), ne<f64, exceptions=observable>(read<f64>(field1(deref(ptr_offset<ptr<@type[[TYPE_creal_T]]>, subtract=false, element=@type[[TYPE_creal_T]], overflow=ub>(array_decay<ptr<@type[[TYPE_creal_T]]>, length=Some(16)>(%[[VALUE_t2]]), read<i32>(%[[VALUE_i]]))))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(from_bool<i32, reason=promotion>(not<bool>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)))))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
