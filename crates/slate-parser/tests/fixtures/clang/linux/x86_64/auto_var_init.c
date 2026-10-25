struct record {
  char c;
  int i;
};

int use(int *, struct record *);

int locals(int param) {
  int plain;
  int initialized = 1;
  int opted_out __attribute__((uninitialized));
  static int persistent;
  struct record r;
  return use(&plain, &r) + initialized + opted_out + persistent + param;
}

// SLATE-FILECHECK-ARGS --dump-ir
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES ZERO
// SLATE-FILECHECK-PREFIX-ARGS ZERO -ftrivial-auto-var-init=zero
// SLATE-FILECHECK-DEFINES PATTERN
// SLATE-FILECHECK-PREFIX-ARGS PATTERN -ftrivial-auto-var-init=pattern

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
// DEFAULT-NEXT:     type @type[[TYPE_record:[0-9]+]] record = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_persistent:[0-9]+]] persistent: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE_record]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_locals:[0-9]+]] @locals(%[[VALUE_param:[0-9]+]] param: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_plain:[0-9]+]] plain: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_initialized:[0-9]+]] initialized: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_opted_out:[0-9]+]] opted_out: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_record]] [storage=automatic];
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<i32>, ptr<@type[[TYPE_record]]>) -> i32>(%[[VALUE_use]], addr_of<ptr<i32>>(%[[VALUE_plain]]), addr_of<ptr<@type[[TYPE_record]]>>(%[[VALUE_r]])), read<i32>(%[[VALUE_initialized]])), read<i32>(%[[VALUE_opted_out]])), read<i32>(%[[VALUE_persistent]])), read<i32>(%[[VALUE_param]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
// SLATE-FILECHECK-BEGIN ZERO
// ZERO: module {
// ZERO-NEXT:     target "x86_64-unknown-linux-gnu" {
// ZERO-NEXT:         endian = little;
// ZERO-NEXT:         pointer [size=8, align=8];
// ZERO-NEXT:         stack_alignment = 16;
// ZERO-NEXT:         long_double = f80;
// ZERO-NEXT:         storage bool [size=1, align=1];
// ZERO-NEXT:         storage i8, u8 [size=1, align=1];
// ZERO-NEXT:         storage i16, u16 [size=2, align=2];
// ZERO-NEXT:         storage i32, u32 [size=4, align=4];
// ZERO-NEXT:         storage i64, u64 [size=8, align=8];
// ZERO-NEXT:         storage i128, u128 [size=16, align=16];
// ZERO-NEXT:         storage bf16 [size=2, align=2];
// ZERO-NEXT:         storage f16 [size=2, align=2];
// ZERO-NEXT:         storage f32 [size=4, align=4];
// ZERO-NEXT:         storage f64 [size=8, align=8];
// ZERO-NEXT:         storage f80 [size=16, align=16];
// ZERO-NEXT:         storage f128 [size=16, align=16];
// ZERO-NEXT:         storage d32 [size=4, align=4];
// ZERO-NEXT:         storage d64 [size=8, align=8];
// ZERO-NEXT:         storage d128 [size=16, align=16];
// ZERO-NEXT:     }
// ZERO-NEXT:     type @type[[TYPE_record:[0-9]+]] record = struct {
// ZERO-NEXT:         field0 c: i8;
// ZERO-NEXT:         field1 i: i32;
// ZERO-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// ZERO-NEXT:     global %[[VALUE_persistent:[0-9]+]] persistent: i32 [storage=static] [linkage=internal];
// ZERO-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE_record]]>) -> i32 [linkage=external];
// ZERO-NEXT:     fn %[[VALUE_locals:[0-9]+]] @locals(%[[VALUE_param:[0-9]+]] param: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// ZERO-NEXT:         let %[[VALUE_plain:[0-9]+]] plain: i32 [storage=automatic] [auto_init=zero];
// ZERO-NEXT:         let %[[VALUE_initialized:[0-9]+]] initialized: i32 [storage=automatic] = const<i32>(1);
// ZERO-NEXT:         let %[[VALUE_opted_out:[0-9]+]] opted_out: i32 [storage=automatic];
// ZERO-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_record]] [storage=automatic] [auto_init=zero];
// ZERO-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<i32>, ptr<@type[[TYPE_record]]>) -> i32>(%[[VALUE_use]], addr_of<ptr<i32>>(%[[VALUE_plain]]), addr_of<ptr<@type[[TYPE_record]]>>(%[[VALUE_r]])), read<i32>(%[[VALUE_initialized]])), read<i32>(%[[VALUE_opted_out]])), read<i32>(%[[VALUE_persistent]])), read<i32>(%[[VALUE_param]]));
// ZERO-NEXT:     }
// ZERO-NEXT: }
// SLATE-FILECHECK-END ZERO
// SLATE-FILECHECK-BEGIN PATTERN
// PATTERN: module {
// PATTERN-NEXT:     target "x86_64-unknown-linux-gnu" {
// PATTERN-NEXT:         endian = little;
// PATTERN-NEXT:         pointer [size=8, align=8];
// PATTERN-NEXT:         stack_alignment = 16;
// PATTERN-NEXT:         long_double = f80;
// PATTERN-NEXT:         storage bool [size=1, align=1];
// PATTERN-NEXT:         storage i8, u8 [size=1, align=1];
// PATTERN-NEXT:         storage i16, u16 [size=2, align=2];
// PATTERN-NEXT:         storage i32, u32 [size=4, align=4];
// PATTERN-NEXT:         storage i64, u64 [size=8, align=8];
// PATTERN-NEXT:         storage i128, u128 [size=16, align=16];
// PATTERN-NEXT:         storage bf16 [size=2, align=2];
// PATTERN-NEXT:         storage f16 [size=2, align=2];
// PATTERN-NEXT:         storage f32 [size=4, align=4];
// PATTERN-NEXT:         storage f64 [size=8, align=8];
// PATTERN-NEXT:         storage f80 [size=16, align=16];
// PATTERN-NEXT:         storage f128 [size=16, align=16];
// PATTERN-NEXT:         storage d32 [size=4, align=4];
// PATTERN-NEXT:         storage d64 [size=8, align=8];
// PATTERN-NEXT:         storage d128 [size=16, align=16];
// PATTERN-NEXT:     }
// PATTERN-NEXT:     type @type[[TYPE_record:[0-9]+]] record = struct {
// PATTERN-NEXT:         field0 c: i8;
// PATTERN-NEXT:         field1 i: i32;
// PATTERN-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// PATTERN-NEXT:     global %[[VALUE_persistent:[0-9]+]] persistent: i32 [storage=static] [linkage=internal];
// PATTERN-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE_record]]>) -> i32 [linkage=external];
// PATTERN-NEXT:     fn %[[VALUE_locals:[0-9]+]] @locals(%[[VALUE_param:[0-9]+]] param: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// PATTERN-NEXT:         let %[[VALUE_plain:[0-9]+]] plain: i32 [storage=automatic] [auto_init=pattern(integer=0xaa, floating=0xff, lone_bool=0xaa, padding=0xaa, bit_runs=false)];
// PATTERN-NEXT:         let %[[VALUE_initialized:[0-9]+]] initialized: i32 [storage=automatic] = const<i32>(1);
// PATTERN-NEXT:         let %[[VALUE_opted_out:[0-9]+]] opted_out: i32 [storage=automatic];
// PATTERN-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_record]] [storage=automatic] [auto_init=pattern(integer=0xaa, floating=0xff, lone_bool=0xaa, padding=0xaa, bit_runs=false)];
// PATTERN-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<i32>, ptr<@type[[TYPE_record]]>) -> i32>(%[[VALUE_use]], addr_of<ptr<i32>>(%[[VALUE_plain]]), addr_of<ptr<@type[[TYPE_record]]>>(%[[VALUE_r]])), read<i32>(%[[VALUE_initialized]])), read<i32>(%[[VALUE_opted_out]])), read<i32>(%[[VALUE_persistent]])), read<i32>(%[[VALUE_param]]));
// PATTERN-NEXT:     }
// PATTERN-NEXT: }
// SLATE-FILECHECK-END PATTERN
