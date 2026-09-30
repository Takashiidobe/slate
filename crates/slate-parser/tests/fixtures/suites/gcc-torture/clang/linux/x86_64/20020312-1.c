// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-do assemble } */
/* { dg-skip-if "" { pdp11-*-* } { "-O0" } { "" } } */

/* PR optimization/5892 */
typedef struct { unsigned long a; unsigned int b, c; } A;
typedef struct { unsigned long a; A *b; int c; } B;

static inline unsigned int
bar (unsigned int x)
{
  unsigned long r;
  asm ("" : "=r" (r) : "0" (x));
  return r >> 31;
}

int foo (B *x)
{
  A *y;
  y = x->b;
  y->b = bar (x->c);
  y->c = ({ unsigned int z = 1; (z << 24) | (z >> 24); });
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
// DEFAULT-NEXT:         field0 a: u64;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a: u64;
// DEFAULT-NEXT:         field1 b: ptr<@type[[TYPE0]]>;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = @type[[TYPE1]];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: u32) -> u32 [linkage=internal] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: u64 [storage=automatic];
// DEFAULT-NEXT:         asm "" [dialect=att] [options=pure,nomem,nostack] {
// DEFAULT-NEXT:             inlateout 0 "r" [reg] width 64 place<u64>(%[[VALUE_r]]) from read<u32>(%[[VALUE_x]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return truncate<u32, reason=return, fits=unknown>(shr<u64, amount_out_of_range=ub, fill=zero_extend>(read<u64>(%[[VALUE_r]]), const<i32>(31)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x_2:[0-9]+]] x: ptr<@type[[TYPE1]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_y:[0-9]+]] y: ptr<@type[[TYPE0]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE0]]>>(%[[VALUE_y]], read<ptr<@type[[TYPE0]]>>(field1(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_x_2]])))));
// DEFAULT-NEXT:         write<u32>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_y]]))), call<u32, signature=fn(u32) -> u32>(%[[VALUE_bar]], reinterpret<u32, reason=arg, fits=unknown>(read<i32>(field2(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_x_2]])))))));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_z:[0-9]+]] z: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:             write<u32>(%[[VALUE0]], or<u32>(shl<u32, overflow=wrap, amount_out_of_range=ub>(read<u32>(%[[VALUE_z]]), const<i32>(24)), shr<u32, amount_out_of_range=ub, fill=zero_extend>(read<u32>(%[[VALUE_z]]), const<i32>(24))));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<u32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_y]]))), read<u32>(%[[VALUE0]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
