typedef char __attribute__((aligned(8))) aligned_char;

int leading = _Alignof(__attribute__((aligned(16))) char);
int trailing = _Alignof(char __attribute__((aligned(16))));
int unchanged_size = sizeof(__attribute__((aligned(16))) char[3]);
int whole_type = _Alignof(__attribute__((aligned(16))) char *);
int reduced = _Alignof(__attribute__((aligned(2))) int);
int biggest = _Alignof(__attribute__((aligned)) char);
int through_typeof = _Alignof(__typeof__((__attribute__((aligned(16))) char)0));

void f(void) {
  char *literal = &(__attribute__((aligned(16))) char){1};
  char *typedef_literal = &(aligned_char){2};
}

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wignored-attributes
// WARN: ⚠ 'aligned' attribute ignored when parsing type
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/type_name_aligned.c:3:39]
// WARN: 2 │
// WARN: 3 │ int leading = _Alignof(__attribute__((aligned(16))) char);
// WARN: ·                                       ───────
// WARN: 4 │ int trailing = _Alignof(char __attribute__((aligned(16))));
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'aligned' attribute ignored when parsing type
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/type_name_aligned.c:4:45]
// WARN: 3 │ int leading = _Alignof(__attribute__((aligned(16))) char);
// WARN: 4 │ int trailing = _Alignof(char __attribute__((aligned(16))));
// WARN: ·                                             ───────
// WARN: 5 │ int unchanged_size = sizeof(__attribute__((aligned(16))) char[3]);
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'aligned' attribute ignored when parsing type
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/type_name_aligned.c:5:44]
// WARN: 4 │ int trailing = _Alignof(char __attribute__((aligned(16))));
// WARN: 5 │ int unchanged_size = sizeof(__attribute__((aligned(16))) char[3]);
// WARN: ·                                            ───────
// WARN: 6 │ int whole_type = _Alignof(__attribute__((aligned(16))) char *);
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'aligned' attribute ignored when parsing type
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/type_name_aligned.c:6:42]
// WARN: 5 │ int unchanged_size = sizeof(__attribute__((aligned(16))) char[3]);
// WARN: 6 │ int whole_type = _Alignof(__attribute__((aligned(16))) char *);
// WARN: ·                                          ───────
// WARN: 7 │ int reduced = _Alignof(__attribute__((aligned(2))) int);
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'aligned' attribute ignored when parsing type
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/type_name_aligned.c:7:39]
// WARN: 6 │ int whole_type = _Alignof(__attribute__((aligned(16))) char *);
// WARN: 7 │ int reduced = _Alignof(__attribute__((aligned(2))) int);
// WARN: ·                                       ───────
// WARN: 8 │ int biggest = _Alignof(__attribute__((aligned)) char);
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'aligned' attribute ignored when parsing type
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/type_name_aligned.c:8:39]
// WARN: 7 │ int reduced = _Alignof(__attribute__((aligned(2))) int);
// WARN: 8 │ int biggest = _Alignof(__attribute__((aligned)) char);
// WARN: ·                                       ───────
// WARN: 9 │ int through_typeof = _Alignof(__typeof__((__attribute__((aligned(16))) char)0));
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'aligned' attribute ignored when parsing type
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/type_name_aligned.c:9:58]
// WARN: 8 │ int biggest = _Alignof(__attribute__((aligned)) char);
// WARN: 9 │ int through_typeof = _Alignof(__typeof__((__attribute__((aligned(16))) char)0));
// WARN: ·                                                          ───────
// WARN: 10 │
// WARN: ╰────
// WARN: -Wignored-attributes
// WARN: ⚠ 'aligned' attribute ignored when parsing type
// WARN: ╭─[tests/fixtures/clang/linux/x86_64/type_name_aligned.c:12:36]
// WARN: 11 │ void f(void) {
// WARN: 12 │   char *literal = &(__attribute__((aligned(16))) char){1};
// WARN: ·                                    ───────
// WARN: 13 │   char *typedef_literal = &(aligned_char){2};
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
// DEFAULT-NEXT:     type @type[[TYPE_aligned_char:[0-9]+]] aligned_char = i8;
// DEFAULT-NEXT:     global %[[VALUE_leading:[0-9]+]] leading: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_trailing:[0-9]+]] trailing: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_unchanged_size:[0-9]+]] unchanged_size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(3))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_whole_type:[0-9]+]] whole_type: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_reduced:[0-9]+]] reduced: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_biggest:[0-9]+]] biggest: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(1))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_through_typeof:[0-9]+]] through_typeof: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(1))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_literal:[0-9]+]] literal: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE_typedef_literal:[0-9]+]] typedef_literal: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] [align=8] = truncate<i8, reason=assign, fits=always>(const<i32>(2)));
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
// IR-WARN-NEXT:     type @type[[TYPE_aligned_char:[0-9]+]] aligned_char = i8;
// IR-WARN-NEXT:     global %[[VALUE_leading:[0-9]+]] leading: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(1))) [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_trailing:[0-9]+]] trailing: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(1))) [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_unchanged_size:[0-9]+]] unchanged_size: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(3))) [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_whole_type:[0-9]+]] whole_type: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(8))) [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_reduced:[0-9]+]] reduced: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))) [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_biggest:[0-9]+]] biggest: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(1))) [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_through_typeof:[0-9]+]] through_typeof: i32 [storage=static] = reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(1))) [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_f:[0-9]+]] @f() -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:         let %[[VALUE_literal:[0-9]+]] literal: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(compound_literal %[[VALUE0:[0-9]+]] [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// IR-WARN-NEXT:         let %[[VALUE_typedef_literal:[0-9]+]] typedef_literal: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(compound_literal %[[VALUE1:[0-9]+]] [storage=automatic] [align=8] = truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
