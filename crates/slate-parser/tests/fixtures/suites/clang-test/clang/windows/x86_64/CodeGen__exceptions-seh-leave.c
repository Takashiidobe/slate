
void g(void);

//////////////////////////////////////////////////////////////////////////////
// __leave with __except

// Nothing in the __try block can trap, so __try.cont isn't created.
int __leave_with___except_simple(void) {
  int myres = 0;
  __try {
    myres = 15;
    __leave;
    myres = 23;
  } __except (1) {
    return 0;
  }
  return 1;
}


// The "normal" case.
int __leave_with___except(void) {
  int myres = 0;
  __try {
    g();
    __leave;
    myres = 23;
  } __except (1) {
    return 0;
  }
  return 1;
}
// For __excepts, instead of an explicit __try.__leave label, we could use
// use invoke.cont as __leave jump target instead.  However, not doing this
// keeps the CodeGen code simpler, __leave is very rare, and SimplifyCFG will
// simplify this anyways.


//////////////////////////////////////////////////////////////////////////////
// __leave with __finally

void abort(void) __attribute__((noreturn));

// Nothing in the __try block can trap, so __finally.cont and friends aren't
// created.
int __leave_with___finally_simple(void) {
  int myres = 0;
  __try {
    myres = 15;
    __leave;
    myres = 23;
  } __finally {
    return 0;
  }
  return 1;
}

// __finally block doesn't return, __finally.cont doesn't exist.
int __leave_with___finally_noreturn(void) {
  int myres = 0;
  __try {
    myres = 15;
    __leave;
    myres = 23;
  } __finally {
    abort();
  }
  return 1;
}

// The "normal" case.
int __leave_with___finally(void) {
  int myres = 0;
  __try {
    g();
    __leave;
    myres = 23;
  } __finally {
    return 0;
  }
  return 1;
}
// For __finally, there needs to be an explicit __try.__leave, because
// abnormal.termination.slot needs to be set there.


//////////////////////////////////////////////////////////////////////////////
// Mixed, nested cases.

int nested___except___finally(void) {
  int myres = 0;
  __try {
    __try {
      g();
    } __finally {
      g();
      __leave;  // Refers to the outer __try, not the __finally!
      myres = 23;
      return 0;
    }

    myres = 51;
  } __except (1) {
  }
  return 1;
}








int nested___except___except(void) {
  int myres = 0;
  __try {
    __try {
      g();
      myres = 16;
    } __except (1) {
      g();
      __leave;  // Refers to the outer __try, not the __except we're in!
      myres = 23;
      return 0;
    }

    myres = 51;
  } __except (1) {
  }
  return 1;
}
// The order of basic blocks in the below doesn't matter.









int nested___finally___except(void) {
  int myres = 0;
  __try {
    __try {
      g();
    } __except (1) {
      g();
      __leave;  // Refers to the outer __try, not the __except!
      myres = 23;
      return 0;
    }

    myres = 51;
  } __finally {
  }
  return 1;
}
// The order of basic blocks in the below doesn't matter.









int nested___finally___finally(void) {
  int myres = 0;
  __try {
    __try {
      g();
      myres = 16;
    } __finally {
      g();
      __leave;  // Refers to the outer __try, not the __finally we're in!
      myres = 23;
      return 0;
    }

    myres = 51;
  } __finally {
  }
  return 1;
}
// The order of basic blocks in the below doesn't matter.

// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
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
// DEFAULT-NEXT:     fn %[[VALUE_g:[0-9]+]] @g() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___leave_with___except_simple:[0-9]+]] @__leave_with___except_simple() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_myres:[0-9]+]] myres: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         seh_try %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres]], const<i32>(15));
// DEFAULT-NEXT:                     seh_leave %[[VALUE0]];
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres]], const<i32>(23));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             except const<i32>(1):
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___leave_with___except:[0-9]+]] @__leave_with___except() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_myres_2:[0-9]+]] myres: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         seh_try %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                     seh_leave %[[VALUE1]];
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres_2]], const<i32>(23));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             except const<i32>(1):
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___leave_with___finally_simple:[0-9]+]] @__leave_with___finally_simple() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_myres_3:[0-9]+]] myres: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         seh_try %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres_3]], const<i32>(15));
// DEFAULT-NEXT:                     seh_leave %[[VALUE2]];
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres_3]], const<i32>(23));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___leave_with___finally_noreturn:[0-9]+]] @__leave_with___finally_noreturn() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_myres_4:[0-9]+]] myres: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         seh_try %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres_4]], const<i32>(15));
// DEFAULT-NEXT:                     seh_leave %[[VALUE3]];
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres_4]], const<i32>(23));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___leave_with___finally:[0-9]+]] @__leave_with___finally() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_myres_5:[0-9]+]] myres: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         seh_try %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                     seh_leave %[[VALUE4]];
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres_5]], const<i32>(23));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     return const<i32>(0);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested___except___finally:[0-9]+]] @nested___except___finally() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_myres_6:[0-9]+]] myres: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         seh_try %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     seh_try %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         finally:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                                 seh_leave %[[VALUE5]];
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_myres_6]], const<i32>(23));
// DEFAULT-NEXT:                                 return const<i32>(0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres_6]], const<i32>(51));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             except const<i32>(1):
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested___except___except:[0-9]+]] @nested___except___except() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_myres_7:[0-9]+]] myres: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         seh_try %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     seh_try %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_myres_7]], const<i32>(16));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         except const<i32>(1):
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                                 seh_leave %[[VALUE7]];
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_myres_7]], const<i32>(23));
// DEFAULT-NEXT:                                 return const<i32>(0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres_7]], const<i32>(51));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             except const<i32>(1):
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested___finally___except:[0-9]+]] @nested___finally___except() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_myres_8:[0-9]+]] myres: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         seh_try %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     seh_try %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         except const<i32>(1):
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                                 seh_leave %[[VALUE9]];
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_myres_8]], const<i32>(23));
// DEFAULT-NEXT:                                 return const<i32>(0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres_8]], const<i32>(51));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested___finally___finally:[0-9]+]] @nested___finally___finally() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_myres_9:[0-9]+]] myres: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         seh_try %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     seh_try %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_myres_9]], const<i32>(16));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         finally:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_g]]);
// DEFAULT-NEXT:                                 seh_leave %[[VALUE11]];
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_myres_9]], const<i32>(23));
// DEFAULT-NEXT:                                 return const<i32>(0);
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_myres_9]], const<i32>(51));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
