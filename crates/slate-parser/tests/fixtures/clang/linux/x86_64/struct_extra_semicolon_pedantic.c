struct outer {
  int a;;;
  struct inner { int b; /* gap */ ; } in;
};

union u {
  ;
  int x;
};

int f(void) {
  struct local { int c; ; } l = {0};
  return l.c;
}

// SLATE-FILECHECK-ARGS -pedantic
// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-WARNING C17

// SLATE-FILECHECK-BEGIN C17
// C17: -Wextra-semi
// C17:   ⚠ extra ';' inside a struct
// C17:    ╭─[tests/fixtures/clang/linux/x86_64/struct_extra_semicolon_pedantic.c:2:9]
// C17:  1 │ struct outer {
// C17:  2 │   int a;;;
// C17:    ·         ─
// C17:  3 │   struct inner { int b; /* gap */ ; } in;
// C17:    ╰────
// C17: -Wextra-semi
// C17:   ⚠ extra ';' inside a struct
// C17:    ╭─[tests/fixtures/clang/linux/x86_64/struct_extra_semicolon_pedantic.c:3:35]
// C17:  2 │   int a;;;
// C17:  3 │   struct inner { int b; /* gap */ ; } in;
// C17:    ·                                   ─
// C17:  4 │ };
// C17:    ╰────
// C17: -Wextra-semi
// C17:   ⚠ extra ';' inside a union
// C17:    ╭─[tests/fixtures/clang/linux/x86_64/struct_extra_semicolon_pedantic.c:7:3]
// C17:  6 │ union u {
// C17:  7 │   ;
// C17:    ·   ─
// C17:  8 │   int x;
// C17:    ╰────
// C17: -Wextra-semi
// C17:   ⚠ extra ';' inside a struct
// C17:     ╭─[tests/fixtures/clang/linux/x86_64/struct_extra_semicolon_pedantic.c:12:25]
// C17:  11 │ int f(void) {
// C17:  12 │   struct local { int c; ; } l = {0};
// C17:     ·                         ─
// C17:  13 │   return l.c;
// C17:     ╰────
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN IR-C17
// IR-C17: module {
// IR-C17-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-C17-NEXT:         endian = little;
// IR-C17-NEXT:         pointer [size=8, align=8];
// IR-C17-NEXT:         stack_alignment = 16;
// IR-C17-NEXT:         long_double = f80;
// IR-C17-NEXT:         storage bool [size=1, align=1];
// IR-C17-NEXT:         storage i8, u8 [size=1, align=1];
// IR-C17-NEXT:         storage i16, u16 [size=2, align=2];
// IR-C17-NEXT:         storage i32, u32 [size=4, align=4];
// IR-C17-NEXT:         storage i64, u64 [size=8, align=8];
// IR-C17-NEXT:         storage i128, u128 [size=16, align=16];
// IR-C17-NEXT:         storage bf16 [size=2, align=2];
// IR-C17-NEXT:         storage f16 [size=2, align=2];
// IR-C17-NEXT:         storage f32 [size=4, align=4];
// IR-C17-NEXT:         storage f64 [size=8, align=8];
// IR-C17-NEXT:         storage f80 [size=16, align=16];
// IR-C17-NEXT:         storage f128 [size=16, align=16];
// IR-C17-NEXT:         storage d32 [size=4, align=4];
// IR-C17-NEXT:         storage d64 [size=8, align=8];
// IR-C17-NEXT:         storage d128 [size=16, align=16];
// IR-C17-NEXT:     }
// IR-C17-NEXT:     type @type[[TYPE_outer:[0-9]+]] outer = struct {
// IR-C17-NEXT:         field0 a: i32;
// IR-C17-NEXT:         field1 in: @type[[TYPE_inner:[0-9]+]];
// IR-C17-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-C17-NEXT:     type @type[[TYPE_inner]] inner = struct {
// IR-C17-NEXT:         field0 b: i32;
// IR-C17-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-C17-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// IR-C17-NEXT:         field0 x: i32;
// IR-C17-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-C17-NEXT:     type @type[[TYPE_local:[0-9]+]] local = struct {
// IR-C17-NEXT:         field0 c: i32;
// IR-C17-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-C17-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-C17-NEXT:         let %[[VALUE_l:[0-9]+]] l: @type[[TYPE_local]] [storage=automatic] = aggregate<@type[[TYPE_local]], zero_fill=false>(field0 = const<i32>(0));
// IR-C17-NEXT:         return read<i32>(field0(%[[VALUE_l]]));
// IR-C17-NEXT:     }
// IR-C17-NEXT: }
// SLATE-FILECHECK-END IR-C17
