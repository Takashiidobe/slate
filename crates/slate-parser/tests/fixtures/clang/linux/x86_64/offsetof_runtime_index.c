// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir

enum pid_type { PIDTYPE_PID, PIDTYPE_TGID, PIDTYPE_MAX };

struct hlist_node { struct hlist_node *next; };
struct inner { char tag; short slots[3]; };
struct task { int state; struct hlist_node pid_links[PIDTYPE_MAX]; struct inner grid[2][3]; };

#define container_of(ptr, type, member) \
  ((type *)((char *)(ptr) - __builtin_offsetof(type, member)))

struct task *task_of(struct hlist_node *node, enum pid_type type) {
  return container_of(node, struct task, pid_links[type]);
}

unsigned long nested(int row, unsigned char column, long slot) {
  return __builtin_offsetof(struct task, grid[row][column].slots[slot]);
}

unsigned long mixed(int column) {
  return __builtin_offsetof(struct task, grid[1][column].slots[2]);
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
// IR-NEXT:     type @type[[TYPE_pid_type:[0-9]+]] pid_type = enum : u32 {
// IR-NEXT:         %[[VALUE_PIDTYPE_PID:[0-9]+]] PIDTYPE_PID = const<i32>(0);
// IR-NEXT:         %[[VALUE_PIDTYPE_TGID:[0-9]+]] PIDTYPE_TGID = const<i32>(1);
// IR-NEXT:         %[[VALUE_PIDTYPE_MAX:[0-9]+]] PIDTYPE_MAX = const<i32>(2);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     type @type[[TYPE_hlist_node:[0-9]+]] hlist_node = struct {
// IR-NEXT:         field0 next: ptr<@type[[TYPE_hlist_node]]>;
// IR-NEXT:     } [size=8, align=8, offsets=[0]];
// IR-NEXT:     type @type[[TYPE_inner:[0-9]+]] inner = struct {
// IR-NEXT:         field0 tag: i8;
// IR-NEXT:         field1 slots: array<i16, 3>;
// IR-NEXT:     } [size=8, align=2, offsets=[0, 2]];
// IR-NEXT:     type @type[[TYPE_task:[0-9]+]] task = struct {
// IR-NEXT:         field0 state: i32;
// IR-NEXT:         field1 pid_links: array<@type[[TYPE_hlist_node]], 2>;
// IR-NEXT:         field2 grid: array<array<@type[[TYPE_inner]], 3>, 2>;
// IR-NEXT:     } [size=72, align=8, offsets=[0, 8, 24]];
// IR-NEXT:     fn %[[VALUE_task_of:[0-9]+]] @task_of(%[[VALUE_node:[0-9]+]] node: ptr<@type[[TYPE_hlist_node]]>, %[[VALUE_type:[0-9]+]] type: @type[[TYPE_pid_type]]) -> ptr<@type[[TYPE_task]]> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return pointer_cast<ptr<@type[[TYPE_task]]>, reason=explicit>(ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(read<ptr<@type[[TYPE_hlist_node]]>>(%[[VALUE_node]])), add<u64, overflow=wrap>(const<u64>(8), mul<u64, overflow=wrap>(widen<u64, reason=explicit>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_pid_type]]>(%[[VALUE_type]]))), const<u64>(8)))));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_nested:[0-9]+]] @nested(%[[VALUE_row:[0-9]+]] row: i32, %[[VALUE_column:[0-9]+]] column: u8, %[[VALUE_slot:[0-9]+]] slot: i64) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64, overflow=wrap>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(26), mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%[[VALUE_row]]))), const<u64>(24))), mul<u64, overflow=wrap>(widen<u64, reason=explicit>(read<u8>(%[[VALUE_column]])), const<u64>(8))), mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(read<i64>(%[[VALUE_slot]])), const<u64>(2)));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_mixed:[0-9]+]] @mixed(%[[VALUE_column_2:[0-9]+]] column: i32) -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return add<u64, overflow=wrap>(const<u64>(54), mul<u64, overflow=wrap>(reinterpret<u64, reason=explicit, fits=unknown>(widen<i64, reason=explicit>(read<i32>(%[[VALUE_column_2]]))), const<u64>(8)));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
