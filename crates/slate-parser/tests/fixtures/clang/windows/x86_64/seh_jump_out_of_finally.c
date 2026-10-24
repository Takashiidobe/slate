// SLATE-FILECHECK-DEFINES C17
// SLATE-FILECHECK-STD C17 c17
// SLATE-FILECHECK-WARNING C17

void continue_out(void) {
  while (1) {
    __try {
    } __finally {
      continue;
    }
  }
}

void continue_inside(void) {
  __try {
  } __finally {
    while (1) {
      continue;
    }
  }
}

void break_out(int x) {
  while (1) {
    __try {
    } __finally {
      break;
    }
  }
  switch (x) {
  case 1:
    __try {
    } __finally {
      break;
    }
  }
}

void break_inside(void) {
  __try {
  } __finally {
    while (1) {
      break;
    }
  }
}

void leave_out(void) {
  __try {
    __try {
    } __finally {
      __leave;
    }
  } __finally {
  }
}

void leave_inside(void) {
  __try {
  } __finally {
    __try {
      __leave;
    } __finally {
    }
  }
}

void return_out(void) {
  __try {
  } __finally {
    return;
  }
}

int return_value_out(void) {
  __try {
  } __finally {
    return 1;
  }
  return 0;
}

void goto_out(void) {
  __try {
  } __finally {
    goto done;
  }
done:;
}

void goto_inside(void) {
  __try {
  } __finally {
    goto inner;
  inner:;
  }
}

void goto_out_of_try_body(void) {
  __try {
    goto done;
  } __finally {
  }
done:;
}

// SLATE-FILECHECK-BEGIN C17
// C17: -Wjump-seh-finally
// C17:   ⚠ jump out of __finally block has undefined behavior
// C17:    ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally.c:6:7]
// C17:  5 │     } __finally {
// C17:  6 │       continue;
// C17:    ·       ─────────
// C17:  7 │     }
// C17:    ╰────
// C17: -Wjump-seh-finally
// C17:   ⚠ jump out of __finally block has undefined behavior
// C17:     ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally.c:24:7]
// C17:  23 │     } __finally {
// C17:  24 │       break;
// C17:     ·       ──────
// C17:  25 │     }
// C17:     ╰────
// C17: -Wjump-seh-finally
// C17:   ⚠ jump out of __finally block has undefined behavior
// C17:     ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally.c:31:7]
// C17:  30 │     } __finally {
// C17:  31 │       break;
// C17:     ·       ──────
// C17:  32 │     }
// C17:     ╰────
// C17: -Wjump-seh-finally
// C17:   ⚠ jump out of __finally block has undefined behavior
// C17:     ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally.c:49:7]
// C17:  48 │     } __finally {
// C17:  49 │       __leave;
// C17:     ·       ────────
// C17:  50 │     }
// C17:     ╰────
// C17: -Wjump-seh-finally
// C17:   ⚠ jump out of __finally block has undefined behavior
// C17:     ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally.c:68:5]
// C17:  67 │   } __finally {
// C17:  68 │     return;
// C17:     ·     ───────
// C17:  69 │   }
// C17:     ╰────
// C17: -Wjump-seh-finally
// C17:   ⚠ jump out of __finally block has undefined behavior
// C17:     ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally.c:75:5]
// C17:  74 │   } __finally {
// C17:  75 │     return 1;
// C17:     ·     ─────────
// C17:  76 │   }
// C17:     ╰────
// C17: -Wjump-seh-finally
// C17:   ⚠ jump out of __finally block has undefined behavior
// C17:     ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally.c:83:5]
// C17:  82 │   } __finally {
// C17:  83 │     goto done;
// C17:     ·     ──────────
// C17:  84 │   }
// C17:     ╰────
// SLATE-FILECHECK-END C17
// SLATE-FILECHECK-BEGIN IR-C17
// IR-C17: module {
// IR-C17-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-C17-NEXT:         endian = little;
// IR-C17-NEXT:         pointer [size=8, align=8];
// IR-C17-NEXT:         stack_alignment = 16;
// IR-C17-NEXT:         long_double = f64;
// IR-C17-NEXT:         storage bool [size=1, align=1];
// IR-C17-NEXT:         storage i8, u8 [size=1, align=1];
// IR-C17-NEXT:         storage i16, u16 [size=2, align=2];
// IR-C17-NEXT:         storage i32, u32 [size=4, align=4];
// IR-C17-NEXT:         storage i64, u64 [size=8, align=8];
// IR-C17-NEXT:         storage i128, u128 [size=16, align=16];
// IR-C17-NEXT:         storage bf16 [size=2, align=2];
// IR-C17-NEXT:         storage f16 [size=2, align=2];
// IR-C17-NEXT:         storage f32 [size=4, align=4];
// IR-C17-NEXT:         storage f64 [size=8, align=8];
// IR-C17-NEXT:         storage f128 [size=16, align=16];
// IR-C17-NEXT:         storage d32 [size=4, align=4];
// IR-C17-NEXT:         storage d64 [size=8, align=8];
// IR-C17-NEXT:         storage d128 [size=16, align=16];
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_continue_out:[0-9]+]] @continue_out() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:         while %[[VALUE0:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// IR-C17-NEXT:             {
// IR-C17-NEXT:                 seh_try %[[VALUE1:[0-9]+]]
// IR-C17-NEXT:                     body:
// IR-C17-NEXT:                         {
// IR-C17-NEXT:                         }
// IR-C17-NEXT:                     finally:
// IR-C17-NEXT:                         {
// IR-C17-NEXT:                             continue %[[VALUE0]];
// IR-C17-NEXT:                         }
// IR-C17-NEXT:             }
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_continue_inside:[0-9]+]] @continue_inside() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:         seh_try %[[VALUE2:[0-9]+]]
// IR-C17-NEXT:             body:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                 }
// IR-C17-NEXT:             finally:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                     while %[[VALUE3:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// IR-C17-NEXT:                         {
// IR-C17-NEXT:                             continue %[[VALUE3]];
// IR-C17-NEXT:                         }
// IR-C17-NEXT:                 }
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_break_out:[0-9]+]] @break_out(%[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:         while %[[VALUE4:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// IR-C17-NEXT:             {
// IR-C17-NEXT:                 seh_try %[[VALUE5:[0-9]+]]
// IR-C17-NEXT:                     body:
// IR-C17-NEXT:                         {
// IR-C17-NEXT:                         }
// IR-C17-NEXT:                     finally:
// IR-C17-NEXT:                         {
// IR-C17-NEXT:                             break %[[VALUE4]];
// IR-C17-NEXT:                         }
// IR-C17-NEXT:             }
// IR-C17-NEXT:         switch %[[VALUE6:[0-9]+]] read<i32>(%[[VALUE_x]])
// IR-C17-NEXT:             {
// IR-C17-NEXT:                 case %[[VALUE6]] const<i32>(1):
// IR-C17-NEXT:                     seh_try %[[VALUE7:[0-9]+]]
// IR-C17-NEXT:                         body:
// IR-C17-NEXT:                             {
// IR-C17-NEXT:                             }
// IR-C17-NEXT:                         finally:
// IR-C17-NEXT:                             {
// IR-C17-NEXT:                                 break %[[VALUE6]];
// IR-C17-NEXT:                             }
// IR-C17-NEXT:             }
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_break_inside:[0-9]+]] @break_inside() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:         seh_try %[[VALUE8:[0-9]+]]
// IR-C17-NEXT:             body:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                 }
// IR-C17-NEXT:             finally:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                     while %[[VALUE9:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// IR-C17-NEXT:                         {
// IR-C17-NEXT:                             break %[[VALUE9]];
// IR-C17-NEXT:                         }
// IR-C17-NEXT:                 }
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_leave_out:[0-9]+]] @leave_out() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:         seh_try %[[VALUE10:[0-9]+]]
// IR-C17-NEXT:             body:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                     seh_try %[[VALUE11:[0-9]+]]
// IR-C17-NEXT:                         body:
// IR-C17-NEXT:                             {
// IR-C17-NEXT:                             }
// IR-C17-NEXT:                         finally:
// IR-C17-NEXT:                             {
// IR-C17-NEXT:                                 seh_leave %[[VALUE10]];
// IR-C17-NEXT:                             }
// IR-C17-NEXT:                 }
// IR-C17-NEXT:             finally:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                 }
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_leave_inside:[0-9]+]] @leave_inside() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:         seh_try %[[VALUE12:[0-9]+]]
// IR-C17-NEXT:             body:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                 }
// IR-C17-NEXT:             finally:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                     seh_try %[[VALUE13:[0-9]+]]
// IR-C17-NEXT:                         body:
// IR-C17-NEXT:                             {
// IR-C17-NEXT:                                 seh_leave %[[VALUE13]];
// IR-C17-NEXT:                             }
// IR-C17-NEXT:                         finally:
// IR-C17-NEXT:                             {
// IR-C17-NEXT:                             }
// IR-C17-NEXT:                 }
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_return_out:[0-9]+]] @return_out() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:         seh_try %[[VALUE14:[0-9]+]]
// IR-C17-NEXT:             body:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                 }
// IR-C17-NEXT:             finally:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                     return;
// IR-C17-NEXT:                 }
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_return_value_out:[0-9]+]] @return_value_out() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-C17-NEXT:         seh_try %[[VALUE15:[0-9]+]]
// IR-C17-NEXT:             body:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                 }
// IR-C17-NEXT:             finally:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                     return const<i32>(1);
// IR-C17-NEXT:                 }
// IR-C17-NEXT:         return const<i32>(0);
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_goto_out:[0-9]+]] @goto_out() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:         seh_try %[[VALUE16:[0-9]+]]
// IR-C17-NEXT:             body:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                 }
// IR-C17-NEXT:             finally:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                     goto %[[VALUE_done:[0-9]+]];
// IR-C17-NEXT:                 }
// IR-C17-NEXT:         label %[[VALUE_done]] done:
// IR-C17-NEXT:             ;
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_goto_inside:[0-9]+]] @goto_inside() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:         seh_try %[[VALUE17:[0-9]+]]
// IR-C17-NEXT:             body:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                 }
// IR-C17-NEXT:             finally:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                     goto %[[VALUE_inner:[0-9]+]];
// IR-C17-NEXT:                     label %[[VALUE_inner]] inner:
// IR-C17-NEXT:                         ;
// IR-C17-NEXT:                 }
// IR-C17-NEXT:     }
// IR-C17-NEXT:     fn %[[VALUE_goto_out_of_try_body:[0-9]+]] @goto_out_of_try_body() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C17-NEXT:         seh_try %[[VALUE18:[0-9]+]]
// IR-C17-NEXT:             body:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                     goto %[[VALUE_done_2:[0-9]+]];
// IR-C17-NEXT:                 }
// IR-C17-NEXT:             finally:
// IR-C17-NEXT:                 {
// IR-C17-NEXT:                 }
// IR-C17-NEXT:         label %[[VALUE_done_2]] done:
// IR-C17-NEXT:             ;
// IR-C17-NEXT:     }
// IR-C17-NEXT: }
// SLATE-FILECHECK-END IR-C17
