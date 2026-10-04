// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT

typedef struct { void *lock; ; } guard_t;

struct s {
  ;
  int x;
  ;;
};

union u {
  int y;
  ;
};

int size_s = sizeof(struct s);
int size_u = sizeof(union u);
int size_guard = sizeof(guard_t);

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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 lock: ptr<void>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_guard_t:[0-9]+]] guard_t = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE_s:[0-9]+]] s = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_u:[0-9]+]] u = union {
// DEFAULT-NEXT:         field0 y: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_size_s:[0-9]+]] size_s: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_size_u:[0-9]+]] size_u: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(4))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_size_guard:[0-9]+]] size_guard: i32 [storage=static] = reinterpret<i32>(truncate<u32>(const<u64>(8))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
