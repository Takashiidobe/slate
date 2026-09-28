
struct Foo;
struct Bar;

__declspec(allocator) void *alloc_void(void);
__declspec(allocator) struct Foo *alloc_foo(void);

void call_alloc(void) {
  struct Foo *p = alloc_void();
  struct Foo *w = alloc_foo();
  struct Foo *q = (struct Foo*)alloc_void();
  struct Foo *r = (struct Foo*)(struct Bar*)alloc_void();
}

// SLATE-FILECHECK-FLAVOR msvc
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 Foo = struct incomplete;
// DEFAULT-NEXT:     type @type1 Bar = struct incomplete;
// DEFAULT-NEXT:     fn %2 @alloc_void() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %3 @alloc_foo() -> ptr<@type0> [linkage=external];
// DEFAULT-NEXT:     fn %4 @call_alloc() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 p: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn() -> ptr<void>>(%2));
// DEFAULT-NEXT:         let %6 w: ptr<@type0> [storage=automatic] = call<ptr<@type0>, signature=fn() -> ptr<@type0>>(%3);
// DEFAULT-NEXT:         let %7 q: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=explicit>(call<ptr<void>, signature=fn() -> ptr<void>>(%2));
// DEFAULT-NEXT:         let %8 r: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=explicit>(pointer_cast<ptr<@type1>, reason=explicit>(call<ptr<void>, signature=fn() -> ptr<void>>(%2)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
