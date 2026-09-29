void abort(void);
void exit(int);

typedef struct {
  short  s __attribute__((aligned(2), packed));
  double d __attribute__((aligned(2), packed));
} TRIAL;

int check(TRIAL *t) {
  if (t->s != 1 || t->d != 16.0)
    return 1;
  return 0;
}

int main(void) {
  TRIAL trial;

  trial.s = 1;
  trial.d = 16.0;

  if (check(&trial) != 0)
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 s: i16;
// DEFAULT-NEXT:         field1 d: f64;
// DEFAULT-NEXT:     } [size=10, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE_TRIAL:[0-9]+]] TRIAL = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_check:[0-9]+]] @check(%[[VALUE_t:[0-9]+]] t: ptr<@type[[TYPE0]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]]))))), const<i32>(1)), ne<f64, exceptions=ignore>(read<f64>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_t]])))), const<f64>(16.0)))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_trial:[0-9]+]] trial: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<i16>(field0(%[[VALUE_trial]]), truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<f64>(field1(%[[VALUE_trial]]), const<f64>(16.0));
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE0]]>) -> i32>(%[[VALUE_check]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_trial]])), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
