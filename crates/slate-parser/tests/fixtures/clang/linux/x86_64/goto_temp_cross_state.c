// { dg-additional-options "-O2" }
#include <stdio.h>

typedef struct {
  int pad;
  int x;
  int y;
} inner_t;

typedef struct {
  int     lead;
  inner_t in;
} outer_t;

typedef struct {
  outer_t *dict;
} state_t;

int compute(const state_t *const ms, int flag) {
  const outer_t *const o   = ms->dict;
  const inner_t *const q   = &o->in;
  int                  acc = q->x;
  if (flag) {
    goto second;
  }
  acc += 100;
second:
  acc += q->y;
  return acc;
}

int main(void) {
  inner_t inr;
  inr.pad = 0;
  inr.x   = 3;
  inr.y   = 4;
  outer_t ou;
  ou.lead = 0;
  ou.in   = inr;
  state_t s;
  s.dict = &ou;
  printf("%d %d\n", compute(&s, 0), compute(&s, 1));
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 pad: i32;
// DEFAULT-NEXT:         field1 x: i32;
// DEFAULT-NEXT:         field2 y: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 inner_t = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 lead: i32;
// DEFAULT-NEXT:         field1 in: @type0;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type3 outer_t = @type2;
// DEFAULT-NEXT:     type @type4 = struct {
// DEFAULT-NEXT:         field0 dict: ptr<@type2>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type5 state_t = @type4;
// DEFAULT-NEXT:     global %20 .str20: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%19 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %8 @compute(%10 ms: ptr<const @type4> [const], %11 flag: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %12 o: ptr<const @type2> [storage=automatic] [const] = pointer_cast<ptr<const @type2>, reason=assign>(read<ptr<@type2>>(field0(deref(read<ptr<const @type4>>(%10)))));
// DEFAULT-NEXT:         let %13 q: ptr<const @type0> [storage=automatic] [const] = addr_of<ptr<const @type0>>(field1(deref(read<ptr<const @type2>>(%12))));
// DEFAULT-NEXT:         let %14 acc: i32 [storage=automatic] = read<i32>(field1(deref(read<ptr<const @type0>>(%13))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%11), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 goto %9;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %21: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:         let %22: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%21), const<i32>(100));
// DEFAULT-NEXT:         write<i32>(%14, read<i32>(%22));
// DEFAULT-NEXT:         label %9 second:
// DEFAULT-NEXT:             let %23: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:             let %24: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%23), read<i32>(field2(deref(read<ptr<const @type0>>(%13)))));
// DEFAULT-NEXT:             write<i32>(%14, read<i32>(%24));
// DEFAULT-NEXT:         return read<i32>(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %16 inr: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%16), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field1(%16), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field2(%16), const<i32>(4));
// DEFAULT-NEXT:         let %17 ou: @type2 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%17), const<i32>(0));
// DEFAULT-NEXT:         write<@type0>(field1(%17), copy<@type0, reason=assign>(read<@type0>(%16)));
// DEFAULT-NEXT:         let %18 s: @type4 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type2>>(field0(%18), addr_of<ptr<@type2>>(%17));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%20)), call<i32, signature=fn(ptr<const @type4>, i32) -> i32>(%8, pointer_cast<ptr<const @type4>, reason=arg>(addr_of<ptr<@type4>>(%18)), const<i32>(0)), call<i32, signature=fn(ptr<const @type4>, i32) -> i32>(%8, pointer_cast<ptr<const @type4>, reason=arg>(addr_of<ptr<@type4>>(%18)), const<i32>(1)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
