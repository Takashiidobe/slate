void abort(void);
void exit(int);

typedef struct {
  double epsilon;
} material_type;

material_type foo(double x) {
  material_type m;

  m.epsilon = 1.0 + x;
  return m;
}

int main(void) {
  int           i;
  material_type x;

  /* We must iterate enough times to overflow the FP stack on the
     x86.  */
  for (i = 0; i < 10; i++) {
    x = foo(1.0);
    if (x.epsilon != 1.0 + 1.0)
      abort();
  }

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
// DEFAULT-NEXT:         field0 epsilon: f64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_material_type:[0-9]+]] material_type = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: f64) -> @type[[TYPE0]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_m:[0-9]+]] m: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<f64>(field0(%[[VALUE_m]]), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), read<f64>(%[[VALUE_x]])));
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_m]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(10))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<@type[[TYPE0]]>(%[[VALUE_x_2]], copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(f64) -> @type[[TYPE0]], abi=sysv64(scalar) -> native_c>(%[[VALUE_foo]], const<f64>(1.0))));
// DEFAULT-NEXT:                     if ne<f64, exceptions=ignore>(read<f64>(field0(%[[VALUE_x_2]])), add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(1.0), const<f64>(1.0)))
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
