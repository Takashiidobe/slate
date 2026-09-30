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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = enum : i32 {
// DEFAULT-NEXT:         %[[VALUE_END:[0-9]+]] END = const<i32>(-1);
// DEFAULT-NEXT:         %[[VALUE_EMPTY:[0-9]+]] EMPTY = const<i32>(256);
// DEFAULT-NEXT:         %[[VALUE_BACKREF:[0-9]+]] BACKREF = const<i32>(257);
// DEFAULT-NEXT:         %[[VALUE_BEGLINE:[0-9]+]] BEGLINE = const<i32>(258);
// DEFAULT-NEXT:         %[[VALUE_ENDLINE:[0-9]+]] ENDLINE = const<i32>(259);
// DEFAULT-NEXT:         %[[VALUE_BEGWORD:[0-9]+]] BEGWORD = const<i32>(260);
// DEFAULT-NEXT:         %[[VALUE_ENDWORD:[0-9]+]] ENDWORD = const<i32>(261);
// DEFAULT-NEXT:         %[[VALUE_LIMWORD:[0-9]+]] LIMWORD = const<i32>(262);
// DEFAULT-NEXT:         %[[VALUE_NOTLIMWORD:[0-9]+]] NOTLIMWORD = const<i32>(263);
// DEFAULT-NEXT:         %[[VALUE_QMARK:[0-9]+]] QMARK = const<i32>(264);
// DEFAULT-NEXT:         %[[VALUE_STAR:[0-9]+]] STAR = const<i32>(265);
// DEFAULT-NEXT:         %[[VALUE_PLUS:[0-9]+]] PLUS = const<i32>(266);
// DEFAULT-NEXT:         %[[VALUE_REPMN:[0-9]+]] REPMN = const<i32>(267);
// DEFAULT-NEXT:         %[[VALUE_CAT:[0-9]+]] CAT = const<i32>(268);
// DEFAULT-NEXT:         %[[VALUE_OR:[0-9]+]] OR = const<i32>(269);
// DEFAULT-NEXT:         %[[VALUE_ORTOP:[0-9]+]] ORTOP = const<i32>(270);
// DEFAULT-NEXT:         %[[VALUE_LPAREN:[0-9]+]] LPAREN = const<i32>(271);
// DEFAULT-NEXT:         %[[VALUE_RPAREN:[0-9]+]] RPAREN = const<i32>(272);
// DEFAULT-NEXT:         %[[VALUE_CSET:[0-9]+]] CSET = const<i32>(273);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_token:[0-9]+]] token = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_tok:[0-9]+]] tok: @type[[TYPE0]] [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_END]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_EMPTY]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_atom:[0-9]+]] @atom() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_or<bool>(logical_and<bool>(ge<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_tok]])), const<i32>(0)), lt<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_tok]])), shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(8)))), ge<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_tok]])), const<i32>(273))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_tok]])), const<i32>(257))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_tok]])), const<i32>(258))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_tok]])), const<i32>(259))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_tok]])), const<i32>(260))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_tok]])), const<i32>(261))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_tok]])), const<i32>(262))), eq<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE0]]>(%[[VALUE_tok]])), const<i32>(263)))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_tok]], int_to_enum<@type[[TYPE0]], reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_atom]]), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_END]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_EMPTY]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
