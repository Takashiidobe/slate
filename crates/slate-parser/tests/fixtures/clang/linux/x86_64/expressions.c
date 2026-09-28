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
// DEFAULT-NEXT:     type @type0 point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     fn %1 @nested_statement_expression(%2 value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %3 doubled: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %4 local: i32 [storage=automatic] = read<i32>(%2);
// DEFAULT-NEXT:             write<i32>(%13, mul<i32, overflow=ub>(read<i32>(%4), const<i32>(2)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         write<i32>(%3, add<i32, overflow=ub>(const<i32>(1), read<i32>(%13)));
// DEFAULT-NEXT:         return read<i32>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @operators(%6 p: ptr<@type0>, %7 values: ptr<i32> [array=4]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %8 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %14: i32 [synthetic, unsequenced] = read<i32>(%8);
// DEFAULT-NEXT:         let %15: i32 [synthetic, unsequenced] = read<i32>(%8);
// DEFAULT-NEXT:         let %16: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%15), const<i32>(1));
// DEFAULT-NEXT:         write<i32, unsequenced>(%8, read<i32>(%16));
// DEFAULT-NEXT:         let %17: i32 [synthetic, unsequenced] = add<i32, overflow=ub>(read<i32>(%14), mul<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field0(deref(read<ptr<@type0>>(%6)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%7), read<i32>(%15))))), neg<i32, overflow=ub>(read<i32>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(read<ptr<@type0>>(%6), const<i32>(0))))))));
// DEFAULT-NEXT:         write<i32, unsequenced>(%8, read<i32>(%17));
// DEFAULT-NEXT:         let %12: i32 [synthetic] = read<i32>(%8);
// DEFAULT-NEXT:         write<i32>(%8, conditional<i32>(ne<i32>(read<i32>(%12), const<i32>(0)), read<i32>(%12), from_bool<i32, reason=promotion>(not<bool>(ne<i32>(read<i32>(%8), const<i32>(0))))));
// DEFAULT-NEXT:         read<i32>(%8);
// DEFAULT-NEXT:         let %18: i32 [synthetic, unsequenced] = read<i32>(%8);
// DEFAULT-NEXT:         let %19: i32 [synthetic, unsequenced] = sub<i32, overflow=ub>(read<i32>(%18), const<i32>(1));
// DEFAULT-NEXT:         write<i32, unsequenced>(%8, read<i32>(%19));
// DEFAULT-NEXT:         write<i32, unsequenced>(%8, read<i32>(%19));
// DEFAULT-NEXT:         return conditional<i32>(gt<i32>(read<i32>(%8), const<i32>(0)), read<i32>(%8), not<i32>(read<i32>(%8)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @address_of_label() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 target: ptr<void> [storage=automatic] = label_addr<ptr<void>>(%10);
// DEFAULT-NEXT:         goto *read<ptr<void>>(%11);
// DEFAULT-NEXT:         label %10 done:
// DEFAULT-NEXT:             return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(const<u64>(8), const<u64>(8))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
