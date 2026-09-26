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
// DEFAULT-NEXT:     type @type0 COUNT = i16;
// DEFAULT-NEXT:     type @type1 TEXT = i8;
// DEFAULT-NEXT:     type @type2 T_VALS = union {
// DEFAULT-NEXT:         field0 id: ptr<i8>;
// DEFAULT-NEXT:     } [size=8, align=2, offsets=[0]];
// DEFAULT-NEXT:     type @type3 VALS = @type2;
// DEFAULT-NEXT:     type @type4 T_VAL = struct {
// DEFAULT-NEXT:         field0 pos: i16;
// DEFAULT-NEXT:         field1 vals: @type2;
// DEFAULT-NEXT:     } [size=10, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type5 VAL = @type4;
// DEFAULT-NEXT:     type @type6 WORD = u16;
// DEFAULT-NEXT:     global %1 id_space: array<array<i8, 33>, 2> [storage=static] [align=16] [linkage=internal];
// DEFAULT-NEXT:     global %8 curval: @type4 [storage=static] = aggregate<@type4, zero_fill=true>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(0))) [linkage=external];
// DEFAULT-NEXT:     global %9 idc: i16 [storage=static] = truncate<i16, reason=assign, fits=always>(const<i32>(0)) [linkage=internal];
// DEFAULT-NEXT:     global %10 cur_line: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     global %11 char_pos: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @exit(%18 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %13 @get_id(%14 c: i8) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field0(field1(%8))), const<i32>(0))), read<i8>(%14));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @get_tok() -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 c: i8 [storage=automatic] = truncate<i8, reason=assign, fits=always>(const<i32>(99));
// DEFAULT-NEXT:         write<ptr<i8>>(field0(field1(%8)), array_decay<ptr<i8>, length=Some(33)>(deref(ptr_offset<ptr<array<i8, 33>>, subtract=false, element=array<i8, 33>, overflow=ub>(array_decay<ptr<array<i8, 33>>, length=Some(2)>(%1), widen<i32, reason=promotion>(read<i16>(%9))))));
// DEFAULT-NEXT:         write<i16>(field0(%8), truncate<i16, reason=assign, fits=unknown>(or<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%10), const<i32>(10)), read<i32>(%11))));
// DEFAULT-NEXT:         return call<u16, signature=fn(i8) -> u16>(%13, read<i8>(%16));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<u16, signature=fn() -> u16>(%15);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%0, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
