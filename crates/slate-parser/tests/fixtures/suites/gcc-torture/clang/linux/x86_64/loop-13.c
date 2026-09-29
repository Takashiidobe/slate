/* PR opt/7130 */
void abort(void);
#define TYPE long

void scale(TYPE *alpha, TYPE *x, int n) {
  int i, ix;

  if (*alpha != 1)
    for (i = 0, ix = 0; i < n; i++, ix += 2) {
      TYPE tmpr, tmpi;
      tmpr      = *alpha * x[ix];
      tmpi      = *alpha * x[ix + 1];
      x[ix]     = tmpr;
      x[ix + 1] = tmpi;
    }
}

int main(void) {
  int  i;
  TYPE x[10];
  TYPE alpha = 2;

  for (i = 0; i < 10; i++)
    x[i] = i;

  scale(&alpha, x, 5);

  if (x[9] != 18)
    abort();

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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_scale:[0-9]+]] @scale(%[[VALUE_alpha:[0-9]+]] alpha: ptr<i64>, %[[VALUE_x:[0-9]+]] x: ptr<i64>, %[[VALUE_n:[0-9]+]] n: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ix:[0-9]+]] ix: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i64>(read<i64>(deref(read<ptr<i64>>(%[[VALUE_alpha]]))), widen<i64, reason=usual_arith>(const<i32>(1)))
// DEFAULT-NEXT:             for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_ix]], const<i32>(0));
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_n]]))
// DEFAULT-NEXT:                 increment: {
// DEFAULT-NEXT:                     let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                     let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_ix]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(2));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_ix]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                     yield void;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_tmpr:[0-9]+]] tmpr: i64 [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_tmpi:[0-9]+]] tmpi: i64 [storage=automatic];
// DEFAULT-NEXT:                         write<i64>(%[[VALUE_tmpr]], mul<i64, overflow=ub>(read<i64>(deref(read<ptr<i64>>(%[[VALUE_alpha]]))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE_x]]), read<i32>(%[[VALUE_ix]]))))));
// DEFAULT-NEXT:                         write<i64>(%[[VALUE_tmpi]], mul<i64, overflow=ub>(read<i64>(deref(read<ptr<i64>>(%[[VALUE_alpha]]))), read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE_x]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_ix]]), const<i32>(1)))))));
// DEFAULT-NEXT:                         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE_x]]), read<i32>(%[[VALUE_ix]]))), read<i64>(%[[VALUE_tmpr]]));
// DEFAULT-NEXT:                         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(read<ptr<i64>>(%[[VALUE_x]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_ix]]), const<i32>(1)))), read<i64>(%[[VALUE_tmpi]]));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: array<i64, 10> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_alpha_2:[0-9]+]] alpha: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(2));
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(10)>(%[[VALUE_x_2]]), read<i32>(%[[VALUE_i_2]]))), widen<i64, reason=assign>(read<i32>(%[[VALUE_i_2]])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i64>, ptr<i64>, i32) -> void>(%[[VALUE_scale]], addr_of<ptr<i64>>(%[[VALUE_alpha_2]]), array_decay<ptr<i64>, length=Some(10)>(%[[VALUE_x_2]]), const<i32>(5));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(10)>(%[[VALUE_x_2]]), const<i32>(9)))), widen<i64, reason=usual_arith>(const<i32>(18)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
