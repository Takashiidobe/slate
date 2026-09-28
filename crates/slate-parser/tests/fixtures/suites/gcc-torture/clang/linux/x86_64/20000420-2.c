// SLATE-FILECHECK-DEFINES DEFAULT

struct x { int a, b, c; };

extern struct x a ();
extern void b (struct x);

void
foo ()
{
  a ();
  b (a ());
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
// DEFAULT-NEXT:     type @type0 x = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:         field2 c: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %1 @a() -> @type0 [linkage=external] [abi=sysv64() -> native_c];
// DEFAULT-NEXT:     fn %2 @b(%4 <unnamed>: @type0) -> void [linkage=external] [abi=sysv64(native_c) -> void];
// DEFAULT-NEXT:     fn %3 @foo() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%1);
// DEFAULT-NEXT:         call<void, signature=fn(@type0) -> void, abi=sysv64(native_c) -> void>(%2, copy<@type0, reason=arg>(call<@type0, signature=fn() -> @type0, abi=sysv64() -> native_c>(%1)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
