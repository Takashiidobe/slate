// SLATE-FILECHECK-DEFINES DEFAULT

typedef struct { int c, d, e, f, g; } D;

void bar (unsigned long, unsigned long);
void foo (D *y)
{
  int x = 0;

  if (y->f == 0)
    x |= 0x1;
  if (y->g == 0)
    x |= 0x2;
  bar ((x << 16) | (y->c & 0xffff), (y->d << 16) | (y->e & 0xffff));
}

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
// DEFAULT-NEXT:         field0 c: i32;
// DEFAULT-NEXT:         field1 d: i32;
// DEFAULT-NEXT:         field2 e: i32;
// DEFAULT-NEXT:         field3 f: i32;
// DEFAULT-NEXT:         field4 g: i32;
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8, 12, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_D:[0-9]+]] D = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: u64, %[[VALUE1:[0-9]+]] <unnamed>: u64) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_y:[0-9]+]] y: ptr<@type[[TYPE0]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field3(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_y]])))), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(field4(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_y]])))), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE4]]), const<i32>(2));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         call<void, signature=fn(u64, u64) -> void>(%[[VALUE_bar]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_x]]), const<i32>(16)), and<i32>(read<i32>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_y]])))), const<i32>(65535))))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_y]])))), const<i32>(16)), and<i32>(read<i32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_y]])))), const<i32>(65535))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
