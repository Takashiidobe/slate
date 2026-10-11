// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

void work(void);
int next(void);
enum disposition { CONTINUE_SEARCH, EXECUTE_HANDLER };

int effectful_filter(void) {
  int r = 0;
  __try {
    work();
  } __except (r = next()) {
    r = (int)_exception_code();
  }
  return r;
}

int enum_filter(void) {
  __try {
    work();
  } __except (_exception_info() ? EXECUTE_HANDLER : CONTINUE_SEARCH) {
    return 1;
  }
  return 0;
}

int leave_from_inner_finally(void) {
  int r = 0;
  __try {
    __try {
      work();
    } __finally {
      r = _abnormal_termination();
      __leave;
    }
    r = 2;
  } __except (1) {
    r = 3;
  }
  return r;
}

int noop(int x) {
  return __noop(x++, next());
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type[[TYPE_disposition:[0-9]+]] disposition = enum : u32 {
// IR-NEXT:         %[[VALUE_CONTINUE_SEARCH:[0-9]+]] CONTINUE_SEARCH = const<i32>(0);
// IR-NEXT:         %[[VALUE_EXECUTE_HANDLER:[0-9]+]] EXECUTE_HANDLER = const<i32>(1);
// IR-NEXT:     } [size=4, align=4];
// IR-NEXT:     fn %[[VALUE_CONTINUE_SEARCH]] @work() -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_EXECUTE_HANDLER]] @next() -> i32 [linkage=external];
// IR-NEXT:     fn %[[VALUE_effectful_filter:[0-9]+]] @effectful_filter() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         seh_try %[[VALUE0:[0-9]+]]
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     call<void>(%[[VALUE_CONTINUE_SEARCH]]);
// IR-NEXT:                 }
// IR-NEXT:             except {
// IR-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = call<i32>(%[[VALUE_EXECUTE_HANDLER]]);
// IR-NEXT:                 write<i32>(%[[VALUE_r]], read<i32>(%[[VALUE1]]));
// IR-NEXT:                 yield read<i32>(%[[VALUE1]]);
// IR-NEXT:             }:
// IR-NEXT:                 {
// IR-NEXT:                     write<i32>(%[[VALUE_r]], reinterpret<i32>(seh_exception_code<u32>()));
// IR-NEXT:                 }
// IR-NEXT:         return read<i32>(%[[VALUE_r]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_enum_filter:[0-9]+]] @enum_filter() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         seh_try %[[VALUE2:[0-9]+]]
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     call<void>(%[[VALUE_CONTINUE_SEARCH]]);
// IR-NEXT:                 }
// IR-NEXT:             except conditional<i32>(ne<ptr<void>>(seh_exception_info<ptr<void>>(), null<ptr<void>>), const<i32>(1), const<i32>(0)):
// IR-NEXT:                 {
// IR-NEXT:                     return const<i32>(1);
// IR-NEXT:                 }
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_leave_from_inner_finally:[0-9]+]] @leave_from_inner_finally() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: i32 [storage=automatic] = const<i32>(0);
// IR-NEXT:         seh_try %[[VALUE3:[0-9]+]]
// IR-NEXT:             body:
// IR-NEXT:                 {
// IR-NEXT:                     seh_try %[[VALUE4:[0-9]+]]
// IR-NEXT:                         body:
// IR-NEXT:                             {
// IR-NEXT:                                 call<void>(%[[VALUE_CONTINUE_SEARCH]]);
// IR-NEXT:                             }
// IR-NEXT:                         finally:
// IR-NEXT:                             {
// IR-NEXT:                                 write<i32>(%[[VALUE_r_2]], seh_abnormal_termination<i32>());
// IR-NEXT:                                 seh_leave %[[VALUE3]];
// IR-NEXT:                             }
// IR-NEXT:                     write<i32>(%[[VALUE_r_2]], const<i32>(2));
// IR-NEXT:                 }
// IR-NEXT:             except const<i32>(1):
// IR-NEXT:                 {
// IR-NEXT:                     write<i32>(%[[VALUE_r_2]], const<i32>(3));
// IR-NEXT:                 }
// IR-NEXT:         return read<i32>(%[[VALUE_r_2]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_noop:[0-9]+]] @noop(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<i32>(0);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
