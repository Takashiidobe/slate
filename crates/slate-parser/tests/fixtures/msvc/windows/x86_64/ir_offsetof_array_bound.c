// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#include <stddef.h>

typedef struct Item { int a; char *b; long long c; } Item;
typedef struct List { int n; int alloc; Item a[1]; } List;
typedef struct Level { int n; void *p; } Level;
typedef struct Structure { int ref; unsigned long long gen; int nLevel; Level aLevel[1]; } Structure;

#define SZ_LIST_1 (offsetof(List, a) + sizeof(Item))
#define SZ_STRUCTURE(N) (offsetof(Structure, aLevel) + (N) * sizeof(Level))

_Static_assert(SZ_LIST_1 == 32, "list bound");
_Static_assert(SZ_STRUCTURE(1) == 40, "structure bound");

int bounds(void) {
  union {
    List list;
    unsigned char buf[SZ_LIST_1];
  } list_storage;
  union {
    Structure structure;
    unsigned char buf[SZ_STRUCTURE(1)];
  } structure_storage;
  return (int)(sizeof(list_storage.buf) + sizeof(structure_storage.buf));
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
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
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// IR-NEXT:     type @type[[TYPE_size_t_2:[0-9]+]] size_t = u64;
// IR-NEXT:     type @type[[TYPE_Item:[0-9]+]] Item = struct {
// IR-NEXT:         field0 a: i32;
// IR-NEXT:         field1 b: ptr<i8>;
// IR-NEXT:         field2 c: i64;
// IR-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// IR-NEXT:     type @type[[TYPE_Item_2:[0-9]+]] Item = @type[[TYPE_Item]];
// IR-NEXT:     type @type[[TYPE_List:[0-9]+]] List = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 alloc: i32;
// IR-NEXT:         field2 a: array<@type[[TYPE_Item]], 1>;
// IR-NEXT:     } [size=32, align=8, offsets=[0, 4, 8]];
// IR-NEXT:     type @type[[TYPE_List_2:[0-9]+]] List = @type[[TYPE_List]];
// IR-NEXT:     type @type[[TYPE_Level:[0-9]+]] Level = struct {
// IR-NEXT:         field0 n: i32;
// IR-NEXT:         field1 p: ptr<void>;
// IR-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// IR-NEXT:     type @type[[TYPE_Level_2:[0-9]+]] Level = @type[[TYPE_Level]];
// IR-NEXT:     type @type[[TYPE_Structure:[0-9]+]] Structure = struct {
// IR-NEXT:         field0 ref: i32;
// IR-NEXT:         field1 gen: u64;
// IR-NEXT:         field2 nLevel: i32;
// IR-NEXT:         field3 aLevel: array<@type[[TYPE_Level]], 1>;
// IR-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24]];
// IR-NEXT:     type @type[[TYPE_Structure_2:[0-9]+]] Structure = @type[[TYPE_Structure]];
// IR-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// IR-NEXT:         field0 list: @type[[TYPE_List]];
// IR-NEXT:         field1 buf: array<u8, 32>;
// IR-NEXT:     } [size=32, align=8, offsets=[0, 0]];
// IR-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// IR-NEXT:         field0 structure: @type[[TYPE_Structure]];
// IR-NEXT:         field1 buf: array<u8, 40>;
// IR-NEXT:     } [size=40, align=8, offsets=[0, 0]];
// IR-NEXT:     fn %[[VALUE_bounds:[0-9]+]] @bounds() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_list_storage:[0-9]+]] list_storage: @type[[TYPE0]] [storage=automatic];
// IR-NEXT:         let %[[VALUE_structure_storage:[0-9]+]] structure_storage: @type[[TYPE1]] [storage=automatic];
// IR-NEXT:         return reinterpret<i32>(truncate<u32>(add<u64>(const<u64>(32), const<u64>(40))));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
