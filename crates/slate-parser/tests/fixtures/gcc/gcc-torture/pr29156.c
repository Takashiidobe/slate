extern void abort(void);
struct test1 {
  int a;
  int b;
};
struct test2 {
  float        d;
  struct test1 sub;
};

int global;

int bla(struct test1 *xa, struct test2 *xb) {
  global    = 1;
  xb->sub.a = 1;
  xa->a     = 8;
  return xb->sub.a;
}

int main(void) {
  struct test2 pom;

  if (bla(&pom.sub, &pom) != 8)
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
// DEFAULT-NEXT:     type @type0 test1 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 test2 = struct {
// DEFAULT-NEXT:         field0 d: f32;
// DEFAULT-NEXT:         field1 sub: @type0;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %3 global: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %4 @bla(%5 xa: ptr<@type0>, %6 xb: ptr<@type1>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%3, const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(field1(deref(read<ptr<@type1>>(%6)))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%5))), const<i32>(8));
// DEFAULT-NEXT:         return read<i32>(field0(field1(deref(read<ptr<@type1>>(%6)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 pom: @type1 [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type0>, ptr<@type1>) -> i32>(%4, addr_of<ptr<@type0>>(field1(%8)), addr_of<ptr<@type1>>(%8)), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
