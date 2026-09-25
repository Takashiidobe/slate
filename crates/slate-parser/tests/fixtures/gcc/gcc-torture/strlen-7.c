/* Test to verify that a strlen() call with a pointer to a dynamic type
   doesn't make assumptions based on the static type of the original
   pointer.  See g++.dg/init/strlen.C for the corresponding C++ test.  */

struct A {
  int  i;
  char a[1];
  void (*p)();
};
struct B {
  char a[sizeof(struct A) - __builtin_offsetof(struct A, a)];
};

__attribute__((noipa)) void init(char *d, const char *s) {
  __builtin_strcpy(d, s);
}

struct B b;

__attribute__((noipa)) void test_dynamic_type(struct A *p) {
  /* The following call is undefined because it writes past the end
     of the p->a subobject, but the corresponding GIMPLE considers
     it valid and there's apparently no way to distinguish invalid
     cases from ones like it that might be valid.  If/when GIMPLE
     changes to make this possible this test can be removed.  */
  char *q = (char *)__builtin_memcpy(p->a, &b, sizeof b);

  init(q, "foobar");

  if (6 != __builtin_strlen(q))
    __builtin_abort();
}

int main(void) {
  struct A *p = (struct A *)__builtin_malloc(sizeof *p);
  test_dynamic_type(p);
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
// DEFAULT-NEXT:     type @type0 A = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 a: array<i8, 1>;
// DEFAULT-NEXT:         field2 p: ptr<fn() -> void>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 B = struct {
// DEFAULT-NEXT:         field0 a: array<i8, 12>;
// DEFAULT-NEXT:     } [size=12, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %5 b: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 .str11: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([102, 111, 111, 98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @init(%3 d: ptr<i8>, %4 s: ptr<const i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(__builtin_strcpy, read<ptr<i8>>(%3), read<ptr<const i8>>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @test_dynamic_type(%7 p: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 q: ptr<i8> [storage=automatic] = pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(__builtin_memcpy, pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type0>>(%7))))), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<@type1>>(%5)), const<u64>(12)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ptr<const i8>) -> void>(%2, read<ptr<i8>>(%8), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%11)));
// DEFAULT-NEXT:         if ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(6))), call<u64, signature=fn(ptr<const i8>) -> u64>(__builtin_strlen, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%8))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 p: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(__builtin_malloc, const<u64>(16)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%6, read<ptr<@type0>>(%10));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
