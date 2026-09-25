void abort(void);
void exit(int);

static void *self(void *p) { return p; }

int f() {
  struct {
    int i;
  } s, *sp;
  int *ip = &s.i;

  s.i = 1;
  sp  = self(&s);

  *ip = 0;
  return sp->i + 1;
}

int main(void) {
  if (f() != 1)
    abort();
  else
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%10 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @self(%3 p: ptr<void>) -> ptr<void> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<void>>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 s: @type0 [storage=automatic];
// DEFAULT-NEXT:         let %7 sp: ptr<@type0> [storage=automatic];
// DEFAULT-NEXT:         let %8 ip: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(field0(%6));
// DEFAULT-NEXT:         write<i32>(field0(%6), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<@type0>>(%7, pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%6)))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn(ptr<void>) -> ptr<void>>(%2, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type0>>(%6))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%8)), const<i32>(0));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(field0(deref(read<ptr<@type0>>(%7)))), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%4), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
