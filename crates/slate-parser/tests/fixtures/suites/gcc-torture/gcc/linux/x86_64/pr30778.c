extern void *memset(void *, int, __SIZE_TYPE__);
extern void  abort(void);

struct reg_stat {
  void *last_death;
  void *last_set;
  void *last_set_value;
  int   last_set_label;
  char  last_set_sign_bit_copies;
  int   last_set_mode : 8;
  char  last_set_invalid;
  char  sign_bit_copies;
  long  nonzero_bits;
};

static struct reg_stat *reg_stat;

void __attribute__((noinline)) init_reg_last(void) {
  memset(reg_stat, 0, __builtin_offsetof(struct reg_stat, sign_bit_copies));
}

int main(void) {
  struct reg_stat r;

  reg_stat       = &r;
  r.nonzero_bits = -1;
  init_reg_last();
  if (r.nonzero_bits != -1)
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_reg_stat:[0-9]+]] reg_stat = struct {
// DEFAULT-NEXT:         field0 last_death: ptr<void>;
// DEFAULT-NEXT:         field1 last_set: ptr<void>;
// DEFAULT-NEXT:         field2 last_set_value: ptr<void>;
// DEFAULT-NEXT:         field3 last_set_label: i32;
// DEFAULT-NEXT:         field4 last_set_sign_bit_copies: i8;
// DEFAULT-NEXT:         field5 last_set_mode: i32 : 8;
// DEFAULT-NEXT:         field6 last_set_invalid: i8;
// DEFAULT-NEXT:         field7 sign_bit_copies: i8;
// DEFAULT-NEXT:         field8 nonzero_bits: i64;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24, 28, 29, 30, 31, 32], bit_offsets=[None, None, None, None, None, Some(232), None, None, None], bit_units=[(29, 1)], field_units=[None, None, None, None, None, Some(0), None, None, None]];
// DEFAULT-NEXT:     global %[[VALUE_reg_stat:[0-9]+]] reg_stat: ptr<@type[[TYPE_reg_stat]]> [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_init_reg_last:[0-9]+]] @init_reg_last() -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_reg_stat]]>>(%[[VALUE_reg_stat]])), const<i32>(0), const<u64>(31));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: @type[[TYPE_reg_stat]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_reg_stat]]>>(%[[VALUE_reg_stat]], addr_of<ptr<@type[[TYPE_reg_stat]]>>(%[[VALUE_r]]));
// DEFAULT-NEXT:         write<i64>(field8(%[[VALUE_r]]), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_init_reg_last]]);
// DEFAULT-NEXT:         if ne<i64>(read<i64>(field8(%[[VALUE_r]])), widen<i64, reason=usual_arith>(neg<i32, overflow=ub>(const<i32>(1))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
