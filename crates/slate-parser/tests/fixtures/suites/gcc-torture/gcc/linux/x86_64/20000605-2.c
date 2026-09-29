void abort(void);
void exit(int);

struct F {
  int i;
};

void f1(struct F *x, struct F *y) {
  int timeout = 0;
  for (; ((const struct F *)x)->i < y->i; x->i++)
    if (++timeout > 5)
      abort();
}

int main(void) {
  struct F x, y;
  x.i = 0;
  y.i = 1;
  f1(&x, &y);
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
// DEFAULT-NEXT:     type @type[[TYPE_F:[0-9]+]] F = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_F]]>, %[[VALUE_y:[0-9]+]] y: ptr<@type[[TYPE_F]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_timeout:[0-9]+]] timeout: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(field0(deref(pointer_cast<ptr<const @type[[TYPE_F]]>, reason=explicit>(read<ptr<@type[[TYPE_F]]>>(%[[VALUE_x]]))))), read<i32>(field0(deref(read<ptr<@type[[TYPE_F]]>>(%[[VALUE_y]])))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<@type[[TYPE_F]]> [synthetic] = read<ptr<@type[[TYPE_F]]>>(%[[VALUE_x]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(field0(deref(read<ptr<@type[[TYPE_F]]>>(%[[VALUE2]]))));
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(field0(deref(read<ptr<@type[[TYPE_F]]>>(%[[VALUE2]]))), read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_timeout]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_timeout]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 if gt<i32>(read<i32>(%[[VALUE6]]), const<i32>(5))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: @type[[TYPE_F]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: @type[[TYPE_F]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_x_2]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_y_2]]), const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_F]]>, ptr<@type[[TYPE_F]]>) -> void>(%[[VALUE_f1]], addr_of<ptr<@type[[TYPE_F]]>>(%[[VALUE_x_2]]), addr_of<ptr<@type[[TYPE_F]]>>(%[[VALUE_y_2]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
