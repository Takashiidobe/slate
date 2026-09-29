/** selects an operating mode */
enum Mode {
  /** disable processing */
  MODE_OFF = 0,
  /// enable processing
  MODE_ON  = 1,
};

/** stores a selected mode */
struct Holder {
  /** current mode value */
  enum Mode mode;
};

/** names holder records */
typedef struct Holder Holder;

/** counts completed operations */
static int completed_count = 1;

/** increments a value and records the operation */
static int increment(int value) {
  /** stores the intermediate result */
  volatile int next = value + 1;
  completed_count++;
  return next;
}

int main(void) {
  struct Holder holder = {MODE_ON};
  return holder.mode == MODE_ON && increment(1) == 2 && completed_count == 2
             ? 0
             : 1;
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
// DEFAULT-NEXT:     type @type[[TYPE_Mode:[0-9]+]] Mode = enum : u32 {
// DEFAULT-NEXT:         %[[VALUE_MODE_OFF:[0-9]+]] MODE_OFF = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_MODE_ON:[0-9]+]] MODE_ON = const<i32>(1);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_Holder:[0-9]+]] Holder = struct {
// DEFAULT-NEXT:         field0 mode: @type[[TYPE_Mode]];
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_Holder_2:[0-9]+]] Holder = @type[[TYPE_Holder]];
// DEFAULT-NEXT:     global %[[VALUE_completed_count:[0-9]+]] completed_count: i32 [storage=static] = const<i32>(1) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_increment:[0-9]+]] @increment(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_next:[0-9]+]] next: volatile i32 [storage=automatic] = add<i32, overflow=ub>(read<i32>(%[[VALUE_value]]), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_completed_count]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_completed_count]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         return read<i32, volatile>(%[[VALUE_next]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_holder:[0-9]+]] holder: @type[[TYPE_Holder]] [storage=automatic] = aggregate<@type[[TYPE_Holder]], zero_fill=false>(field0 = int_to_enum<@type[[TYPE_Mode]], reason=assign>(reinterpret<u32, reason=assign, fits=always>(const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if eq<u32>(enum_to_int<u32, reason=promotion>(read<@type[[TYPE_Mode]]>(field0(%[[VALUE_holder]]))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)))
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], eq<i32>(call<i32, signature=fn(i32) -> i32>(%[[VALUE_increment]], const<i32>(1)), const<i32>(2)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE2]], const<bool>(false));
// DEFAULT-NEXT:         return conditional<i32>(logical_and<bool>(read<bool>(%[[VALUE2]]), eq<i32>(read<i32>(%[[VALUE_completed_count]]), const<i32>(2))), const<i32>(0), const<i32>(1));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
