/* PR tree-optimization/47538 */

struct S {
  double        a, b, *c;
  unsigned long d;
};

__attribute__((noinline, noclone)) void foo(struct S *x, const struct S *y) {
  const unsigned long n = y->d + 1;
  const double        m = 0.25 * (y->b - y->a);
  x->a                  = y->a;
  x->b                  = y->b;
  if (n == 1) {
    x->c[0] = 0.;
  } else if (n == 2) {
    x->c[1] = m * y->c[0];
    x->c[0] = 2.0 * x->c[1];
  } else {
    double        o = 0.0, p = 1.0;
    unsigned long i;

    for (i = 1; i <= n - 2; i++) {
      x->c[i]  = m * (y->c[i - 1] - y->c[i + 1]) / (double)i;
      o       += p * x->c[i];
      p        = -p;
    }
    x->c[n - 1]  = m * y->c[n - 2] / (n - 1.0);
    o           += p * x->c[n - 1];
    x->c[0]      = 2.0 * o;
  }
}

int main(void) {
  struct S x, y;
  double   c[4] = {10, 20, 30, 40}, d[4], e[4] = {118, 118, 118, 118};

  y.a = 10;
  y.b = 6;
  y.c = c;
  x.c = d;
  y.d = 3;
  __builtin_memcpy(d, e, sizeof d);
  foo(&x, &y);
  if (d[0] != 0 || d[1] != 20 || d[2] != 10 || d[3] != -10)
    __builtin_abort();
  y.d = 2;
  __builtin_memcpy(d, e, sizeof d);
  foo(&x, &y);
  if (d[0] != 60 || d[1] != 20 || d[2] != -10 || d[3] != 118)
    __builtin_abort();
  y.d = 1;
  __builtin_memcpy(d, e, sizeof d);
  foo(&x, &y);
  if (d[0] != -20 || d[1] != -10 || d[2] != 118 || d[3] != 118)
    __builtin_abort();
  y.d = 0;
  __builtin_memcpy(d, e, sizeof d);
  foo(&x, &y);
  if (d[0] != 0 || d[1] != 118 || d[2] != 118 || d[3] != 118)
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:         field1 b: f64;
// DEFAULT-NEXT:         field2 c: ptr<f64>;
// DEFAULT-NEXT:         field3 d: u64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     fn %1 @foo(%2 x: ptr<@type0>, %3 y: ptr<const @type0>) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %4 n: u64 [storage=automatic] [const] = add<u64, overflow=wrap>(read<u64>(field3(deref(read<ptr<const @type0>>(%3)))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:         let %5 m: f64 [storage=automatic] [const] = mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(0.25), sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(field1(deref(read<ptr<const @type0>>(%3)))), read<f64>(field0(deref(read<ptr<const @type0>>(%3))))));
// DEFAULT-NEXT:         write<f64>(field0(deref(read<ptr<@type0>>(%2))), read<f64>(field0(deref(read<ptr<const @type0>>(%3)))));
// DEFAULT-NEXT:         write<f64>(field1(deref(read<ptr<@type0>>(%2))), read<f64>(field1(deref(read<ptr<const @type0>>(%3)))));
// DEFAULT-NEXT:         if eq<u64>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<@type0>>(%2)))), const<i32>(0))), const<f64>(0.0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if eq<u64>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2))))
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<@type0>>(%2)))), const<i32>(1))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%5), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<const @type0>>(%3)))), const<i32>(0))))));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<@type0>>(%2)))), const<i32>(0))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<@type0>>(%2)))), const<i32>(1))))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             else
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %6 o: f64 [storage=automatic] = const<f64>(0.0);
// DEFAULT-NEXT:                     let %7 p: f64 [storage=automatic] = const<f64>(1.0);
// DEFAULT-NEXT:                     let %8 i: u64 [storage=automatic];
// DEFAULT-NEXT:                     for %15
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             write<u64>(%8, reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:                         condition: le<u64>(read<u64>(%8), sub<u64, overflow=wrap>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %16: u64 [synthetic] = read<u64>(%8);
// DEFAULT-NEXT:                             let %17: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%16), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                             write<u64>(%8, read<u64>(%17));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<@type0>>(%2)))), read<u64>(%8))), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%5), sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<const @type0>>(%3)))), sub<u64, overflow=wrap>(read<u64>(%8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<const @type0>>(%3)))), add<u64, overflow=wrap>(read<u64>(%8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))))))))), int_to_float<f64, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(%8))));
// DEFAULT-NEXT:                                 let %18: f64 [synthetic] = read<f64>(%6);
// DEFAULT-NEXT:                                 let %19: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%18), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%7), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<@type0>>(%2)))), read<u64>(%8))))));
// DEFAULT-NEXT:                                 write<f64>(%6, read<f64>(%19));
// DEFAULT-NEXT:                                 write<f64>(%7, neg<f64>(read<f64>(%7)));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<@type0>>(%2)))), sub<u64, overflow=wrap>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), div<f64, rounding=nearest_even, exceptions=ignore, contract=on>(mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%5), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<const @type0>>(%3)))), sub<u64, overflow=wrap>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))))))), sub<f64, rounding=nearest_even, exceptions=ignore, contract=on>(int_to_float<f64, reason=usual_arith, exact=false, rounding=nearest_even, exceptions=ignore>(read<u64>(%4)), const<f64>(1.0))));
// DEFAULT-NEXT:                     let %20: f64 [synthetic] = read<f64>(%6);
// DEFAULT-NEXT:                     let %21: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%20), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(read<f64>(%7), read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<@type0>>(%2)))), sub<u64, overflow=wrap>(read<u64>(%4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))))));
// DEFAULT-NEXT:                     write<f64>(%6, read<f64>(%21));
// DEFAULT-NEXT:                     write<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(read<ptr<f64>>(field2(deref(read<ptr<@type0>>(%2)))), const<i32>(0))), mul<f64, rounding=nearest_even, exceptions=ignore, contract=on>(const<f64>(2.0), read<f64>(%6)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 x: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %11 y: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %12 c: array<f64, 4> [storage=automatic] = aggregate<array<f64, 4>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(10)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(20)), index2 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(30)), index3 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(40)));
// DEFAULT-NEXT:         let %13 d: array<f64, 4> [storage=automatic];
// DEFAULT-NEXT:         let %14 e: array<f64, 4> [storage=automatic] = aggregate<array<f64, 4>, zero_fill=false>(index0 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(118)), index1 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(118)), index2 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(118)), index3 = int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(118)));
// DEFAULT-NEXT:         write<f64>(field0(%11), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(10)));
// DEFAULT-NEXT:         write<f64>(field1(%11), int_to_float<f64, reason=assign, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(6)));
// DEFAULT-NEXT:         write<ptr<f64>>(field2(%11), array_decay<ptr<f64>, length=Some(4)>(%12));
// DEFAULT-NEXT:         write<ptr<f64>>(field2(%10), array_decay<ptr<f64>, length=Some(4)>(%13));
// DEFAULT-NEXT:         write<u64>(field3(%11), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(3))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<f64>, length=Some(4)>(%13)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<f64>, length=Some(4)>(%14)), const<u64>(32));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<const @type0>) -> void>(%1, addr_of<ptr<@type0>>(%10), pointer_cast<ptr<const @type0>, reason=arg>(addr_of<ptr<@type0>>(%11)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(0)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(1)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(20)))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(2)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(10)))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(3)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(10)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<u64>(field3(%11), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<f64>, length=Some(4)>(%13)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<f64>, length=Some(4)>(%14)), const<u64>(32));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<const @type0>) -> void>(%1, addr_of<ptr<@type0>>(%10), pointer_cast<ptr<const @type0>, reason=arg>(addr_of<ptr<@type0>>(%11)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(0)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(60))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(1)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(20)))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(2)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(10))))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(3)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(118))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<u64>(field3(%11), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(1))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<f64>, length=Some(4)>(%13)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<f64>, length=Some(4)>(%14)), const<u64>(32));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<const @type0>) -> void>(%1, addr_of<ptr<@type0>>(%10), pointer_cast<ptr<const @type0>, reason=arg>(addr_of<ptr<@type0>>(%11)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(0)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(20)))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(1)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(neg<i32, overflow=ub>(const<i32>(10))))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(2)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(118)))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(3)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(118))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         write<u64>(field3(%11), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<f64>, length=Some(4)>(%13)), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<f64>, length=Some(4)>(%14)), const<u64>(32));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, ptr<const @type0>) -> void>(%1, addr_of<ptr<@type0>>(%10), pointer_cast<ptr<const @type0>, reason=arg>(addr_of<ptr<@type0>>(%11)));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(0)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(0))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(1)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(118)))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(2)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(118)))), ne<f64, exceptions=ignore>(read<f64>(deref(ptr_offset<ptr<f64>, subtract=false, element=f64, overflow=ub>(array_decay<ptr<f64>, length=Some(4)>(%13), const<i32>(3)))), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(118))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
