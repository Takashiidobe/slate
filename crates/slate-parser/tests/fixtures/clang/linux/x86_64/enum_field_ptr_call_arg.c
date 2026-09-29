typedef enum { REPEAT_NONE, REPEAT_CHECK, REPEAT_VALID } FSE_repeat;

typedef struct {
  FSE_repeat mode;
} Entropy;

static unsigned select_type(FSE_repeat *repeatMode, unsigned count) {
  if (count == 0) {
    *repeatMode = REPEAT_NONE;
    return 0;
  }
  return (unsigned)*repeatMode + count;
}

int run(Entropy *e, unsigned count) {
  return (int)select_type(&e->mode, count);
}

int main(void) {
  Entropy e;
  e.mode = REPEAT_VALID;
  return run(&e, 0) + run(&e, 5);
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_REPEAT_NONE:[0-9]+]] REPEAT_NONE = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_REPEAT_CHECK:[0-9]+]] REPEAT_CHECK = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_REPEAT_VALID:[0-9]+]] REPEAT_VALID = const<i32>(2);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_FSE_repeat:[0-9]+]] FSE_repeat = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 mode: @type[[TYPE0]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Entropy:[0-9]+]] Entropy = @type[[TYPE1]];
// DEFAULT-NEXT:     fn %[[VALUE_select_type:[0-9]+]] @select_type(%[[VALUE_repeatMode:[0-9]+]] repeatMode: ptr<@type[[TYPE0]]>, %[[VALUE_count:[0-9]+]] count: u32) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if eq<u32>(read<u32>(%[[VALUE_count]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<@type[[TYPE0]]>(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_repeatMode]])), int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:                 return reinterpret<u32, reason=return, fits=always>(const<i32>(0));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE0]]>(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_repeatMode]])))), read<u32>(%[[VALUE_count]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_run:[0-9]+]] @run(%[[VALUE_e:[0-9]+]] e: ptr<@type[[TYPE1]]>, %[[VALUE_count_2:[0-9]+]] count: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=explicit, fits=unknown>(call<u32, signature=fn(ptr<@type[[TYPE0]]>, u32) -> u32>(%[[VALUE_select_type]], addr_of<ptr<@type[[TYPE0]]>>(field0(deref(read<ptr<@type[[TYPE1]]>>(%[[VALUE_e]])))), read<u32>(%[[VALUE_count_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_e_2:[0-9]+]] e: @type[[TYPE1]] [storage=automatic];
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(field0(%[[VALUE_e_2]]), int_to_enum<@type[[TYPE0]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(2))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(call<i32, signature=fn(ptr<@type[[TYPE1]]>, u32) -> i32>(%[[VALUE_run]], addr_of<ptr<@type[[TYPE1]]>>(%[[VALUE_e_2]]), reinterpret<u32, reason=arg, fits=always>(const<i32>(0))), call<i32, signature=fn(ptr<@type[[TYPE1]]>, u32) -> i32>(%[[VALUE_run]], addr_of<ptr<@type[[TYPE1]]>>(%[[VALUE_e_2]]), reinterpret<u32, reason=arg, fits=always>(const<i32>(5))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
