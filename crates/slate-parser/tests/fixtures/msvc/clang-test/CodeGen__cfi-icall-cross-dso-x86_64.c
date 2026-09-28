








void caller(void (*f)()) {
  f();
}

// Check that we emit both string and hash based type entries for static void g(),
// and don't emit them for the declaration of h().

static void g(void) {}

void h(void);

typedef void (*Fn)(void);
Fn g1() {
  return &g;
}
Fn h1() {
  return &h;
}

inline void foo() {}
void bar() { foo(); }



// Check that the type entries are correct.

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=x86_64-pc-windows-msvc
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
// DEFAULT-NEXT:     type @type0 Fn = ptr<fn() -> void>;
// DEFAULT-NEXT:     fn %0 @caller(%1 f: ptr<fn(unprototyped) -> void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(read<ptr<fn(unprototyped) -> void>>(%1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @g() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @h() -> void [linkage=external];
// DEFAULT-NEXT:     fn %5 @g1(unprototyped) -> ptr<fn() -> void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<fn() -> void>>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @h1(unprototyped) -> ptr<fn() -> void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<fn() -> void>>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @foo(unprototyped) -> void [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @bar(unprototyped) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<void, signature=fn(unprototyped) -> void>(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
