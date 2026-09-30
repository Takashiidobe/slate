struct point {
  int x;
  int y;
};

int nested_statement_expression(int value) {
  int doubled = 1 + ({
    int local = value;
    local * 2;
  });
  return doubled;
}

int operators(struct point *p, int values[4]) {
  int i = 0;
  i += (p->x + values[i++]) * -p[0].y;
  i = i ? : !i;
  i = (i, --i);
  return (i > 0) ? i : ~i;
}

int address_of_label(void) {
  void *target = &&done;
  goto *target;
done:
  return sizeof(struct point) + sizeof target;
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
// DEFAULT-NEXT:     type @type[[TYPE_point:[0-9]+]] point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %[[VALUE_nested_statement_expression:[0-9]+]] @nested_statement_expression(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_doubled:[0-9]+]] doubled: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_local:[0-9]+]] local: i32 [storage=automatic] = read<i32>(%[[VALUE_value]]);
// DEFAULT-NEXT:             write<i32>(%[[VALUE0]], mul<i32, overflow=ub>(read<i32>(%[[VALUE_local]]), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(%[[VALUE_doubled]], add<i32, overflow=ub>(const<i32>(1), read<i32>(%[[VALUE0]])));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_doubled]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_operators:[0-9]+]] @operators(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_point]]>, %[[VALUE_values:[0-9]+]] values: ptr<i32> [array=4]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic, unsequenced] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic, unsequenced] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32, unsequenced>(%[[VALUE_i]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), mul<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(deref(read<ptr<@type[[TYPE_point]]>>(%[[VALUE_p]])))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_values]]), read<i32>(%[[VALUE2]]))))), neg<i32, overflow=ub>(read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_point]]>, subtract=false, element=@type[[TYPE_point]], overflow=ub>(read<ptr<@type[[TYPE_point]]>>(%[[VALUE_p]]), const<i32>(0))))))));
// DEFAULT-NEXT:         write<i32, unsequenced>(%[[VALUE_i]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_i]], conditional<i32>(ne<i32>(read<i32>(%[[VALUE5]]), const<i32>(0)), read<i32>(%[[VALUE5]]), from_bool<i32, reason=promotion>(not<bool>(ne<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))))));
// DEFAULT-NEXT:         read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic, unsequenced] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic, unsequenced] = sub<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32, unsequenced>(%[[VALUE_i]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         write<i32, unsequenced>(%[[VALUE_i]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         return conditional<i32>(gt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0)), read<i32>(%[[VALUE_i]]), not<i32>(read<i32>(%[[VALUE_i]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_address_of_label:[0-9]+]] @address_of_label() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_target:[0-9]+]] target: ptr<void> [storage=automatic] = label_addr<ptr<void>>(%[[VALUE_done:[0-9]+]]);
// DEFAULT-NEXT:         goto *read<ptr<void>>(%[[VALUE_target]]);
// DEFAULT-NEXT:         label %[[VALUE_done]] done:
// DEFAULT-NEXT:             return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(8), const<u64>(8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
