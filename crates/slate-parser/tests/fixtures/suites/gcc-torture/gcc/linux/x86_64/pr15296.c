/* PR optimization/15296.  The delayed-branch scheduler caused code that
   SEGV:d for CRIS; a register was set to -1 in a delay-slot for the
   fall-through code, while that register held a pointer used in code at
   the branch target.  */

void abort(void);
void exit(int);

typedef __INTPTR_TYPE__ intptr_t;
typedef intptr_t        W;
union u0 {
  union u0 *r;
  W         i;
};
struct s1 {
  union u0 **m0;
  union u0   m1[4];
};

void f(void *, struct s1 *, const union u0 *, W, W, W)
    __attribute__((__noinline__));
void g(void *, char *) __attribute__((__noinline__));

void f(void *a, struct s1 *b, const union u0 *h, W v0, W v1, W v4) {
  union u0  *e  = 0;
  union u0  *k  = 0;
  union u0 **v5 = b->m0;
  union u0  *c  = b->m1;
  union u0 **d  = &v5[0];
l0:;
  if (v0 < v1)
    goto l0;
  if (v0 == 0)
    goto l3;
  v0 = v4;
  if (v0 != 0)
    goto l3;
  c[0].r = *d;
  v1     = -1;
  e      = c[0].r;
  if (e != 0)
    g(a, "");
  k    = e + 3;
  k->i = v1;
  goto l4;
l3:;
  c[0].i = v0;
  e      = c[1].r;
  if (e != 0)
    g(a, "");
  e = c[0].r;
  if (e == 0)
    g(a, "");
  k    = e + 2;
  k->r = c[1].r;
l4:;
}

void g(void *a, char *b) { abort(); }

int main() {
  union u0  uv[] = {{.i = 111}, {.i = 222}, {.i = 333}, {.i = 444}};
  struct s1 s    = {0, {{.i = 555}, {.i = 0}, {.i = 999}, {.i = 777}}};
  f(0, &s, 0, 20000, 10000, (W)uv);
  if (s.m1[0].i != (W)uv || s.m1[1].i != 0 || s.m1[2].i != 999 ||
      s.m1[3].i != 777 || uv[0].i != 111 || uv[1].i != 222 || uv[2].i != 0 ||
      uv[3].i != 444)
    abort();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_intptr_t:[0-9]+]] intptr_t = i64;
// DEFAULT-NEXT:     type @type[[TYPE_W:[0-9]+]] W = i64;
// DEFAULT-NEXT:     type @type[[TYPE_u0:[0-9]+]] u0 = union {
// DEFAULT-NEXT:         field0 r: ptr<@type[[TYPE_u0]]>;
// DEFAULT-NEXT:         field1 i: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_s1:[0-9]+]] s1 = struct {
// DEFAULT-NEXT:         field0 m0: ptr<ptr<@type[[TYPE_u0]]>>;
// DEFAULT-NEXT:         field1 m1: array<@type[[TYPE_u0]], 4>;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_a:[0-9]+]] a: ptr<void>, %[[VALUE_b:[0-9]+]] b: ptr<@type[[TYPE_s1]]>, %[[VALUE_h:[0-9]+]] h: ptr<const @type[[TYPE_u0]]>, %[[VALUE_v0:[0-9]+]] v0: i64, %[[VALUE_v1:[0-9]+]] v1: i64, %[[VALUE_v4:[0-9]+]] v4: i64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_e:[0-9]+]] e: ptr<@type[[TYPE_u0]]> [storage=automatic] = null<ptr<@type[[TYPE_u0]]>>;
// DEFAULT-NEXT:         let %[[VALUE_k:[0-9]+]] k: ptr<@type[[TYPE_u0]]> [storage=automatic] = null<ptr<@type[[TYPE_u0]]>>;
// DEFAULT-NEXT:         let %[[VALUE_v5:[0-9]+]] v5: ptr<ptr<@type[[TYPE_u0]]>> [storage=automatic] = read<ptr<ptr<@type[[TYPE_u0]]>>>(field0(deref(read<ptr<@type[[TYPE_s1]]>>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_u0]]> [storage=automatic] = array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(field1(deref(read<ptr<@type[[TYPE_s1]]>>(%[[VALUE_b]]))));
// DEFAULT-NEXT:         let %[[VALUE_d:[0-9]+]] d: ptr<ptr<@type[[TYPE_u0]]>> [storage=automatic] = addr_of<ptr<ptr<@type[[TYPE_u0]]>>>(deref(ptr_offset<ptr<ptr<@type[[TYPE_u0]]>>, subtract=false, element=ptr<@type[[TYPE_u0]]>, overflow=ub>(read<ptr<ptr<@type[[TYPE_u0]]>>>(%[[VALUE_v5]]), const<i32>(0))));
// DEFAULT-NEXT:         label %[[VALUE_l0:[0-9]+]] l0:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%[[VALUE_v0]]), read<i64>(%[[VALUE_v1]]))
// DEFAULT-NEXT:             goto %[[VALUE_l0]];
// DEFAULT-NEXT:         if eq<i64>(read<i64>(%[[VALUE_v0]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             goto %[[VALUE_l3:[0-9]+]];
// DEFAULT-NEXT:         write<i64>(%[[VALUE_v0]], read<i64>(%[[VALUE_v4]]));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_v0]]), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             goto %[[VALUE_l3]];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_u0]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_c]]), const<i32>(0)))), read<ptr<@type[[TYPE_u0]]>>(deref(read<ptr<ptr<@type[[TYPE_u0]]>>>(%[[VALUE_d]]))));
// DEFAULT-NEXT:         write<i64>(%[[VALUE_v1]], widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_u0]]>>(%[[VALUE_e]], read<ptr<@type[[TYPE_u0]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_c]]), const<i32>(0))))));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_u0]]>>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_e]]), null<ptr<@type[[TYPE_u0]]>>)
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, ptr<i8>) -> void>(%[[VALUE_g:[0-9]+]], read<ptr<void>>(%[[VALUE_a]]), array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_u0]]>>(%[[VALUE_k]], ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_e]]), const<i32>(3)));
// DEFAULT-NEXT:         write<i64>(field1(deref(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_k]]))), read<i64>(%[[VALUE_v1]]));
// DEFAULT-NEXT:         goto %[[VALUE_l4:[0-9]+]];
// DEFAULT-NEXT:         label %[[VALUE_l3]] l3:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         write<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_c]]), const<i32>(0)))), read<i64>(%[[VALUE_v0]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_u0]]>>(%[[VALUE_e]], read<ptr<@type[[TYPE_u0]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_c]]), const<i32>(1))))));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_u0]]>>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_e]]), null<ptr<@type[[TYPE_u0]]>>)
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, ptr<i8>) -> void>(%[[VALUE_g]], read<ptr<void>>(%[[VALUE_a]]), array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_2]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_u0]]>>(%[[VALUE_e]], read<ptr<@type[[TYPE_u0]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_c]]), const<i32>(0))))));
// DEFAULT-NEXT:         if eq<ptr<@type[[TYPE_u0]]>>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_e]]), null<ptr<@type[[TYPE_u0]]>>)
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, ptr<i8>) -> void>(%[[VALUE_g]], read<ptr<void>>(%[[VALUE_a]]), array_decay<ptr<i8>, length=Some(1)>(%[[VALUE_str_3]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_u0]]>>(%[[VALUE_k]], ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_e]]), const<i32>(2)));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_u0]]>>(field0(deref(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_k]]))), read<ptr<@type[[TYPE_u0]]>>(field0(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(read<ptr<@type[[TYPE_u0]]>>(%[[VALUE_c]]), const<i32>(1))))));
// DEFAULT-NEXT:         label %[[VALUE_l4]] l4:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_g]] @g(%[[VALUE_a_2:[0-9]+]] a: ptr<void>, %[[VALUE_b_2:[0-9]+]] b: ptr<i8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_uv:[0-9]+]] uv: array<@type[[TYPE_u0]], 4> [storage=automatic] [align=16] = aggregate<array<@type[[TYPE_u0]], 4>, zero_fill=false>(index0 = aggregate<@type[[TYPE_u0]], zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(111))), index1 = aggregate<@type[[TYPE_u0]], zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(222))), index2 = aggregate<@type[[TYPE_u0]], zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(333))), index3 = aggregate<@type[[TYPE_u0]], zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(444))));
// DEFAULT-NEXT:         let %[[VALUE_s:[0-9]+]] s: @type[[TYPE_s1]] [storage=automatic] = aggregate<@type[[TYPE_s1]], zero_fill=false>(field0 = null<ptr<ptr<@type[[TYPE_u0]]>>>, field1 = aggregate<array<@type[[TYPE_u0]], 4>, zero_fill=false>(index0 = aggregate<@type[[TYPE_u0]], zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(555))), index1 = aggregate<@type[[TYPE_u0]], zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(0))), index2 = aggregate<@type[[TYPE_u0]], zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(999))), index3 = aggregate<@type[[TYPE_u0]], zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(777)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<@type[[TYPE_s1]]>, ptr<const @type[[TYPE_u0]]>, i64, i64, i64) -> void>(%[[VALUE_f]], null<ptr<void>>, addr_of<ptr<@type[[TYPE_s1]]>>(%[[VALUE_s]]), null<ptr<const @type[[TYPE_u0]]>>, widen<i64, reason=arg>(const<i32>(20000)), widen<i64, reason=arg>(const<i32>(10000)), ptr_to_int<i64, reason=explicit>(array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(%[[VALUE_uv]])));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(field1(%[[VALUE_s]])), const<i32>(0))))), ptr_to_int<i64, reason=explicit>(array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(%[[VALUE_uv]]))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(field1(%[[VALUE_s]])), const<i32>(1))))), widen<i64, reason=usual_arith>(const<i32>(0)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(field1(%[[VALUE_s]])), const<i32>(2))))), widen<i64, reason=usual_arith>(const<i32>(999)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(field1(%[[VALUE_s]])), const<i32>(3))))), widen<i64, reason=usual_arith>(const<i32>(777)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(%[[VALUE_uv]]), const<i32>(0))))), widen<i64, reason=usual_arith>(const<i32>(111)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(%[[VALUE_uv]]), const<i32>(1))))), widen<i64, reason=usual_arith>(const<i32>(222)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(%[[VALUE_uv]]), const<i32>(2))))), widen<i64, reason=usual_arith>(const<i32>(0)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type[[TYPE_u0]]>, subtract=false, element=@type[[TYPE_u0]], overflow=ub>(array_decay<ptr<@type[[TYPE_u0]]>, length=Some(4)>(%[[VALUE_uv]]), const<i32>(3))))), widen<i64, reason=usual_arith>(const<i32>(444))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
