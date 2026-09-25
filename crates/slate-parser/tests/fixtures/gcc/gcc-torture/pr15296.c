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
// DEFAULT-NEXT:     type @type0 intptr_t = i64;
// DEFAULT-NEXT:     type @type1 W = i64;
// DEFAULT-NEXT:     type @type2 u0 = union {
// DEFAULT-NEXT:         field0 r: ptr<@type2>;
// DEFAULT-NEXT:         field1 i: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type3 s1 = struct {
// DEFAULT-NEXT:         field0 m0: ptr<ptr<@type2>>;
// DEFAULT-NEXT:         field1 m1: array<@type2, 4>;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %36 .str36: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %37 .str37: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     global %38 .str38: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%27 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @f(%11 a: ptr<void>, %12 b: ptr<@type3>, %13 h: ptr<const @type2>, %14 v0: i64, %15 v1: i64, %16 v4: i64) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %17 e: ptr<@type2> [storage=automatic] = null<ptr<@type2>>;
// DEFAULT-NEXT:         let %18 k: ptr<@type2> [storage=automatic] = null<ptr<@type2>>;
// DEFAULT-NEXT:         let %19 v5: ptr<ptr<@type2>> [storage=automatic] = read<ptr<ptr<@type2>>>(field0(deref(read<ptr<@type3>>(%12))));
// DEFAULT-NEXT:         let %20 c: ptr<@type2> [storage=automatic] = array_decay<ptr<@type2>, length=Some(4)>(field1(deref(read<ptr<@type3>>(%12))));
// DEFAULT-NEXT:         let %21 d: ptr<ptr<@type2>> [storage=automatic] = addr_of<ptr<ptr<@type2>>>(deref(ptr_offset<ptr<ptr<@type2>>, subtract=false, element=ptr<@type2>, overflow=ub>(read<ptr<ptr<@type2>>>(%19), const<i32>(0))));
// DEFAULT-NEXT:         label %8 l0:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         if lt<i64>(read<i64>(%14), read<i64>(%15))
// DEFAULT-NEXT:             goto %8;
// DEFAULT-NEXT:         if eq<i64>(read<i64>(%14), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             goto %9;
// DEFAULT-NEXT:         write<i64>(%14, read<i64>(%16));
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%14), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             goto %9;
// DEFAULT-NEXT:         write<ptr<@type2>>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(0)))), read<ptr<@type2>>(deref(read<ptr<ptr<@type2>>>(%21))));
// DEFAULT-NEXT:         write<i64>(%15, widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<ptr<@type2>>(%17, read<ptr<@type2>>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(0))))));
// DEFAULT-NEXT:         if ne<ptr<@type2>>(read<ptr<@type2>>(%17), null<ptr<@type2>>)
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, ptr<i8>) -> void>(%7, read<ptr<void>>(%11), array_decay<ptr<i8>, length=Some(1)>(%36));
// DEFAULT-NEXT:         write<ptr<@type2>>(%18, ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%17), const<i32>(3)));
// DEFAULT-NEXT:         write<i64>(field1(deref(read<ptr<@type2>>(%18))), read<i64>(%15));
// DEFAULT-NEXT:         goto %10;
// DEFAULT-NEXT:         label %9 l3:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:         write<i64>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(0)))), read<i64>(%14));
// DEFAULT-NEXT:         write<ptr<@type2>>(%17, read<ptr<@type2>>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(1))))));
// DEFAULT-NEXT:         if ne<ptr<@type2>>(read<ptr<@type2>>(%17), null<ptr<@type2>>)
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, ptr<i8>) -> void>(%7, read<ptr<void>>(%11), array_decay<ptr<i8>, length=Some(1)>(%37));
// DEFAULT-NEXT:         write<ptr<@type2>>(%17, read<ptr<@type2>>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(0))))));
// DEFAULT-NEXT:         if eq<ptr<@type2>>(read<ptr<@type2>>(%17), null<ptr<@type2>>)
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>, ptr<i8>) -> void>(%7, read<ptr<void>>(%11), array_decay<ptr<i8>, length=Some(1)>(%38));
// DEFAULT-NEXT:         write<ptr<@type2>>(%18, ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%17), const<i32>(2)));
// DEFAULT-NEXT:         write<ptr<@type2>>(field0(deref(read<ptr<@type2>>(%18))), read<ptr<@type2>>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%20), const<i32>(1))))));
// DEFAULT-NEXT:         label %10 l4:
// DEFAULT-NEXT:             ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @g(%22 a: ptr<void>, %23 b: ptr<i8>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %25 uv: array<@type2, 4> [storage=automatic] = aggregate<array<@type2, 4>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(111))), index1 = aggregate<@type2, zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(222))), index2 = aggregate<@type2, zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(333))), index3 = aggregate<@type2, zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(444))));
// DEFAULT-NEXT:         let %26 s: @type3 [storage=automatic] = aggregate<@type3, zero_fill=false>(field0 = null<ptr<ptr<@type2>>>, field1 = aggregate<array<@type2, 4>, zero_fill=false>(index0 = aggregate<@type2, zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(555))), index1 = aggregate<@type2, zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(0))), index2 = aggregate<@type2, zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(999))), index3 = aggregate<@type2, zero_fill=false>(field1 = widen<i64, reason=assign>(const<i32>(777)))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>, ptr<@type3>, ptr<const @type2>, i64, i64, i64) -> void>(%6, null<ptr<void>>, addr_of<ptr<@type3>>(%26), null<ptr<const @type2>>, widen<i64, reason=arg>(const<i32>(20000)), widen<i64, reason=arg>(const<i32>(10000)), ptr_to_int<i64, reason=explicit>(array_decay<ptr<@type2>, length=Some(4)>(%25)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(field1(%26)), const<i32>(0))))), ptr_to_int<i64, reason=explicit>(array_decay<ptr<@type2>, length=Some(4)>(%25))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(field1(%26)), const<i32>(1))))), widen<i64, reason=usual_arith>(const<i32>(0)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(field1(%26)), const<i32>(2))))), widen<i64, reason=usual_arith>(const<i32>(999)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(field1(%26)), const<i32>(3))))), widen<i64, reason=usual_arith>(const<i32>(777)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%25), const<i32>(0))))), widen<i64, reason=usual_arith>(const<i32>(111)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%25), const<i32>(1))))), widen<i64, reason=usual_arith>(const<i32>(222)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%25), const<i32>(2))))), widen<i64, reason=usual_arith>(const<i32>(0)))), ne<i64>(read<i64>(field1(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(array_decay<ptr<@type2>, length=Some(4)>(%25), const<i32>(3))))), widen<i64, reason=usual_arith>(const<i32>(444))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
