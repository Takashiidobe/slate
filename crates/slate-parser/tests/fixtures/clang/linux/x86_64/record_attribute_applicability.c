// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

struct Fields {
  char tag;
  int packed_member __attribute__((packed));
  int aligned_member __attribute__((aligned(8)));
  int malloc_member __attribute__((malloc));
  int weak_member __attribute__((weak));
  __attribute__((cold)) int first_cold, second_cold;
  int ms_member __attribute__((ms_struct));
};

struct __attribute__((packed)) Packed {
  char tag;
  int value;
};

struct __attribute__((transparent_union)) NotAUnion {
  int *value;
};

union __attribute__((transparent_union)) Transparent {
  int *signed_value;
  unsigned *unsigned_value;
};

struct Trailing {
  int value;
} __attribute__((used));

__attribute__((packed)) struct Leading {
  char tag;
  int value;
};

int sizes(void) {
  return sizeof(struct Fields) + sizeof(struct Packed) + sizeof(struct Leading);
}

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wignored-attributes
// WARN: ⚠ 'malloc' attribute ignored; it applies only to functions
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/record_attribute_applicability.c:6:36]
// WARN: 5 │   int aligned_member __attribute__((aligned(8)));
// WARN: 6 │   int malloc_member __attribute__((malloc));
// WARN: ·                                    ──────
// WARN: 7 │   int weak_member __attribute__((weak));
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'weak' attribute ignored
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/record_attribute_applicability.c:7:34]
// WARN: 6 │   int malloc_member __attribute__((malloc));
// WARN: 7 │   int weak_member __attribute__((weak));
// WARN: ·                                  ────
// WARN: 8 │   __attribute__((cold)) int first_cold, second_cold;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'cold' attribute ignored; it applies only to functions
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/record_attribute_applicability.c:8:18]
// WARN: 7 │   int weak_member __attribute__((weak));
// WARN: 8 │   __attribute__((cold)) int first_cold, second_cold;
// WARN: ·                  ────
// WARN: 9 │   int ms_member __attribute__((ms_struct));
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'cold' attribute ignored; it applies only to functions
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/record_attribute_applicability.c:8:18]
// WARN: 7 │   int weak_member __attribute__((weak));
// WARN: 8 │   __attribute__((cold)) int first_cold, second_cold;
// WARN: ·                  ────
// WARN: 9 │   int ms_member __attribute__((ms_struct));
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'ms_struct' attribute ignored; it applies only to structs, unions, and
// WARN: │ classes
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/record_attribute_applicability.c:9:32]
// WARN: 8 │   __attribute__((cold)) int first_cold, second_cold;
// WARN: 9 │   int ms_member __attribute__((ms_struct));
// WARN: ·                                ─────────
// WARN: 10 │ };
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'transparent_union' attribute ignored; it applies only to unions
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/record_attribute_applicability.c:17:23]
// WARN: 16 │
// WARN: 17 │ struct __attribute__((transparent_union)) NotAUnion {
// WARN: ·                       ─────────────────
// WARN: 18 │   int *value;
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'used' attribute ignored; it applies only to variables with non-local
// WARN: │ storage and functions
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/record_attribute_applicability.c:28:18]
// WARN: 27 │   int value;
// WARN: 28 │ } __attribute__((used));
// WARN: ·                  ────
// WARN: 29 │
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ attribute ignored; place it after the tag keyword to apply it to the type
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/record_attribute_applicability.c:30:16]
// WARN: 29 │
// WARN: 30 │ __attribute__((packed)) struct Leading {
// WARN: ·                ──────
// WARN: 31 │   char tag;
// WARN: ╰────
// SLATE-FILECHECK-END WARN
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
// DEFAULT-NEXT:     type @type[[TYPE_Fields:[0-9]+]] Fields = struct {
// DEFAULT-NEXT:         field0 tag: i8;
// DEFAULT-NEXT:         field1 packed_member: i32;
// DEFAULT-NEXT:         field2 aligned_member: i32;
// DEFAULT-NEXT:         field3 malloc_member: i32;
// DEFAULT-NEXT:         field4 weak_member: i32;
// DEFAULT-NEXT:         field5 first_cold: i32;
// DEFAULT-NEXT:         field6 second_cold: i32;
// DEFAULT-NEXT:         field7 ms_member: i32;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 1, 8, 12, 16, 20, 24, 28]];
// DEFAULT-NEXT:     type @type[[TYPE_Packed:[0-9]+]] Packed = struct {
// DEFAULT-NEXT:         field0 tag: i8;
// DEFAULT-NEXT:         field1 value: i32;
// DEFAULT-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE_NotAUnion:[0-9]+]] NotAUnion = struct {
// DEFAULT-NEXT:         field0 value: ptr<i32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Transparent:[0-9]+]] Transparent = union {
// DEFAULT-NEXT:         field0 signed_value: ptr<i32>;
// DEFAULT-NEXT:         field1 unsigned_value: ptr<u32>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_Trailing:[0-9]+]] Trailing = struct {
// DEFAULT-NEXT:         field0 value: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Leading:[0-9]+]] Leading = struct {
// DEFAULT-NEXT:         field0 tag: i8;
// DEFAULT-NEXT:         field1 value: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_sizes:[0-9]+]] @sizes() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32>(truncate<u32>(add<u64>(add<u64>(const<u64>(32), const<u64>(5)), const<u64>(8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN IR-WARN
// IR-WARN: module {
// IR-WARN-NEXT:     target "x86_64-unknown-linux-gnu" {
// IR-WARN-NEXT:         endian = little;
// IR-WARN-NEXT:         pointer [size=8, align=8];
// IR-WARN-NEXT:         stack_alignment = 16;
// IR-WARN-NEXT:         long_double = f80;
// IR-WARN-NEXT:         storage bool [size=1, align=1];
// IR-WARN-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN-NEXT:         storage f16 [size=2, align=2];
// IR-WARN-NEXT:         storage f32 [size=4, align=4];
// IR-WARN-NEXT:         storage f64 [size=8, align=8];
// IR-WARN-NEXT:         storage f80 [size=16, align=16];
// IR-WARN-NEXT:         storage f128 [size=16, align=16];
// IR-WARN-NEXT:         storage d32 [size=4, align=4];
// IR-WARN-NEXT:         storage d64 [size=8, align=8];
// IR-WARN-NEXT:         storage d128 [size=16, align=16];
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     type @type[[TYPE_Fields:[0-9]+]] Fields = struct {
// IR-WARN-NEXT:         field0 tag: i8;
// IR-WARN-NEXT:         field1 packed_member: i32;
// IR-WARN-NEXT:         field2 aligned_member: i32;
// IR-WARN-NEXT:         field3 malloc_member: i32;
// IR-WARN-NEXT:         field4 weak_member: i32;
// IR-WARN-NEXT:         field5 first_cold: i32;
// IR-WARN-NEXT:         field6 second_cold: i32;
// IR-WARN-NEXT:         field7 ms_member: i32;
// IR-WARN-NEXT:     } [size=32, align=8, offsets=[0, 1, 8, 12, 16, 20, 24, 28]];
// IR-WARN-NEXT:     type @type[[TYPE_Packed:[0-9]+]] Packed = struct {
// IR-WARN-NEXT:         field0 tag: i8;
// IR-WARN-NEXT:         field1 value: i32;
// IR-WARN-NEXT:     } [size=5, align=1, offsets=[0, 1]];
// IR-WARN-NEXT:     type @type[[TYPE_NotAUnion:[0-9]+]] NotAUnion = struct {
// IR-WARN-NEXT:         field0 value: ptr<i32>;
// IR-WARN-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-WARN-NEXT:     type @type[[TYPE_Transparent:[0-9]+]] Transparent = union {
// IR-WARN-NEXT:         field0 signed_value: ptr<i32>;
// IR-WARN-NEXT:         field1 unsigned_value: ptr<u32>;
// IR-WARN-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// IR-WARN-NEXT:     type @type[[TYPE_Trailing:[0-9]+]] Trailing = struct {
// IR-WARN-NEXT:         field0 value: i32;
// IR-WARN-NEXT:     } [size=4, align=4, offsets=[0]];
// IR-WARN-NEXT:     type @type[[TYPE_Leading:[0-9]+]] Leading = struct {
// IR-WARN-NEXT:         field0 tag: i8;
// IR-WARN-NEXT:         field1 value: i32;
// IR-WARN-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// IR-WARN-NEXT:     fn %[[VALUE_sizes:[0-9]+]] @sizes() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return reinterpret<i32>(truncate<u32>(add<u64>(add<u64>(const<u64>(32), const<u64>(5)), const<u64>(8))));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
