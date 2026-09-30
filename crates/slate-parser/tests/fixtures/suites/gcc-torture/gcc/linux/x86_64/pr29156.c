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
// DEFAULT-NEXT:     type @type[[TYPE_test1:[0-9]+]] test1 = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_test2:[0-9]+]] test2 = struct {
// DEFAULT-NEXT:         field0 d: f32;
// DEFAULT-NEXT:         field1 sub: @type[[TYPE_test1]];
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_global:[0-9]+]] global: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bla:[0-9]+]] @bla(%[[VALUE_xa:[0-9]+]] xa: ptr<@type[[TYPE_test1]]>, %[[VALUE_xb:[0-9]+]] xb: ptr<@type[[TYPE_test2]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(%[[VALUE_global]], const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(field1(deref(read<ptr<@type[[TYPE_test2]]>>(%[[VALUE_xb]])))), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_test1]]>>(%[[VALUE_xa]]))), const<i32>(8));
// DEFAULT-NEXT:         return read<i32>(field0(field1(deref(read<ptr<@type[[TYPE_test2]]>>(%[[VALUE_xb]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_pom:[0-9]+]] pom: @type[[TYPE_test2]] [storage=automatic];
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_test1]]>, ptr<@type[[TYPE_test2]]>) -> i32>(%[[VALUE_bla]], addr_of<ptr<@type[[TYPE_test1]]>>(field1(%[[VALUE_pom]])), addr_of<ptr<@type[[TYPE_test2]]>>(%[[VALUE_pom]])), const<i32>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
