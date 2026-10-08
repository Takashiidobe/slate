struct pair { int first; int array[2]; };
static_assert((constexpr int){3} == 3);
static_assert((constexpr int){} == 0);
static_assert(_Generic(&(constexpr int[]){1, 2}, const int (*)[2]: 1, default: 0));
static_assert(sizeof (thread_local int){2} == sizeof(int));
static_assert((constexpr struct pair){.first = 7}.first == 7);
static_assert((constexpr struct pair){.array = {5, 6}}.first == 0);
int *first(void) { return &(static int){1}; }
int *second(void) { return &(static int){1}; }
int *thread(void) { return &(static thread_local int){2}; }
int automatic(int value) { return (int){value}; }
int constant(void) { return (constexpr int){5}; }
int *initialized(void) { return &(static int){(constexpr int){7}}; }
int member(void) { return (constexpr struct pair){.first = 8}.first; }
int *registered_pointer(int *p) { return &*(register int *){p}; }
int registered(void) { return (register int){3}; }

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-PREFIX-ARGS DEFAULT --dump-ir
// SLATE-FILECHECK-STD DEFAULT c23

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
// DEFAULT-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// DEFAULT-NEXT:         field0 first: i32;
// DEFAULT-NEXT:         field1 array: array<i32, 2>;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_first:[0-9]+]] @first() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(compound_literal %[[VALUE0:[0-9]+]] [storage=static] = const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_second:[0-9]+]] @second() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(compound_literal %[[VALUE1:[0-9]+]] [storage=static] = const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_thread:[0-9]+]] @thread() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(compound_literal %[[VALUE2:[0-9]+]] [storage=thread] = const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_automatic:[0-9]+]] @automatic(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(compound_literal %[[VALUE3:[0-9]+]] [storage=automatic] = read<i32>(%[[VALUE_value]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_constant:[0-9]+]] @constant() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(5);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_initialized:[0-9]+]] @initialized() -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(compound_literal %[[VALUE4:[0-9]+]] [storage=static] = const<i32>(7));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_member:[0-9]+]] @member() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(8);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_registered_pointer:[0-9]+]] @registered_pointer(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return addr_of<ptr<i32>>(deref(read<ptr<i32>>(compound_literal %[[VALUE5:[0-9]+]] [storage=automatic] = read<ptr<i32>>(%[[VALUE_p]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_registered:[0-9]+]] @registered() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(compound_literal %[[VALUE6:[0-9]+]] [storage=automatic] = const<i32>(3));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
