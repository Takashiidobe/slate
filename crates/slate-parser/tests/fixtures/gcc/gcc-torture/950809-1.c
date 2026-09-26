void abort(void);
void exit(int);

struct S {
  int *sp, fc, *sc, a[2];
};

void f(struct S *x) {
  int *t  = x->sc;
  int  t1 = t[0];
  int  t2 = t[1];
  int  t3 = t[2];
  int  a0 = x->a[0];
  int  a1 = x->a[1];
  t[2]    = t1;
  t[0]    = a1;
  x->a[1] = a0;
  x->a[0] = t3;
  x->fc   = t2;
  x->sp   = t;
}

int main(void) {
  struct S   s;
  static int sc[3] = {2, 3, 4};
  s.sc             = sc;
  s.a[0]           = 10;
  s.a[1]           = 11;
  f(&s);
  if (s.sp[2] != 2)
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
// DEFAULT-NEXT:     type @type0 S = struct {
// DEFAULT-NEXT:         field0 sp: ptr<i32>;
// DEFAULT-NEXT:         field1 fc: i32;
// DEFAULT-NEXT:         field2 sc: ptr<i32>;
// DEFAULT-NEXT:         field3 a: array<i32, 2>;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     global %13 sc: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=false>(index0 = const<i32>(2), index1 = const<i32>(3), index2 = const<i32>(4)) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%14 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @f(%4 x: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 t: ptr<i32> [storage=automatic] = read<ptr<i32>>(field2(deref(read<ptr<@type0>>(%4))));
// DEFAULT-NEXT:         let %6 t1: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), const<i32>(0))));
// DEFAULT-NEXT:         let %7 t2: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), const<i32>(1))));
// DEFAULT-NEXT:         let %8 t3: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), const<i32>(2))));
// DEFAULT-NEXT:         let %9 a0: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field3(deref(read<ptr<@type0>>(%4)))), const<i32>(0))));
// DEFAULT-NEXT:         let %10 a1: i32 [storage=automatic] = read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field3(deref(read<ptr<@type0>>(%4)))), const<i32>(1))));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), const<i32>(2))), read<i32>(%6));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%5), const<i32>(0))), read<i32>(%10));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field3(deref(read<ptr<@type0>>(%4)))), const<i32>(1))), read<i32>(%9));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field3(deref(read<ptr<@type0>>(%4)))), const<i32>(0))), read<i32>(%8));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type0>>(%4))), read<i32>(%7));
// DEFAULT-NEXT:         write<ptr<i32>>(field0(deref(read<ptr<@type0>>(%4))), read<ptr<i32>>(%5));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %12 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i32>>(field2(%12), array_decay<ptr<i32>, length=Some(3)>(%13));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field3(%12)), const<i32>(0))), const<i32>(10));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field3(%12)), const<i32>(1))), const<i32>(11));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%3, addr_of<ptr<@type0>>(%12));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(field0(%12)), const<i32>(2)))), const<i32>(2))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
