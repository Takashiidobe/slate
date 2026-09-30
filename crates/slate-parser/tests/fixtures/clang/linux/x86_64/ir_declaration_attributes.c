// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-STD IR gnu23

[[deprecated("old")]] int deprecated_object;
[[maybe_unused]] int unused_object;
[[vendor::custom(7)]] int vendor_object;

typedef short may_alias_short __attribute__((__may_alias__));
typedef int labeled_alias asm("alias_symbol");
typedef int __attribute__((deprecated)) deprecated_alias;

extern int renamed asm("real_name");
static int placed __attribute__((section("data.custom"))) = 1;
__attribute__((visibility("hidden"), used)) int exported = 2;

may_alias_short aliased;
deprecated_alias reused;

int locals(void) {
  int aligned_local __attribute__((aligned(16))) = 0;
  int __attribute__((vector_size(16))) vector_local = {0};
  int labeled_local asm("ignored_on_locals") = 3;
  [[maybe_unused]] int annotated_local = 4;
  return aligned_local + labeled_local + annotated_local + vector_local[0];
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
// IR-NEXT:     type @type[[TYPE_may_alias_short:[0-9]+]] may_alias_short = i16;
// IR-NEXT:     type @type[[TYPE_labeled_alias:[0-9]+]] labeled_alias = i32;
// IR-NEXT:     type @type[[TYPE_deprecated_alias:[0-9]+]] deprecated_alias = i32;
// IR-NEXT:     global %[[VALUE_deprecated_object:[0-9]+]] deprecated_object: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_unused_object:[0-9]+]] unused_object: i32 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_vendor_object:[0-9]+]] vendor_object: i32 [storage=static] [linkage=external];
// IR-NEXT:     extern %[[VALUE_renamed:[0-9]+]] renamed: i32 [storage=static] [linkage=external] [asm_name="real_name"];
// IR-NEXT:     global %[[VALUE_placed:[0-9]+]] placed: i32 [storage=static] = const<i32>(1) [linkage=internal] [section="data.custom"];
// IR-NEXT:     global %[[VALUE_exported:[0-9]+]] exported: i32 [storage=static] = const<i32>(2) [linkage=external] [visibility=hidden] [used];
// IR-NEXT:     global %[[VALUE_aliased:[0-9]+]] aliased: i16 [storage=static] [linkage=external];
// IR-NEXT:     global %[[VALUE_reused:[0-9]+]] reused: i32 [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_locals:[0-9]+]] @locals() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_aligned_local:[0-9]+]] aligned_local: i32 [storage=automatic] [align=16] = const<i32>(0);
// IR-NEXT:         let %[[VALUE_vector_local:[0-9]+]] vector_local: vector<i32, 4> [storage=automatic] = aggregate<vector<i32, 4>, zero_fill=true>(index0 = const<i32>(0));
// IR-NEXT:         let %[[VALUE_labeled_local:[0-9]+]] labeled_local: i32 [storage=automatic] = const<i32>(3);
// IR-NEXT:         let %[[VALUE_annotated_local:[0-9]+]] annotated_local: i32 [storage=automatic] = const<i32>(4);
// IR-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_aligned_local]]), read<i32>(%[[VALUE_labeled_local]])), read<i32>(%[[VALUE_annotated_local]])), read<i32>(lane(%[[VALUE_vector_local]], const<i32>(0))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
