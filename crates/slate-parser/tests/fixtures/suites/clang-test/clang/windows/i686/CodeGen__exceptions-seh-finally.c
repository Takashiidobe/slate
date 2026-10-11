// NOTE: we're passing "-O1 -disable-llvm-passes" to avoid adding optnone and noinline everywhere.

void abort(void) __attribute__((noreturn));
void might_crash(void);
void cleanup(void);
int check_condition(void);
void basic_finally(void) {
  __try {
    might_crash();
  } __finally {
    cleanup();
  }
}

//
//


// Mostly check that we don't double emit 'r' which would crash.
void decl_in_finally(void) {
  __try {
    might_crash();
  } __finally {
    int r;
  }
}

// Ditto, don't crash double emitting 'l'.
void label_in_finally(void) {
  __try {
    might_crash();
  } __finally {
l:
    cleanup();
    if (check_condition())
      goto l;
  }
}

//

//

int crashed;
void use_abnormal_termination(void) {
  __try {
    might_crash();
  } __finally {
    crashed = __abnormal_termination();
  }
}

//
//


void noreturn_noop_finally(void) {
  __try {
    __noop();
  } __finally {
    abort();
  }
}



void noreturn_finally(void) {
  __try {
    might_crash();
  } __finally {
    abort();
  }
}

//
//


int finally_with_return(void) {
  __try {
    return 42;
  } __finally {
  }
}


int nested___finally___finally(void) {
  __try {
    __try {
    } __finally {
      return 1;
    }
  } __finally {
    // Intentionally no return here.
  }
  return 0;
}

//
//



// FIXME: Our behavior seems suspiciously different.

int nested___finally___finally_with_eh_edge(void) {
  __try {
    __try {
      might_crash();
    } __finally {
      return 899;
    }
  } __finally {
    // Intentionally no return here.
  }
  return 912;
}
//
// [[invokecont]]
//
//
//
//



void finally_within_finally(void) {
  __try {
    might_crash();
  } __finally {
    __try {
      might_crash();
    } __finally {
    }
  }
}






void cleanup_with_func(const char *);
void finally_with_func(void) {
  __try {
    might_crash();
  } __finally {
    cleanup_with_func(__func__);
  }
}


// Look for the absence of noinline.  nounwind is expected; any further
// attributes should be string attributes.

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "i686-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=4, align=4];
// DEFAULT-NEXT:         stack_alignment = 4;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     global %[[VALUE_crashed:[0-9]+]] crashed: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 18> [storage=static] = code_units<array<i8, 18>>([102, 105, 110, 97, 108, 108, 121, 95, 119, 105, 116, 104, 95, 102, 117, 110, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_might_crash:[0-9]+]] @might_crash() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_cleanup:[0-9]+]] @cleanup() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_check_condition:[0-9]+]] @check_condition() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_basic_finally:[0-9]+]] @basic_finally() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         seh_try %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_might_crash]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_cleanup]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_decl_in_finally:[0-9]+]] @decl_in_finally() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         seh_try %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_might_crash]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_label_in_finally:[0-9]+]] @label_in_finally() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         seh_try %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_might_crash]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     label %[[VALUE_l:[0-9]+]] l:
// DEFAULT-NEXT:                         call<void, signature=fn() -> void>(%[[VALUE_cleanup]]);
// DEFAULT-NEXT:                     if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_check_condition]]), const<i32>(0))
// DEFAULT-NEXT:                         goto %[[VALUE_l]];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_use_abnormal_termination:[0-9]+]] @use_abnormal_termination() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         seh_try %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_might_crash]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_crashed]], seh_abnormal_termination<i32>());
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_noreturn_noop_finally:[0-9]+]] @noreturn_noop_finally() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         seh_try %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_noreturn_finally:[0-9]+]] @noreturn_finally() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         seh_try %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_might_crash]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_finally_with_return:[0-9]+]] @finally_with_return() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         seh_try %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     return const<i32>(42);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested___finally___finally:[0-9]+]] @nested___finally___finally() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         seh_try %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     seh_try %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         finally:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 return const<i32>(1);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested___finally___finally_with_eh_edge:[0-9]+]] @nested___finally___finally_with_eh_edge() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         seh_try %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     seh_try %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_might_crash]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         finally:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 return const<i32>(899);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(912);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_finally_within_finally:[0-9]+]] @finally_within_finally() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         seh_try %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_might_crash]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     seh_try %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_might_crash]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         finally:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_cleanup_with_func:[0-9]+]] @cleanup_with_func(%[[VALUE13:[0-9]+]] <unnamed>: ptr<const i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_finally_with_func:[0-9]+]] @finally_with_func() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         seh_try %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_might_crash]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<const i8>) -> void>(%[[VALUE_cleanup_with_func]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(18)>(%[[VALUE_str]])));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
