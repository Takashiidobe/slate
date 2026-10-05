// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir -std=gnu11

struct crypto_tfm {
  unsigned int flags;
  void *crt_ctx[] __attribute__((aligned(16)));
};

struct samples {
  char tag;
  short values[];
};

struct wide {
  char tag;
  long long values[];
};

struct crypto_tfm shared;

unsigned int ctx_alignment(void) {
  struct crypto_tfm *tfm;
  return __alignof__(tfm->crt_ctx);
}

unsigned int member_alignments(struct samples *s, struct wide *w) {
  return __alignof__(s->values) + _Alignof(s->values) + __alignof__(w->values) + _Alignof(w->values);
}

unsigned int object_alignment(void) {
  return __alignof__(shared.crt_ctx);
}

unsigned int type_alignment(void) {
  return __alignof__(short[]) + __alignof__(char[]);
}

enum { CTX_ALIGNMENT = __alignof__(((struct crypto_tfm *)0)->crt_ctx) };

_Static_assert(__alignof__(((struct samples *)0)->values) == 2, "flexible array member alignment");

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "i686-unknown-linux-gnu" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=4];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=4];
// IR-NEXT:         storage f80 [size=12, align=4];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_crypto_tfm:[0-9]+]] crypto_tfm = struct {
// IR-NEXT:         field0 flags: u32;
// IR-NEXT:         field1 crt_ctx: array<ptr<void>, incomplete>;
// IR-NEXT:     } [size=16, align=16, offsets=[0, 16]];
// IR-NEXT:     type @type[[TYPE_samples:[0-9]+]] samples = struct {
// IR-NEXT:         field0 tag: i8;
// IR-NEXT:         field1 values: array<i16, incomplete>;
// IR-NEXT:     } [size=2, align=2, offsets=[0, 2]];
// IR-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = struct {
// IR-NEXT:         field0 tag: i8;
// IR-NEXT:         field1 values: array<i64, incomplete>;
// IR-NEXT:     } [size=4, align=4, offsets=[0, 4]];
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// IR-NEXT:         %[[VALUE_CTX_ALIGNMENT:[0-9]+]] CTX_ALIGNMENT = const<i32>(16);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     global %[[VALUE_shared:[0-9]+]] shared: @type[[TYPE_crypto_tfm]] [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_ctx_alignment:[0-9]+]] @ctx_alignment() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_tfm:[0-9]+]] tfm: ptr<@type[[TYPE_crypto_tfm]]> [storage=automatic];
// IR-NEXT:         return const<u32>(16);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_member_alignments:[0-9]+]] @member_alignments(%[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_samples]]>, %[[VALUE_w:[0-9]+]] w: ptr<@type[[TYPE_wide]]>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u32>(add<u32>(add<u32>(const<u32>(2), const<u32>(2)), const<u32>(4)), const<u32>(4));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_object_alignment:[0-9]+]] @object_alignment() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u32>(16);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_type_alignment:[0-9]+]] @type_alignment() -> u32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u32>(const<u32>(2), const<u32>(1));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
