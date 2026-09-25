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
// DEFAULT-NEXT:     type @type0 a = struct {
// DEFAULT-NEXT:         field0 x: ptr<@type0>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     fn %1 @foo(%2 b: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %7
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%3, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%3), const<i32>(1000))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%3);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%3, read<i32>(%10));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%2))), read<ptr<@type0>>(%2));
// DEFAULT-NEXT:                     let %11: ptr<@type0> [synthetic] = read<ptr<@type0>>(%2);
// DEFAULT-NEXT:                     let %12: ptr<@type0> [synthetic] = ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%11), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<@type0>>(%2, read<ptr<@type0>>(%12));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @bar(%5 b: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %6 i: i32 [storage=automatic];
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%6, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), const<i32>(1000))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %13: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %14: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%13), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%14));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<ptr<@type0>>(field0(deref(read<ptr<@type0>>(%5))), read<ptr<@type0>>(%5));
// DEFAULT-NEXT:                     let %15: ptr<@type0> [synthetic] = read<ptr<@type0>>(%5);
// DEFAULT-NEXT:                     let %16: ptr<@type0> [synthetic] = ptr_offset<ptr<@type0>, subtract=true, element=@type0, overflow=ub>(read<ptr<@type0>>(%15), const<i32>(1));
// DEFAULT-NEXT:                     write<ptr<@type0>>(%5, read<ptr<@type0>>(%16));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
