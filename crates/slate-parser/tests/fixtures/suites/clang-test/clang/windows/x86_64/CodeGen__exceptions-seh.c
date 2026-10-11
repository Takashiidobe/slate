
void try_body(int numerator, int denominator, int *myres) {
  *myres = numerator / denominator;
}

int safe_div(int numerator, int denominator, int *res) {
  int myres = 0;
  int success = 1;
  __try {
    try_body(numerator, denominator, &myres);
  } __except (1) {
    success = -42;
  }
  *res = myres;
  return success;
}

//
//
//

// 32-bit SEH needs this filter to save the exception code.
//

// Mingw uses msvcrt, so it can also use _except_handler3.

void j(void);

int filter_expr_capture(void) {
  int r = 42;
  __try {
    j();
  } __except(r = -1) {
    r = 13;
  }
  return r;
}

//
//

//
//
//
//

int nested_try(void) {
  int r = 42;
  __try {
    __try {
      j();
      r = 0;
    } __except(_exception_code() == 123) {
      r = 123;
    }
  } __except(_exception_code() == 456) {
    r = 456;
  }
  return r;
}
//
//
//
//
//
//
//
//
//
//
//

int basic_finally(int g) {
  __try {
    j();
  } __finally {
    ++g;
  }
  return g;
}
//
//
//


int returns_int(void);
int except_return(void) {
  __try {
    return returns_int();
  } __except(1) {
    return 42;
  }
}
//
//
//


// PR 24751: don't assert if a variable is used twice in a __finally block.
// Also, make sure we don't do redundant work to capture/project it.
void finally_capture_twice(int x) {
  __try {
  } __finally {
    int y = x;
    int z = x;
  }
}
//

int exception_code_in_except(void) {
  __try {
    try_body(0, 0, 0);
  } __except(1) {
    return _exception_code();
  }
  return 0;
}

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
// DEFAULT-NEXT:     fn %[[VALUE_try_body:[0-9]+]] @try_body(%[[VALUE_numerator:[0-9]+]] numerator: i32, %[[VALUE_denominator:[0-9]+]] denominator: i32, %[[VALUE_myres:[0-9]+]] myres: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_myres]])), div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE_numerator]]), read<i32>(%[[VALUE_denominator]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_safe_div:[0-9]+]] @safe_div(%[[VALUE_numerator_2:[0-9]+]] numerator: i32, %[[VALUE_denominator_2:[0-9]+]] denominator: i32, %[[VALUE_res:[0-9]+]] res: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_myres_2:[0-9]+]] myres: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_success:[0-9]+]] success: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         seh_try %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, ptr<i32>) -> void>(%[[VALUE_try_body]], read<i32>(%[[VALUE_numerator_2]]), read<i32>(%[[VALUE_denominator_2]]), addr_of<ptr<i32>>(%[[VALUE_myres_2]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             except const<i32>(1):
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_success]], neg<i32, overflow=ub>(const<i32>(42)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_res]])), read<i32>(%[[VALUE_myres_2]]));
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_success]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_j:[0-9]+]] @j() -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_filter_expr_capture:[0-9]+]] @filter_expr_capture() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i32 [storage=automatic] = const<i32>(42);
// DEFAULT-NEXT:         seh_try %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             except {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_r]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield read<i32>(%[[VALUE2]]);
// DEFAULT-NEXT:             }:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_r]], const<i32>(13));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_nested_try:[0-9]+]] @nested_try() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: i32 [storage=automatic] = const<i32>(42);
// DEFAULT-NEXT:         seh_try %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     seh_try %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 call<void, signature=fn() -> void>(%[[VALUE_j]]);
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_r_2]], const<i32>(0));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                         except eq<u32>(seh_exception_code<u32>(), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(123))):
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_r_2]], const<i32>(123));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             except eq<u32>(seh_exception_code<u32>(), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(456))):
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_r_2]], const<i32>(456));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_r_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_basic_finally:[0-9]+]] @basic_finally(%[[VALUE_g:[0-9]+]] g: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         seh_try %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_j]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_g]]);
// DEFAULT-NEXT:                     let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                     write<i32>(%[[VALUE_g]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_g]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_returns_int:[0-9]+]] @returns_int() -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_except_return:[0-9]+]] @except_return() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         seh_try %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     return call<i32, signature=fn() -> i32>(%[[VALUE_returns_int]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             except const<i32>(1):
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     return const<i32>(42);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_finally_capture_twice:[0-9]+]] @finally_capture_twice(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         seh_try %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             finally:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                     let %[[VALUE_z:[0-9]+]] z: i32 [storage=automatic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_exception_code_in_except:[0-9]+]] @exception_code_in_except() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         seh_try %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<void, signature=fn(i32, i32, ptr<i32>) -> void>(%[[VALUE_try_body]], const<i32>(0), const<i32>(0), null<ptr<i32>>);
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             except const<i32>(1):
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     return reinterpret<i32, reason=return, fits=unknown>(seh_exception_code<u32>());
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
