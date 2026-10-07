/*
** file prologue
*/
#include <stddef.h>

/* section */

/* leading record */
struct record {
  int a;      /* trailing a */
  /* leading b */
  int b;
  /* dangling */
};

enum state {
  READY,   /* trailing READY */
  DONE     /* trailing DONE */
};

#define LIMIT 4 /* directive */
/** explicit doc */
int g;            /* trailing g starts
                  ** and continues */
/**/
int h;

/*
** guarded doc
*/
#ifndef NOT_DEFINED
int guarded(void);
#endif /* NOT_DEFINED */

int call(int, int);

/* leading f */
static int f(
  int x,     /* param x */
  int y      /* param y */
){
  int r;     /* trailing r */
  r = x /* interior */ + y;
  /* leading return */
  return call(r, /* argument */
              LIMIT);
}

int use_f(void) { return f(1, 2) + g + h; }
/* eof */

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-ARGS --show-comments

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     comment detached "/*\n** file prologue\n*/" [spelling=[[#FILE0:]]:0+22, expansion={{[0-9]+}}:0+22];
// DEFAULT-NEXT:     comment detached "/* section */" [spelling=[[#FILE0]]:44+13, expansion=[[#FILE0]]:44+13];
// DEFAULT-NEXT:     comment detached "/* directive */" [spelling=[[#FILE0]]:271+15, expansion=[[#FILE0]]:271+15];
// DEFAULT-NEXT:     comment detached "/* eof */" [spelling=[[#FILE0]]:773+9, expansion=[[#FILE0]]:773+9];
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
// DEFAULT-NEXT:     comment leading "/* leading record */" [spelling=[[#FILE0]]:59+20, expansion=[[#FILE0]]:59+20];
// DEFAULT-NEXT:     comment detached "/* dangling */" [spelling=[[#FILE0]]:156+14, expansion=[[#FILE0]]:156+14];
// DEFAULT-NEXT:     type @type[[TYPE_record:[0-9]+]] record = struct {
// DEFAULT-NEXT:         comment trailing "/* trailing a */" [spelling=[[#FILE0]]:110+16, expansion=[[#FILE0]]:110+16];
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         comment leading "/* leading b */" [spelling=[[#FILE0]]:129+15, expansion=[[#FILE0]]:129+15];
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_state:[0-9]+]] state = enum : u32 {
// DEFAULT-NEXT:         comment trailing "/* trailing READY */" [spelling=[[#FILE0]]:199+20, expansion=[[#FILE0]]:199+20];
// DEFAULT-NEXT:         %[[VALUE_READY:[0-9]+]] READY = const<i32>(0);
// DEFAULT-NEXT:         comment trailing "/* trailing DONE */" [spelling=[[#FILE0]]:231+19, expansion=[[#FILE0]]:231+19];
// DEFAULT-NEXT:         %[[VALUE_DONE:[0-9]+]] DONE = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     comment leading doc "/** explicit doc */" [spelling=[[#FILE0]]:287+19, expansion=[[#FILE0]]:287+19];
// DEFAULT-NEXT:     comment trailing "/* trailing g starts\n                  ** and continues */" [spelling=[[#FILE0]]:325+58, expansion=[[#FILE0]]:325+58];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     comment leading "/**/" [spelling=[[#FILE0]]:384+4, expansion=[[#FILE0]]:384+4];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     comment leading "/*\n** guarded doc\n*/" [spelling=[[#FILE0]]:397+20, expansion=[[#FILE0]]:397+20];
// DEFAULT-NEXT:     fn %[[VALUE_guarded:[0-9]+]] @guarded() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_call:[0-9]+]] @call(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE1:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     comment leading "/* leading f */" [spelling=[[#FILE0]]:504+15, expansion=[[#FILE0]]:504+15];
// DEFAULT-NEXT:     param x comment trailing "/* param x */" [spelling=[[#FILE0]]:547+13, expansion=[[#FILE0]]:547+13];
// DEFAULT-NEXT:     param y comment trailing "/* param y */" [spelling=[[#FILE0]]:574+13, expansion=[[#FILE0]]:574+13];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y:[0-9]+]] y: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:         comment trailing "/* trailing r */" [spelling=[[#FILE0]]:604+16, expansion=[[#FILE0]]:604+16];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_r]], add<i32, overflow=ub>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y]])));
// DEFAULT-NEXT:         comment trailing "/* interior */" [spelling=[[#FILE0]]:629+14, expansion=[[#FILE0]]:629+14];
// DEFAULT-NEXT:         comment leading "/* leading return */" [spelling=[[#FILE0]]:651+20, expansion=[[#FILE0]]:651+20];
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_call]], read<i32>(%[[VALUE_r]]), const<i32>(4));
// DEFAULT-NEXT:         comment trailing "/* argument */" [spelling=[[#FILE0]]:689+14, expansion=[[#FILE0]]:689+14];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use_f:[0-9]+]] @use_f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(i32, i32) -> i32>(%[[VALUE_f]], const<i32>(1), const<i32>(2)), read<i32>(%[[VALUE_g]])), read<i32>(%[[VALUE_h]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
