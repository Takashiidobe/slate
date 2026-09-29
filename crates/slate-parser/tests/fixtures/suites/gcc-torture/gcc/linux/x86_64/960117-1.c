void exit(int);

static char   id_space[2][32 + 1];
typedef short COUNT;

typedef char TEXT;

union T_VALS {
  TEXT *id __attribute__((aligned(2), packed));
};
typedef union T_VALS VALS;

struct T_VAL {
  COUNT pos __attribute__((aligned(2), packed));
  VALS  vals __attribute__((aligned(2), packed));
};
typedef struct T_VAL VAL;

VAL curval = {0};

static short idc = 0;
static int   cur_line;
static int   char_pos;

typedef unsigned short WORD;

WORD get_id(char c) { curval.vals.id[0] = c; }

WORD get_tok() {
  char c         = 'c';
  curval.vals.id = id_space[idc];
  curval.pos     = (cur_line << 10) | char_pos;
  return get_id(c);
}

int main(void) {
  get_tok();
  exit(0);
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
// DEFAULT-NEXT:     type @type[[TYPE_COUNT:[0-9]+]] COUNT = i16;
// DEFAULT-NEXT:     type @type[[TYPE_TEXT:[0-9]+]] TEXT = i8;
// DEFAULT-NEXT:     type @type[[TYPE_T_VALS:[0-9]+]] T_VALS = union {
// DEFAULT-NEXT:         field0 id: ptr<i8>;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_VALS:[0-9]+]] VALS = @type[[TYPE_T_VALS]];
// DEFAULT-NEXT:     type @type[[TYPE_T_VAL:[0-9]+]] T_VAL = struct {
// DEFAULT-NEXT:         field0 pos: i16;
// DEFAULT-NEXT:         field1 vals: @type[[TYPE_T_VALS]];
// DEFAULT-NEXT:     } [size=10, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_VAL:[0-9]+]] VAL = @type[[TYPE_T_VAL]];
// DEFAULT-NEXT:     type @type[[TYPE_WORD:[0-9]+]] WORD = u16;
// DEFAULT-NEXT:     global %[[VALUE_id_space:[0-9]+]] id_space: array<array<i8, 33>, 2> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_curval:[0-9]+]] curval: @type[[TYPE_T_VAL]] [storage=static] = aggregate<@type[[TYPE_T_VAL]], zero_fill=true>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_idc:[0-9]+]] idc: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_cur_line:[0-9]+]] cur_line: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_char_pos:[0-9]+]] char_pos: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_get_id:[0-9]+]] @get_id(%[[VALUE_c:[0-9]+]] c: i8) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field0(field1(%[[VALUE_curval]]))), const<i32>(0))), read<i8>(%[[VALUE_c]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_get_tok:[0-9]+]] @get_tok() -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_c_2:[0-9]+]] c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(99));
// DEFAULT-NEXT:         write<ptr<i8>>(field0(field1(%[[VALUE_curval]])), array_decay<ptr<i8>, length=Some(33)>(deref(ptr_offset<ptr<array<i8, 33>>, subtract=false, element=array<i8, 33>, overflow=ub>(array_decay<ptr<array<i8, 33>>, length=Some(2)>(%[[VALUE_id_space]]), widen<i32, reason=promotion>(read<i16>(%[[VALUE_idc]]))))));
// DEFAULT-NEXT:         write<i16>(field0(%[[VALUE_curval]]), truncate<i16, reason=assign, fits=unknown>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE_cur_line]]), const<i32>(10)), read<i32>(%[[VALUE_char_pos]]))));
// DEFAULT-NEXT:         return call<u16, signature=fn(i8) -> u16>(%[[VALUE_get_id]], read<i8>(%[[VALUE_c_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<u16, signature=fn() -> u16>(%[[VALUE_get_tok]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
