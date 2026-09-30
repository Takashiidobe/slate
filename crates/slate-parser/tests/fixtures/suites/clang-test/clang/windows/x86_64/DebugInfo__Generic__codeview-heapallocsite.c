
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

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

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
// DEFAULT-NEXT:     type @type[[TYPE_Foo:[0-9]+]] Foo = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_Bar:[0-9]+]] Bar = struct incomplete;
// DEFAULT-NEXT:     fn %[[VALUE_alloc_void:[0-9]+]] @alloc_void() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_alloc_foo:[0-9]+]] @alloc_foo() -> ptr<@type[[TYPE_Foo]]> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_call_alloc:[0-9]+]] @call_alloc() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_Foo]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_Foo]]>, reason=assign>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_alloc_void]]));
// DEFAULT-NEXT:         let %[[VALUE_w:[0-9]+]] w: ptr<@type[[TYPE_Foo]]> [storage=automatic] = call<ptr<@type[[TYPE_Foo]]>, signature=fn() -> ptr<@type[[TYPE_Foo]]>>(%[[VALUE_alloc_foo]]);
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_Foo]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_Foo]]>, reason=explicit>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_alloc_void]]));
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: ptr<@type[[TYPE_Foo]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_Foo]]>, reason=explicit>(pointer_cast<ptr<@type[[TYPE_Bar]]>, reason=explicit>(call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_alloc_void]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
