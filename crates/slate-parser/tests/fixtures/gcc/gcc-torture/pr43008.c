int i;
struct X {
  int *p;
};
struct X *__attribute__((malloc)) my_alloc(void) {
  struct X *p = __builtin_malloc(sizeof(struct X));
  p->p        = &i;
  return p;
}
extern void abort(void);
int         main() {
  struct X *p, *q;
  p       = my_alloc();
  q       = my_alloc();
  *(p->p) = 1;
  *(q->p) = 0;
  if (*(p->p) != 0)
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
// DEFAULT-NEXT:     type @type0 X = struct {
// DEFAULT-NEXT:         field0 p: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     global %0 i: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %2 @my_alloc() -> ptr<@type0> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 p: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_malloc, const<u64>(8)));
// DEFAULT-NEXT:         write<ptr<i32>>(field0(deref(read<ptr<@type0>>(%3))), addr_of<ptr<i32>>(%0));
// DEFAULT-NEXT:         return read<ptr<@type0>>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 p: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %7 q: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type0>>(%6, call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%2));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%2);
// DEFAULT-NEXT:         write<ptr<@type0>>(%7, call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%2));
// DEFAULT-NEXT:         call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%2);
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field0(deref(read<ptr<@type0>>(%6))))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(field0(deref(read<ptr<@type0>>(%7))))), const<i32>(0));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(read<ptr<i32>>(field0(deref(read<ptr<@type0>>(%6)))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
