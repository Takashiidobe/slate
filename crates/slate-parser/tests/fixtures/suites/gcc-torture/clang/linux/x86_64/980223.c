void abort(void);

typedef struct {
  char *addr;
  long  type;
} object;

object bar(object blah) { abort(); }

object foo(object x, object y) {
  object z = *(object *)(x.addr);
  if (z.type & 64) {
    y = *(object *)(z.addr + sizeof(object));
    z = *(object *)(z.addr);
    if (z.type & 64)
      y = bar(y);
  }
  return y;
}

int    nil;
object cons1[2] = {{(char *)&nil, 0}, {(char *)&nil, 0}};
object cons2[2] = {{(char *)&cons1, 64}, {(char *)&nil, 0}};

int main(void) {
  object x     = {(char *)&cons2, 64};
  object y     = {(char *)&nil, 0};
  object three = foo(x, y);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 addr: ptr<i8>;
// DEFAULT-NEXT:         field1 type: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_object:[0-9]+]] object = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_nil:[0-9]+]] nil: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cons1:[0-9]+]] cons1: array<@type[[TYPE0]], 2> [storage=static] [align=16] = aggregate<array<@type[[TYPE0]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_nil]])), field1 = widen<i64, reason=assign>(const<i32>(0))), index1 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_nil]])), field1 = widen<i64, reason=assign>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_cons2:[0-9]+]] cons2: array<@type[[TYPE0]], 2> [storage=static] [align=16] = aggregate<array<@type[[TYPE0]], 2>, zero_fill=false>(index0 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<array<@type[[TYPE0]], 2>>>(%[[VALUE_cons1]])), field1 = widen<i64, reason=assign>(const<i32>(64))), index1 = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_nil]])), field1 = widen<i64, reason=assign>(const<i32>(0)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_blah:[0-9]+]] blah: @type[[TYPE0]]) -> @type[[TYPE0]] [linkage=external] [abi=sysv64(native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: @type[[TYPE0]], %[[VALUE_y:[0-9]+]] y: @type[[TYPE0]]) -> @type[[TYPE0]] [linkage=external] [abi=sysv64(native_c, native_c) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: @type[[TYPE0]] [storage=automatic] = copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(deref(pointer_cast<ptr<@type[[TYPE0]]>, reason=explicit>(read<ptr<i8>>(field0(%[[VALUE_x]]))))));
// DEFAULT-NEXT:         if ne<i64>(and<i64>(read<i64>(field1(%[[VALUE_z]])), widen<i64, reason=usual_arith>(const<i32>(64))), const<i64>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<@type[[TYPE0]]>(%[[VALUE_y]], copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(deref(pointer_cast<ptr<@type[[TYPE0]]>, reason=explicit>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field0(%[[VALUE_z]])), const<u64>(16)))))));
// DEFAULT-NEXT:                 write<@type[[TYPE0]]>(%[[VALUE_z]], copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(deref(pointer_cast<ptr<@type[[TYPE0]]>, reason=explicit>(read<ptr<i8>>(field0(%[[VALUE_z]])))))));
// DEFAULT-NEXT:                 if ne<i64>(and<i64>(read<i64>(field1(%[[VALUE_z]])), widen<i64, reason=usual_arith>(const<i32>(64))), const<i64>(0))
// DEFAULT-NEXT:                     write<@type[[TYPE0]]>(%[[VALUE_y]], copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(@type[[TYPE0]]) -> @type[[TYPE0]], abi=sysv64(native_c) -> native_c>(%[[VALUE_bar]], copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_y]])))));
// DEFAULT-NEXT:                     copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(@type[[TYPE0]]) -> @type[[TYPE0]], abi=sysv64(native_c) -> native_c>(%[[VALUE_bar]], copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_y]]))));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return copy<@type[[TYPE0]], reason=return>(read<@type[[TYPE0]]>(%[[VALUE_y]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_x_2:[0-9]+]] x: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<array<@type[[TYPE0]], 2>>>(%[[VALUE_cons2]])), field1 = widen<i64, reason=assign>(const<i32>(64)));
// DEFAULT-NEXT:         let %[[VALUE_y_2:[0-9]+]] y: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%[[VALUE_nil]])), field1 = widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_three:[0-9]+]] three: @type[[TYPE0]] [storage=automatic] = copy<@type[[TYPE0]], reason=assign>(call<@type[[TYPE0]], signature=fn(@type[[TYPE0]], @type[[TYPE0]]) -> @type[[TYPE0]], abi=sysv64(native_c, native_c) -> native_c>(%[[VALUE_foo]], copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_x_2]])), copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_y_2]]))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
