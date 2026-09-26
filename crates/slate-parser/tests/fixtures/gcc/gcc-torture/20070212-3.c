struct foo {
  int i;
  int j;
};

int bar(struct foo *k, int k2, int f, int f2) {
  int *p, *q;
  int  res;
  if (f)
    p = &k->i;
  else
    p = &k->j;
  res  = *p;
  k->i = 1;
  if (f2)
    q = p;
  else
    q = &k2;
  return res + *q;
}

extern void abort(void);

int main() {
  struct foo k;
  k.i = 0;
  k.j = 1;
  if (bar(&k, 1, 1, 1) != 1)
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
// DEFAULT-NEXT:     type @type0 foo = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %1 @bar(%2 k: ptr<@type0>, %3 k2: i32, %4 f: i32, %5 f2: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 p: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %7 q: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %8 res: i32 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<i32>>(%6, addr_of<ptr<i32>>(field0(deref(read<ptr<@type0>>(%2)))));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<i32>>(%6, addr_of<ptr<i32>>(field1(deref(read<ptr<@type0>>(%2)))));
// DEFAULT-NEXT:         write<i32>(%8, read<i32>(deref(read<ptr<i32>>(%6))));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%2))), const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%5), const<i32>(0))
// DEFAULT-NEXT:             write<ptr<i32>>(%7, read<ptr<i32>>(%6));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<ptr<i32>>(%7, addr_of<ptr<i32>>(%3));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(read<i32>(%8), read<i32>(deref(read<ptr<i32>>(%7))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 k: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(field0(%11), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field1(%11), const<i32>(1));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type0>, i32, i32, i32) -> i32>(%1, addr_of<ptr<@type0>>(%11), const<i32>(1), const<i32>(1), const<i32>(1)), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
