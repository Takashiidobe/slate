struct record {
  char c;
  int i;
  int field __attribute__((uninitialized));
};

int use(int *, struct record *);

int global __attribute__((uninitialized));

int locals(int param, int ignored_param __attribute__((uninitialized))) {
  int plain;
  int initialized = 1;
  int opted_out __attribute__((uninitialized));
  int reserved __attribute__((__uninitialized__));
  [[gnu::uninitialized]] int gnu_scoped;
  [[__gnu__::__uninitialized__]] int gnu_reserved;
  [[clang::uninitialized]] int clang_scoped;
  [[uninitialized]] int unscoped;
  register int in_register __attribute__((uninitialized));
  static int persistent __attribute__((uninitialized));
  struct record r;
  return use(&plain, &r) + initialized + opted_out + reserved + gnu_scoped +
         gnu_reserved + clang_scoped + unscoped + in_register + persistent +
         param + ignored_param + global;
}

// SLATE-FILECHECK-ARGS --dump-ir -std=c23
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT
// SLATE-FILECHECK-DEFINES ZERO
// SLATE-FILECHECK-PREFIX-ARGS ZERO -ftrivial-auto-var-init=zero
// SLATE-FILECHECK-DEFINES PATTERN
// SLATE-FILECHECK-PREFIX-ARGS PATTERN -ftrivial-auto-var-init=pattern
// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wignored-attributes
// WARN:   ⚠ 'uninitialized' attribute ignored; it applies only to local variables
// WARN:    ╭─[tests/fixtures/gcc/linux/x86_64/auto_var_init.c:4:28]
// WARN:  3 │   int i;
// WARN:  4 │   int field __attribute__((uninitialized));
// WARN:    ·                            ─────────────
// WARN:  5 │ };
// WARN:    ╰────
// WARN: -Wignored-attributes
// WARN:   ⚠ 'uninitialized' attribute ignored; it applies only to local variables
// WARN:     ╭─[tests/fixtures/gcc/linux/x86_64/auto_var_init.c:9:27]
// WARN:   8 │
// WARN:   9 │ int global __attribute__((uninitialized));
// WARN:     ·                           ─────────────
// WARN:  10 │
// WARN:     ╰────
// WARN: -Wignored-attributes
// WARN:   ⚠ 'uninitialized' attribute ignored; it applies only to local variables
// WARN:     ╭─[tests/fixtures/gcc/linux/x86_64/auto_var_init.c:11:56]
// WARN:  10 │
// WARN:  11 │ int locals(int param, int ignored_param __attribute__((uninitialized))) {
// WARN:     ·                                                        ─────────────
// WARN:  12 │   int plain;
// WARN:     ╰────
// WARN: -Wunknown-attributes
// WARN:   ⚠ unknown attribute 'clang::uninitialized' ignored
// WARN:     ╭─[tests/fixtures/gcc/linux/x86_64/auto_var_init.c:18:5]
// WARN:  17 │   {{\[\[}}__gnu__::__uninitialized__]] int gnu_reserved;
// WARN:  18 │   {{\[\[}}clang::uninitialized]] int clang_scoped;
// WARN:     ·     ────────────────────
// WARN:  19 │   {{\[\[}}uninitialized]] int unscoped;
// WARN:     ╰────
// WARN: -Wignored-attributes
// WARN:   ⚠ 'uninitialized' attribute ignored; it applies only to local variables
// WARN:     ╭─[tests/fixtures/gcc/linux/x86_64/auto_var_init.c:21:40]
// WARN:  20 │   register int in_register __attribute__((uninitialized));
// WARN:  21 │   static int persistent __attribute__((uninitialized));
// WARN:     ·                                        ─────────────
// WARN:  22 │   struct record r;
// WARN:     ╰────
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
// DEFAULT-NEXT:     type @type[[TYPE_record:[0-9]+]] record = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:         field2 field: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     global %[[VALUE_global:[0-9]+]] global: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_persistent:[0-9]+]] persistent: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE_record]]>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_locals:[0-9]+]] @locals(%[[VALUE_param:[0-9]+]] param: i32, %[[VALUE_ignored_param:[0-9]+]] ignored_param: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_plain:[0-9]+]] plain: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_initialized:[0-9]+]] initialized: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         let %[[VALUE_opted_out:[0-9]+]] opted_out: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_reserved:[0-9]+]] reserved: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_gnu_scoped:[0-9]+]] gnu_scoped: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_gnu_reserved:[0-9]+]] gnu_reserved: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_clang_scoped:[0-9]+]] clang_scoped: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_unscoped:[0-9]+]] unscoped: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_in_register:[0-9]+]] in_register: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_record]] [storage=automatic];
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<i32>, ptr<@type[[TYPE_record]]>) -> i32>(%[[VALUE_use]], addr_of<ptr<i32>>(%[[VALUE_plain]]), addr_of<ptr<@type[[TYPE_record]]>>(%[[VALUE_r]])), read<i32>(%[[VALUE_initialized]])), read<i32>(%[[VALUE_opted_out]])), read<i32>(%[[VALUE_reserved]])), read<i32>(%[[VALUE_gnu_scoped]])), read<i32>(%[[VALUE_gnu_reserved]])), read<i32>(%[[VALUE_clang_scoped]])), read<i32>(%[[VALUE_unscoped]])), read<i32>(%[[VALUE_in_register]])), read<i32>(%[[VALUE_persistent]])), read<i32>(%[[VALUE_param]])), read<i32>(%[[VALUE_ignored_param]])), read<i32>(%[[VALUE_global]]));
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
// ZERO-NEXT:         field2 field: i32;
// ZERO-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// ZERO-NEXT:     global %[[VALUE_global:[0-9]+]] global: i32 [storage=static] [linkage=external];
// ZERO-NEXT:     global %[[VALUE_persistent:[0-9]+]] persistent: i32 [storage=static] [linkage=internal];
// ZERO-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE_record]]>) -> i32 [linkage=external];
// ZERO-NEXT:     fn %[[VALUE_locals:[0-9]+]] @locals(%[[VALUE_param:[0-9]+]] param: i32, %[[VALUE_ignored_param:[0-9]+]] ignored_param: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// ZERO-NEXT:         let %[[VALUE_plain:[0-9]+]] plain: i32 [storage=automatic] [auto_init=zero];
// ZERO-NEXT:         let %[[VALUE_initialized:[0-9]+]] initialized: i32 [storage=automatic] = const<i32>(1);
// ZERO-NEXT:         let %[[VALUE_opted_out:[0-9]+]] opted_out: i32 [storage=automatic];
// ZERO-NEXT:         let %[[VALUE_reserved:[0-9]+]] reserved: i32 [storage=automatic];
// ZERO-NEXT:         let %[[VALUE_gnu_scoped:[0-9]+]] gnu_scoped: i32 [storage=automatic];
// ZERO-NEXT:         let %[[VALUE_gnu_reserved:[0-9]+]] gnu_reserved: i32 [storage=automatic];
// ZERO-NEXT:         let %[[VALUE_clang_scoped:[0-9]+]] clang_scoped: i32 [storage=automatic] [auto_init=zero];
// ZERO-NEXT:         let %[[VALUE_unscoped:[0-9]+]] unscoped: i32 [storage=automatic] [auto_init=zero];
// ZERO-NEXT:         let %[[VALUE_in_register:[0-9]+]] in_register: i32 [storage=automatic];
// ZERO-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_record]] [storage=automatic] [auto_init=zero];
// ZERO-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<i32>, ptr<@type[[TYPE_record]]>) -> i32>(%[[VALUE_use]], addr_of<ptr<i32>>(%[[VALUE_plain]]), addr_of<ptr<@type[[TYPE_record]]>>(%[[VALUE_r]])), read<i32>(%[[VALUE_initialized]])), read<i32>(%[[VALUE_opted_out]])), read<i32>(%[[VALUE_reserved]])), read<i32>(%[[VALUE_gnu_scoped]])), read<i32>(%[[VALUE_gnu_reserved]])), read<i32>(%[[VALUE_clang_scoped]])), read<i32>(%[[VALUE_unscoped]])), read<i32>(%[[VALUE_in_register]])), read<i32>(%[[VALUE_persistent]])), read<i32>(%[[VALUE_param]])), read<i32>(%[[VALUE_ignored_param]])), read<i32>(%[[VALUE_global]]));
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
// PATTERN-NEXT:         field2 field: i32;
// PATTERN-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// PATTERN-NEXT:     global %[[VALUE_global:[0-9]+]] global: i32 [storage=static] [linkage=external];
// PATTERN-NEXT:     global %[[VALUE_persistent:[0-9]+]] persistent: i32 [storage=static] [linkage=internal];
// PATTERN-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE_record]]>) -> i32 [linkage=external];
// PATTERN-NEXT:     fn %[[VALUE_locals:[0-9]+]] @locals(%[[VALUE_param:[0-9]+]] param: i32, %[[VALUE_ignored_param:[0-9]+]] ignored_param: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// PATTERN-NEXT:         let %[[VALUE_plain:[0-9]+]] plain: i32 [storage=automatic] [auto_init=pattern(value_bits=0xfe)];
// PATTERN-NEXT:         let %[[VALUE_initialized:[0-9]+]] initialized: i32 [storage=automatic] = const<i32>(1);
// PATTERN-NEXT:         let %[[VALUE_opted_out:[0-9]+]] opted_out: i32 [storage=automatic];
// PATTERN-NEXT:         let %[[VALUE_reserved:[0-9]+]] reserved: i32 [storage=automatic];
// PATTERN-NEXT:         let %[[VALUE_gnu_scoped:[0-9]+]] gnu_scoped: i32 [storage=automatic];
// PATTERN-NEXT:         let %[[VALUE_gnu_reserved:[0-9]+]] gnu_reserved: i32 [storage=automatic];
// PATTERN-NEXT:         let %[[VALUE_clang_scoped:[0-9]+]] clang_scoped: i32 [storage=automatic] [auto_init=pattern(value_bits=0xfe)];
// PATTERN-NEXT:         let %[[VALUE_unscoped:[0-9]+]] unscoped: i32 [storage=automatic] [auto_init=pattern(value_bits=0xfe)];
// PATTERN-NEXT:         let %[[VALUE_in_register:[0-9]+]] in_register: i32 [storage=automatic];
// PATTERN-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_record]] [storage=automatic] [auto_init=pattern(value_bits=0xfe)];
// PATTERN-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<i32>, ptr<@type[[TYPE_record]]>) -> i32>(%[[VALUE_use]], addr_of<ptr<i32>>(%[[VALUE_plain]]), addr_of<ptr<@type[[TYPE_record]]>>(%[[VALUE_r]])), read<i32>(%[[VALUE_initialized]])), read<i32>(%[[VALUE_opted_out]])), read<i32>(%[[VALUE_reserved]])), read<i32>(%[[VALUE_gnu_scoped]])), read<i32>(%[[VALUE_gnu_reserved]])), read<i32>(%[[VALUE_clang_scoped]])), read<i32>(%[[VALUE_unscoped]])), read<i32>(%[[VALUE_in_register]])), read<i32>(%[[VALUE_persistent]])), read<i32>(%[[VALUE_param]])), read<i32>(%[[VALUE_ignored_param]])), read<i32>(%[[VALUE_global]]));
// PATTERN-NEXT:     }
// PATTERN-NEXT: }
// SLATE-FILECHECK-END PATTERN
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
// IR-WARN-NEXT:     type @type[[TYPE_record:[0-9]+]] record = struct {
// IR-WARN-NEXT:         field0 c: i8;
// IR-WARN-NEXT:         field1 i: i32;
// IR-WARN-NEXT:         field2 field: i32;
// IR-WARN-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// IR-WARN-NEXT:     global %[[VALUE_global:[0-9]+]] global: i32 [storage=static] [linkage=external];
// IR-WARN-NEXT:     global %[[VALUE_persistent:[0-9]+]] persistent: i32 [storage=static] [linkage=internal];
// IR-WARN-NEXT:     fn %[[VALUE_use:[0-9]+]] @use(%[[VALUE0:[0-9]+]] <unnamed>: ptr<i32>, %[[VALUE1:[0-9]+]] <unnamed>: ptr<@type[[TYPE_record]]>) -> i32 [linkage=external];
// IR-WARN-NEXT:     fn %[[VALUE_locals:[0-9]+]] @locals(%[[VALUE_param:[0-9]+]] param: i32, %[[VALUE_ignored_param:[0-9]+]] ignored_param: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         let %[[VALUE_plain:[0-9]+]] plain: i32 [storage=automatic];
// IR-WARN-NEXT:         let %[[VALUE_initialized:[0-9]+]] initialized: i32 [storage=automatic] = const<i32>(1);
// IR-WARN-NEXT:         let %[[VALUE_opted_out:[0-9]+]] opted_out: i32 [storage=automatic];
// IR-WARN-NEXT:         let %[[VALUE_reserved:[0-9]+]] reserved: i32 [storage=automatic];
// IR-WARN-NEXT:         let %[[VALUE_gnu_scoped:[0-9]+]] gnu_scoped: i32 [storage=automatic];
// IR-WARN-NEXT:         let %[[VALUE_gnu_reserved:[0-9]+]] gnu_reserved: i32 [storage=automatic];
// IR-WARN-NEXT:         let %[[VALUE_clang_scoped:[0-9]+]] clang_scoped: i32 [storage=automatic];
// IR-WARN-NEXT:         let %[[VALUE_unscoped:[0-9]+]] unscoped: i32 [storage=automatic];
// IR-WARN-NEXT:         let %[[VALUE_in_register:[0-9]+]] in_register: i32 [storage=automatic];
// IR-WARN-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_record]] [storage=automatic];
// IR-WARN-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<i32>, ptr<@type[[TYPE_record]]>) -> i32>(%[[VALUE_use]], addr_of<ptr<i32>>(%[[VALUE_plain]]), addr_of<ptr<@type[[TYPE_record]]>>(%[[VALUE_r]])), read<i32>(%[[VALUE_initialized]])), read<i32>(%[[VALUE_opted_out]])), read<i32>(%[[VALUE_reserved]])), read<i32>(%[[VALUE_gnu_scoped]])), read<i32>(%[[VALUE_gnu_reserved]])), read<i32>(%[[VALUE_clang_scoped]])), read<i32>(%[[VALUE_unscoped]])), read<i32>(%[[VALUE_in_register]])), read<i32>(%[[VALUE_persistent]])), read<i32>(%[[VALUE_param]])), read<i32>(%[[VALUE_ignored_param]])), read<i32>(%[[VALUE_global]]));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
