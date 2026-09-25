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
// DEFAULT-NEXT:     type @type0 a = struct {
// DEFAULT-NEXT:         field0 b: ptr<f32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 b = struct {
// DEFAULT-NEXT:         field0 b: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type2 c = struct {
// DEFAULT-NEXT:         field0 b: ptr<f32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %1 a: ptr<@type0> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = addr_of<ptr<i32>>(%15)) [linkage=external];
// DEFAULT-NEXT:     global %5 c: ptr<@type2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %6 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %15 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %16 b2: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %17 b3: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %18 ptr: ptr<ptr<i32>> [storage=static] = addr_of<ptr<ptr<i32>>>(field0(%16)) [linkage=external];
// DEFAULT-NEXT:     fn %7 @use_a(%8 a: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @set_b(%10 a: ptr<ptr<i32>>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%10)), addr_of<ptr<i32>>(%6));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @use_c(%12 a: ptr<@type2>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @retme(%14 val: ptr<ptr<i32>>) -> ptr<ptr<i32>> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<ptr<i32>>>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<ptr<@type0>>(%1, null<ptr<@type0>>);
// DEFAULT-NEXT:         write<ptr<i32>>(field0(%3), addr_of<ptr<i32>>(%15));
// DEFAULT-NEXT:         write<ptr<ptr<i32>>>(%18, call<ptr<ptr<i32>>, signature=fn(ptr<ptr<i32>>) -> ptr<ptr<i32>>>(%13, addr_of<ptr<ptr<i32>>>(field0(%3))));
// DEFAULT-NEXT:         call<ptr<ptr<i32>>, signature=fn(ptr<ptr<i32>>) -> ptr<ptr<i32>>>(%13, addr_of<ptr<ptr<i32>>>(field0(%3)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<ptr<i32>>) -> void>(%9, read<ptr<ptr<i32>>>(%18));
// DEFAULT-NEXT:         write<@type1>(%17, copy<@type1, reason=assign>(read<@type1>(%3)));
// DEFAULT-NEXT:         if ne<ptr<i32>>(read<ptr<i32>>(field0(%17)), addr_of<ptr<i32>>(%6))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<ptr<@type2>>(%5, null<ptr<@type2>>);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
