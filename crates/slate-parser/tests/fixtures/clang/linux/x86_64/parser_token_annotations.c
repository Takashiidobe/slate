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
// DEFAULT-NEXT:     comment leading "/* file comment */" [spelling=[[#FILE0:]]:0+18, expansion={{[0-9]+}}:0+18];
// DEFAULT-NEXT:     comment detached "/* trailing comment */" [spelling=[[#FILE0]]:434+22, expansion=[[#FILE0]]:434+22];
// DEFAULT-NEXT:     comment leading "/* field comment */" [spelling=[[#FILE0]]:43+19, expansion=[[#FILE0]]:43+19];
// DEFAULT-NEXT:     comment leading "/* enumerator comment */" [spelling=[[#FILE0]]:99+24, expansion=[[#FILE0]]:99+24];
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
// DEFAULT-NEXT:     type @type[[TYPE_Item:[0-9]+]] Item = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Item_2:[0-9]+]] Item = @type[[TYPE_Item]];
// DEFAULT-NEXT:     type @type[[TYPE_State:[0-9]+]] State = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_READY:[0-9]+]] READY = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_DONE:[0-9]+]] DONE = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     fn %[[VALUE_read_item:[0-9]+]] @read_item(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_Item]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         comment leading "/* function comment */" [spelling=[[#FILE0]]:170+22, expansion=[[#FILE0]]:170+22];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             comment leading "/* empty block comment */" [spelling=[[#FILE0]]:201+25, expansion=[[#FILE0]]:201+25];
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             comment leading "/* statement expression comment */" [spelling=[[#FILE0]]:283+34, expansion=[[#FILE0]]:283+34];
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], read<i32>(field0(deref(read<ptr<@type[[TYPE_Item]]>>(%[[VALUE_p]])))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE0]]));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
