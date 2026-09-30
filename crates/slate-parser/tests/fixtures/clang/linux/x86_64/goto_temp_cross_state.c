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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 pad: i32;
// DEFAULT-NEXT:         field1 x: i32;
// DEFAULT-NEXT:         field2 y: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_inner_t:[0-9]+]] inner_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 lead: i32;
// DEFAULT-NEXT:         field1 in: @type[[TYPE0]];
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_outer_t:[0-9]+]] outer_t = @type[[TYPE1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 dict: ptr<@type[[TYPE1]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_state_t:[0-9]+]] state_t = @type[[TYPE2]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_compute:[0-9]+]] @compute(%[[VALUE_ms:[0-9]+]] ms: ptr<const @type[[TYPE2]]> [const], %[[VALUE_flag:[0-9]+]] flag: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: ptr<const @type[[TYPE1]]> [storage=automatic] [const] = pointer_cast<ptr<const @type[[TYPE1]]>, reason=assign>(read<ptr<@type[[TYPE1]]>>(field0(deref(read<ptr<const @type[[TYPE2]]>>(%[[VALUE_ms]])))));
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<const @type[[TYPE0]]> [storage=automatic] [const] = addr_of<ptr<const @type[[TYPE0]]>>(field1(deref(read<ptr<const @type[[TYPE1]]>>(%[[VALUE_o]]))));
// DEFAULT-NEXT:         let %[[VALUE_acc:[0-9]+]] acc: i32 [storage=automatic] = read<i32>(field1(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_q]]))));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_flag]]), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 goto %[[VALUE_second:[0-9]+]];
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_acc]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(100));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_acc]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         label %[[VALUE_second]] second:
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_acc]]);
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), read<i32>(field2(deref(read<ptr<const @type[[TYPE0]]>>(%[[VALUE_q]])))));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_acc]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_acc]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_inr:[0-9]+]] inr: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_inr]]), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field1(%[[VALUE_inr]]), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_inr]]), const<i32>(4));
// DEFAULT-NEXT:         let %[[VALUE_ou:[0-9]+]] ou: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%[[VALUE_ou]]), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(field1(%[[VALUE_ou]]), copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(%[[VALUE_inr]])));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE2]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE1]]>>(field0(%[[VALUE_s]]), addr_of<ptr<@type[[TYPE1]]>>(%[[VALUE_ou]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), call<i32, signature=fn(ptr<const @type[[TYPE2]]>, i32) -> i32>(%[[VALUE_compute]], pointer_cast<ptr<const @type[[TYPE2]]>, reason=arg>(addr_of<ptr<@type[[TYPE2]]>>(%[[VALUE_s]])), const<i32>(0)), call<i32, signature=fn(ptr<const @type[[TYPE2]]>, i32) -> i32>(%[[VALUE_compute]], pointer_cast<ptr<const @type[[TYPE2]]>, reason=arg>(addr_of<ptr<@type[[TYPE2]]>>(%[[VALUE_s]])), const<i32>(1)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
