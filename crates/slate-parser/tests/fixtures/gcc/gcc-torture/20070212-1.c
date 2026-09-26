struct f {
  int i;
};

int g(int i, int c, struct f *ff, int *p) {
  int *t;
  if (c)
    t = &i;
  else
    t = &ff->i;
  *p = 0;
  return *t;
}

extern void abort(void);

int main() {
  struct f f;
  f.i = 1;
  if (g(5, 0, &f, &f.i) != 0)
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
// DEFAULT-NEXT:     type @type0 f = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %1 @g(%2 i: i32, %3 c: i32, %4 ff: ptr<@type0>, %5 p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 t: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%3), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<i32>>(%6, addr_of<ptr<i32>>(%2));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<i32>>(%6, addr_of<ptr<i32>>(field0(deref(read<ptr<@type0>>(%4)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%5)), const<i32>(0));
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%6)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 f: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%9), const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i32, i32, ptr<@type0>, ptr<i32>) -> i32>(%1, const<i32>(5), const<i32>(0), addr_of<ptr<@type0>>(%9), addr_of<ptr<i32>>(field0(%9))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
