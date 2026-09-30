enum { EIGHT = 8, SIXTEEN = 16 };
typedef long double wide;

_Alignas(EIGHT) char by_enumerator;
char by_attribute __attribute__((aligned(SIXTEEN)));
_Alignas(wide) char by_typedef;

struct fields {
  char c;
  _Alignas(EIGHT) char enumerator;
  char attribute __attribute__((aligned(SIXTEEN)));
  _Alignas(wide) char type;
};

struct __attribute__((aligned(SIXTEEN))) tagged {
  char c;
};

typedef int aligned_int __attribute__((aligned(EIGHT)));
typedef int vector __attribute__((vector_size(SIXTEEN)));

struct fields fields;
struct tagged tagged;
aligned_int typedef_object;
vector vector_object;

_Static_assert(_Alignof(struct fields) == 16, "");
_Static_assert(__builtin_offsetof(struct fields, enumerator) == 8, "");
_Static_assert(__builtin_offsetof(struct fields, attribute) == 16, "");
_Static_assert(__builtin_offsetof(struct fields, type) == 32, "");
_Static_assert(_Alignof(struct tagged) == 16, "");
_Static_assert(_Alignof(aligned_int) == 8, "");

int local(void) {
  _Alignas(wide) char c = 1;
  return c;
}



// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_EIGHT:[0-9]+]] EIGHT = const<i32>(8);
// DEFAULT-NEXT:         %[[VALUE_SIXTEEN:[0-9]+]] SIXTEEN = const<i32>(16);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = f80;
// DEFAULT-NEXT:     type @type[[TYPE_fields:[0-9]+]] fields = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 enumerator: i8;
// DEFAULT-NEXT:         field2 attribute: i8;
// DEFAULT-NEXT:         field3 type: i8;
// DEFAULT-NEXT:     } [size=48, align=16, offsets=[0, 8, 16, 32]];
// DEFAULT-NEXT:     type @type[[TYPE_tagged:[0-9]+]] tagged = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_aligned_int:[0-9]+]] aligned_int = i32;
// DEFAULT-NEXT:     type @type[[TYPE_vector:[0-9]+]] vector = vector<i32, 4>;
// DEFAULT-NEXT:     global %[[VALUE_by_enumerator:[0-9]+]] by_enumerator: i8 [storage=static] [align=8] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_by_attribute:[0-9]+]] by_attribute: i8 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_by_typedef:[0-9]+]] by_typedef: i8 [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fields:[0-9]+]] fields: @type[[TYPE_fields]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_tagged:[0-9]+]] tagged: @type[[TYPE_tagged]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_typedef_object:[0-9]+]] typedef_object: i32 [storage=static] [align=8] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_vector_object:[0-9]+]] vector_object: vector<i32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_local:[0-9]+]] @local() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic] [align=16] = truncate<i8, reason=assign, fits=always>(const<i32>(1));
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
