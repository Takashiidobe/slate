extern void abort(void);

struct S {
  long o;
};

struct T {
  long     o;
  struct S m[82];
};

struct T t;

int main() {
  struct S *p, *q;

  p = (struct S *)&t;
  p = &((struct T *)p)->m[0];
  q = p + 82;
  while (--q > p)
    q->o = -1;
  q->o = 0;

  if (q > p)
    abort();
  if (q - p > 0)
    abort();
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
// DEFAULT-NEXT:         field0 o: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type1 T = struct {
// DEFAULT-NEXT:         field0 o: i64;
// DEFAULT-NEXT:         field1 m: array<@type0, 82>;
// DEFAULT-NEXT:     } [size=664, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %3 t: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 p: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %6 q: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type0>>(%5, pointer_cast<ptr<@type0>, reason=explicit>(addr_of<ptr<@type1>>(%3)));
// DEFAULT-NEXT:         write<ptr<@type0>>(%5, addr_of<ptr<@type0>>(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(82)>(field1(deref(pointer_cast<ptr<@type1>, reason=explicit>(read<ptr<@type0>>(%5))))), const<i32>(0)))));
// DEFAULT-NEXT:         write<ptr<@type0>>(%6, ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%5), const<i32>(82)));
// DEFAULT-NEXT:         while %7 {
// DEFAULT-NEXT:             let %8: ptr<@type0> [synthetic] = read<ptr<@type0>>(%6);
// DEFAULT-NEXT:             let %9: ptr<@type0> [synthetic] = ptr_offset<ptr<@type0>, subtract=true, element=@type0, overflow=ub>(read<ptr<@type0>>(%8), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<@type0>>(%6, read<ptr<@type0>>(%9));
// DEFAULT-NEXT:             yield gt<ptr<@type0>>(read<ptr<@type0>>(%9), read<ptr<@type0>>(%5));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<i64>(field0(deref(read<ptr<@type0>>(%6))), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(field0(deref(read<ptr<@type0>>(%6))), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         if gt<ptr<@type0>>(read<ptr<@type0>>(%6), read<ptr<@type0>>(%5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         if gt<i64>(ptr_diff<i64, element=@type0, same_array=required, overflow=ub>(read<ptr<@type0>>(%6), read<ptr<@type0>>(%5)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
