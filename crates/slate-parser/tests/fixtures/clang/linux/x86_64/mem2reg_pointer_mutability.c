#include <stdio.h>

struct Row {
  int cells[2];
};

static void set_cell(struct Row *row) {
  row->cells[1] = 9;
}

static int *mutable_identity(const int *value) { return (int *)value; }

int main(void) {
  struct Row row = {{3, 4}};
  set_cell(&row);
  int value                 = 7;
  *mutable_identity(&value) = 11;
  printf("%d %d\n", row.cells[1], value);
  return 0;
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
// DEFAULT-NEXT:     type @type[[TYPE_Row:[0-9]+]] Row = struct {
// DEFAULT-NEXT:         field0 cells: array<i32, 2>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_set_cell:[0-9]+]] @set_cell(%[[VALUE_row:[0-9]+]] row: ptr<@type[[TYPE_Row]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field0(deref(read<ptr<@type[[TYPE_Row]]>>(%[[VALUE_row]])))), const<i32>(1))), const<i32>(9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_mutable_identity:[0-9]+]] @mutable_identity(%[[VALUE_value:[0-9]+]] value: ptr<const i32>) -> ptr<i32> [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return pointer_cast<ptr<i32>, reason=explicit>(read<ptr<const i32>>(%[[VALUE_value]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_row_2:[0-9]+]] row: @type[[TYPE_Row]] [storage=automatic] = aggregate<@type[[TYPE_Row]], zero_fill=false>(field0 = aggregate<array<i32, 2>, zero_fill=false>(index0 = const<i32>(3), index1 = const<i32>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_Row]]>) -> void>(%[[VALUE_set_cell]], addr_of<ptr<@type[[TYPE_Row]]>>(%[[VALUE_row_2]]));
// DEFAULT-NEXT:         let %[[VALUE_value_2:[0-9]+]] value: i32 [storage=automatic] = const<i32>(7);
// DEFAULT-NEXT:         write<i32>(deref(call<ptr<i32>, signature=fn(ptr<const i32>) -> ptr<i32>>(%[[VALUE_mutable_identity]], pointer_cast<ptr<const i32>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_value_2]])))), const<i32>(11));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(7)>(%[[VALUE_str]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(2)>(field0(%[[VALUE_row_2]])), const<i32>(1)))), read<i32>(%[[VALUE_value_2]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
