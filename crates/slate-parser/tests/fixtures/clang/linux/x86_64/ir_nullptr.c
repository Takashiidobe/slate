// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-metadata -std=c23
// SLATE-FILECHECK-DEFINES DEFAULT

typedef typeof(nullptr) nullptr_t;
int printf(const char *, ...);

nullptr_t g;
nullptr_t g2 = nullptr;
nullptr_t g3 = 0;
int *gp = nullptr;

int distinct_from_void_pointer = _Generic(nullptr, void *: 1, nullptr_t: 2);
int object_type = _Generic(g, nullptr_t: 2, default: 0);
int same_layout = sizeof(nullptr_t) == sizeof(void *) && _Alignof(nullptr_t) == _Alignof(void *);
int conditional_with_pointer = _Generic(1 ? nullptr : (int *)0, int *: 1, default: 0);
int conditional_with_nullptr = _Generic(1 ? nullptr : nullptr, nullptr_t: 1, default: 0);
int classify = __builtin_classify_type(nullptr);

int f(nullptr_t n, int *p, char *q) {
  bool b = n;
  void *v = n;
  p = n;
  n = nullptr;
  n = 0;
  if (n)
    return 1;
  if (!n && p == n && n == p && n == nullptr && n == 0 && 0 == n)
    return 2;
  printf("%p", n);
  (void)v;
  q = (char *)n;
  b = (bool)n;
  n = (nullptr_t)nullptr;
  return b + (p != nullptr) + (n != q);
}

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
// DEFAULT-NEXT:     type @type0 nullptr_t = ptr<void> [c="typeof(nullptr)"] [c_canon="nullptr_t"];
// DEFAULT-NEXT:     global %2 g: ptr<void> [storage=static] [linkage=external] [c="nullptr_t"] [typedef_chain="nullptr_t"];
// DEFAULT-NEXT:     global %3 g2: ptr<void> [storage=static] = null<ptr<void>> [linkage=external] [c="nullptr_t"] [typedef_chain="nullptr_t"];
// DEFAULT-NEXT:     global %4 g3: ptr<void> [storage=static] = null<ptr<void>> [linkage=external] [c="nullptr_t"] [typedef_chain="nullptr_t"];
// DEFAULT-NEXT:     global %5 gp: ptr<i32> [storage=static] = null<ptr<i32>> [linkage=external] [c="int *"];
// DEFAULT-NEXT:     global %6 distinct_from_void_pointer: i32 [storage=static] = const<i32>(2) [linkage=external] [c="int"];
// DEFAULT-NEXT:     global %7 object_type: i32 [storage=static] = const<i32>(2) [linkage=external] [c="int"];
// DEFAULT-NEXT:     global %8 same_layout: i32 [storage=static] = from_bool<i32>(logical_and<bool>(eq<u64>(const<u64>(8) [size_of="ptr<void>"], const<u64>(8) [size_of="ptr<void>"]), eq<u64>(const<u64>(8) [align_of="ptr<void>"], const<u64>(8) [align_of="ptr<void>"]))) [linkage=external] [c="int"];
// DEFAULT-NEXT:     global %9 conditional_with_pointer: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// DEFAULT-NEXT:     global %10 conditional_with_nullptr: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// DEFAULT-NEXT:     global %11 classify: i32 [storage=static] = const<i32>(-1) [c_builtin="__builtin_classify_type"] [linkage=external] [c="int"];
// DEFAULT-NEXT:     global %19 .str19: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 112, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%18 <unnamed>: ptr<const i8> [c="const char *"], ...) -> i32 [linkage=external] [c="int(const char *, ...)"] [c_builtin="printf"];
// DEFAULT-NEXT:     fn %12 @f(%13 n: ptr<void> [c="nullptr_t"] [typedef_chain="nullptr_t"], %14 p: ptr<i32> [c="int *"], %15 q: ptr<i8> [c="char *"]) -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(nullptr_t, int *, char *)"] {
// DEFAULT-NEXT:         let %16 b: bool [storage=automatic] = ne<ptr<void>>(read<ptr<void>>(%13), null<ptr<void>>) [c="_Bool"];
// DEFAULT-NEXT:         let %17 v: ptr<void> [storage=automatic] = pointer_cast<ptr<void>>(read<ptr<void>>(%13)) [c="void *"];
// DEFAULT-NEXT:         write<ptr<i32>>(%14, pointer_cast<ptr<i32>>(read<ptr<void>>(%13)));
// DEFAULT-NEXT:         write<ptr<void>>(%13, null<ptr<void>>);
// DEFAULT-NEXT:         write<ptr<void>>(%13, null<ptr<void>>);
// DEFAULT-NEXT:         if ne<ptr<void>>(read<ptr<void>>(%13), null<ptr<void>>)
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(logical_and<bool>(not<bool>(ne<ptr<void>>(read<ptr<void>>(%13), null<ptr<void>>)), eq<ptr<i32>>(read<ptr<i32>>(%14), pointer_cast<ptr<i32>>(read<ptr<void>>(%13)))), eq<ptr<i32>>(pointer_cast<ptr<i32>>(read<ptr<void>>(%13)), read<ptr<i32>>(%14))), eq<ptr<void>>(read<ptr<void>>(%13), null<ptr<void>>)), eq<ptr<void>>(read<ptr<void>>(%13), null<ptr<void>>)), eq<ptr<void>>(null<ptr<void>>, read<ptr<void>>(%13)))
// DEFAULT-NEXT:             return const<i32>(2);
// DEFAULT-NEXT:         call<i32>(%1, pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(3)>(%19)), read<ptr<void>>(%13));
// DEFAULT-NEXT:         read<ptr<void>>(%17);
// DEFAULT-NEXT:         write<ptr<i8>>(%15, pointer_cast<ptr<i8>>(read<ptr<void>>(%13)));
// DEFAULT-NEXT:         write<bool>(%16, ne<ptr<void>>(read<ptr<void>>(%13), null<ptr<void>>));
// DEFAULT-NEXT:         write<ptr<void>>(%13, null<ptr<void>>);
// DEFAULT-NEXT:         return add<i32>(add<i32>(from_bool<i32>(read<bool>(%16)), from_bool<i32>(ne<ptr<i32>>(read<ptr<i32>>(%14), null<ptr<i32>>))), from_bool<i32>(ne<ptr<i8>>(pointer_cast<ptr<i8>>(read<ptr<void>>(%13)), read<ptr<i8>>(%15))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
