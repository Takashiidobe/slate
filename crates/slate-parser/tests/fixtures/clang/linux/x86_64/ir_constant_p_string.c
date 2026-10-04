// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

int g;
int arr[3];

_Static_assert(__builtin_constant_p("abc"), "");
_Static_assert(__builtin_constant_p(L"w"), "");
_Static_assert(__builtin_constant_p(("x")), "");
_Static_assert(__builtin_constant_p(1 ? "a" : "b"), "");
_Static_assert(__builtin_constant_p("abc" + 0), "");
_Static_assert(__builtin_constant_p(0 + "abc"), "");
_Static_assert(__builtin_constant_p("abc" - 0), "");
_Static_assert(__builtin_constant_p(&"abc"[0]), "");
_Static_assert(__builtin_constant_p(&*"abc"), "");
_Static_assert(__builtin_constant_p((const void *)"abc"), "");
_Static_assert(__builtin_constant_p((long)"abc"), "");
_Static_assert(__builtin_constant_p((char *)0), "");
_Static_assert(__builtin_constant_p((char *)5), "");
_Static_assert(__builtin_constant_p((0, "abc")), "");
_Static_assert(!__builtin_constant_p("abc" + 1), "");
_Static_assert(!__builtin_constant_p(&g), "");
_Static_assert(!__builtin_constant_p(arr), "");
_Static_assert(!__builtin_constant_p(g ? "a" : "b"), "");

void *create(const char *name);

void *named(void) {
  return ((void) sizeof(struct { _Static_assert(__builtin_constant_p("ctx"), "names must be constant"); char a; }),
          create("ctx"));
}

int local(const char *name) {
  return __builtin_constant_p("abc") + __builtin_constant_p(name);
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// IR-NEXT:         field0 a: i8;
// IR-NEXT:     } [size=1, align=1, offsets=[0]];
// IR-NEXT:     global %[[VALUE_g:[0-9]+]] g: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_arr:[0-9]+]] arr: array<i32, 3> [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([99, 116, 120, 0]) [linkage=internal];
// IR-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([97, 98, 99, 0]) [linkage=internal];
// IR-NEXT:     fn %[[VALUE_create:[0-9]+]] @create(%[[VALUE_name:[0-9]+]] name: ptr<const i8>) -> ptr<void> [linkage=external];
// IR-NEXT:     fn %[[VALUE_named:[0-9]+]] @named() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         const<u64>(1);
// IR-NEXT:         return call<ptr<void>>(%[[VALUE_create]], pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_local:[0-9]+]] @local(%[[VALUE_name_2:[0-9]+]] name: ptr<const i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<i32>(const<i32>(1), const<i32>(0));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
