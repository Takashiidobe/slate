// SLATE-FILECHECK-DEFINES DEFAULT

struct a
{
  struct a * x;
};

void
foo (struct a * b)
{
  int i;

  for (i = 0; i < 1000; i++)
    {
      b->x = b;
      b++;
    }
}

void
bar (struct a * b)
{
  int i;

  for (i = 0; i < 1000; i++)
    {
      b->x = b;
      b--;
    }
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
// DEFAULT-NEXT:     type @type[[TYPE_a:[0-9]+]] a = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type[[TYPE_a]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_a]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(1000))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_a]]>>(field0(deref(read<ptr<@type[[TYPE_a]]>>(%[[VALUE_b]]))), read<ptr<@type[[TYPE_a]]>>(%[[VALUE_b]]));
// DEFAULT-NEXT:                     let %[[VALUE3:[0-9]+]]: ptr<@type[[TYPE_a]]> [synthetic] = read<ptr<@type[[TYPE_a]]>>(%[[VALUE_b]]);
// DEFAULT-NEXT:                     let %[[VALUE4:[0-9]+]]: ptr<@type[[TYPE_a]]> [synthetic] = ptr_offset<ptr<@type[[TYPE_a]]>, subtract=false, element=@type[[TYPE_a]], overflow=ub>(read<ptr<@type[[TYPE_a]]>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_a]]>>(%[[VALUE_b]], read<ptr<@type[[TYPE_a]]>>(%[[VALUE4]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_b_2:[0-9]+]] b: ptr<@type[[TYPE_a]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), const<i32>(1000))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_a]]>>(field0(deref(read<ptr<@type[[TYPE_a]]>>(%[[VALUE_b_2]]))), read<ptr<@type[[TYPE_a]]>>(%[[VALUE_b_2]]));
// DEFAULT-NEXT:                     let %[[VALUE8:[0-9]+]]: ptr<@type[[TYPE_a]]> [synthetic] = read<ptr<@type[[TYPE_a]]>>(%[[VALUE_b_2]]);
// DEFAULT-NEXT:                     let %[[VALUE9:[0-9]+]]: ptr<@type[[TYPE_a]]> [synthetic] = ptr_offset<ptr<@type[[TYPE_a]]>, subtract=true, element=@type[[TYPE_a]], overflow=ub>(read<ptr<@type[[TYPE_a]]>>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_a]]>>(%[[VALUE_b_2]], read<ptr<@type[[TYPE_a]]>>(%[[VALUE9]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
