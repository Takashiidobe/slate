/* file comment */
typedef struct Item {
  /* field comment */
  int value;
} Item;
enum State {
  /* enumerator comment */
  READY,
  DONE
};
int read_item(Item *p) {
  /* function comment */
  {
    /* empty block comment */
    _Pragma("STDC FENV_ACCESS ON")
  }
  int x = ({
    /* statement expression comment */
    _Pragma("STDC FP_CONTRACT OFF")
    p->value;
  });
  return x +
    _Pragma("GCC diagnostic push")
    DONE;
}
/* trailing comment */

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --show-comments

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
// DEFAULT-NEXT:     type @type0 Item = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 Item = @type0;
// DEFAULT-NEXT:     type @type2 State = enum : u32 {
// DEFAULT-NEXT:         %0 READY = const<i32>(0);
// DEFAULT-NEXT:         %1 DONE = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %5 @read_item(%6 p: ptr<@type0>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %7 x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %8: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             write<i32>(%8, read<i32>(field0(deref(read<ptr<@type0>>(%6)))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%8));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%7), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
