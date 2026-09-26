void abort(void);
void exit(int);

typedef enum {
  END   = -1,
  EMPTY = (1 << 8),
  BACKREF,
  BEGLINE,
  ENDLINE,
  BEGWORD,
  ENDWORD,
  LIMWORD,
  NOTLIMWORD,
  QMARK,
  STAR,
  PLUS,
  REPMN,
  CAT,
  OR,
  ORTOP,
  LPAREN,
  RPAREN,
  CSET
} token;

static token tok;

static int atom() {
  if ((tok >= 0 && tok < (1 << 8)) || tok >= CSET || tok == BACKREF ||
      tok == BEGLINE || tok == ENDLINE || tok == BEGWORD || tok == ENDWORD ||
      tok == LIMWORD || tok == NOTLIMWORD)
    return 1;
  else
    return 0;
}

int main(void) {
  tok = 0;
  if (atom() != 1)
    abort();
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
// DEFAULT-NEXT:     type @type0 = enum : i32 {
// DEFAULT-NEXT:         %0 END = const<i32>(-1);
// DEFAULT-NEXT:         %1 EMPTY = const<i32>(256);
// DEFAULT-NEXT:         %2 BACKREF = const<i32>(257);
// DEFAULT-NEXT:         %3 BEGLINE = const<i32>(258);
// DEFAULT-NEXT:         %4 ENDLINE = const<i32>(259);
// DEFAULT-NEXT:         %5 BEGWORD = const<i32>(260);
// DEFAULT-NEXT:         %6 ENDWORD = const<i32>(261);
// DEFAULT-NEXT:         %7 LIMWORD = const<i32>(262);
// DEFAULT-NEXT:         %8 NOTLIMWORD = const<i32>(263);
// DEFAULT-NEXT:         %9 QMARK = const<i32>(264);
// DEFAULT-NEXT:         %10 STAR = const<i32>(265);
// DEFAULT-NEXT:         %11 PLUS = const<i32>(266);
// DEFAULT-NEXT:         %12 REPMN = const<i32>(267);
// DEFAULT-NEXT:         %13 CAT = const<i32>(268);
// DEFAULT-NEXT:         %14 OR = const<i32>(269);
// DEFAULT-NEXT:         %15 ORTOP = const<i32>(270);
// DEFAULT-NEXT:         %16 LPAREN = const<i32>(271);
// DEFAULT-NEXT:         %17 RPAREN = const<i32>(272);
// DEFAULT-NEXT:         %18 CSET = const<i32>(273);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 token = @type0;
// DEFAULT-NEXT:     global %23 tok: @type0 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%26 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %24 @atom() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_and<bool>(ge<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%23)), const<i32>(0)), lt<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%23)), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)))), ge<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%23)), const<i32>(273))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%23)), const<i32>(257))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%23)), const<i32>(258))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%23)), const<i32>(259))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%23)), const<i32>(260))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%23)), const<i32>(261))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%23)), const<i32>(262))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%23)), const<i32>(263)))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<@type0>(%23, int_to_enum<@type0, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%24), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
