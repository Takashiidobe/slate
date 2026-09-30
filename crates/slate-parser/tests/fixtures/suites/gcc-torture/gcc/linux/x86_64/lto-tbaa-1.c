/* { dg-additional-options "-fno-early-inlining -fno-ipa-cp" }  */
struct a {
  float *b;
} *a;
struct b {
  int *b;
} b;
struct c {
  float *b;
}                              *c;
int                             d;
void                            use_a(struct a *a) {}
void                            set_b(int **a) { *a = &d; }
void                            use_c(struct c *a) {}
__attribute__((noinline)) int **retme(int **val) { return val; }
int                             e;
struct b                        b = {&e};
struct b                        b2;
struct b                        b3;
int                           **ptr = &b2.b;
int                             main(void) {
  a   = (void *)0;
  b.b = &e;
  ptr = retme(&b.b);
  set_b(ptr);
  b3 = b;
  if (b3.b != &d)
    __builtin_abort();
  c = (void *)0;
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
// DEFAULT-NEXT:     type @type[[TYPE_a:[0-9]+]] a = struct {
// DEFAULT-NEXT:         field0 b: ptr<f32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_b:[0-9]+]] b = struct {
// DEFAULT-NEXT:         field0 b: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_c:[0-9]+]] c = struct {
// DEFAULT-NEXT:         field0 b: ptr<f32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<@type[[TYPE_a]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: @type[[TYPE_b]] [storage=static] = aggregate<@type[[TYPE_b]], zero_fill=false>(field0 = addr_of<ptr<i32>>(%[[VALUE_e:[0-9]+]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_c]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b2:[0-9]+]] b2: @type[[TYPE_b]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b3:[0-9]+]] b3: @type[[TYPE_b]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_ptr:[0-9]+]] ptr: ptr<ptr<i32>> [storage=static] = addr_of<ptr<ptr<i32>>>(field0(%[[VALUE_b2]])) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_use_a:[0-9]+]] @use_a(%[[VALUE_a_2:[0-9]+]] a: ptr<@type[[TYPE_a]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_set_b:[0-9]+]] @set_b(%[[VALUE_a_3:[0-9]+]] a: ptr<ptr<i32>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE_a_3]])), addr_of<ptr<i32>>(%[[VALUE_d]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use_c:[0-9]+]] @use_c(%[[VALUE_a_4:[0-9]+]] a: ptr<@type[[TYPE_c]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_retme:[0-9]+]] @retme(%[[VALUE_val:[0-9]+]] val: ptr<ptr<i32>>) -> ptr<ptr<i32>> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<ptr<i32>>>(%[[VALUE_val]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_a]]>>(%[[VALUE_a]], null<ptr<@type[[TYPE_a]]>>);
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%[[VALUE_b]]), addr_of<ptr<i32>>(%[[VALUE_e]]));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%[[VALUE_ptr]], call<ptr<ptr<i32>>, signature=fn(ptr<ptr<i32>>) -> ptr<ptr<i32>>>(%[[VALUE_retme]], addr_of<ptr<ptr<i32>>>(field0(%[[VALUE_b]]))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>) -> void>(%[[VALUE_set_b]], read<ptr<ptr<i32>>>(%[[VALUE_ptr]]));
// DEFAULT-NEXT:         write<@type[[TYPE_b]]>(%[[VALUE_b3]], copy<@type[[TYPE_b]], reason=assign>(read<@type[[TYPE_b]]>(%[[VALUE_b]])));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(field0(%[[VALUE_b3]])), addr_of<ptr<i32>>(%[[VALUE_d]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_c]]>>(%[[VALUE_c]], null<ptr<@type[[TYPE_c]]>>);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
