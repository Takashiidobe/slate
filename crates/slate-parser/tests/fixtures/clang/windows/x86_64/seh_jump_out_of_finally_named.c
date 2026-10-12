// SLATE-FILECHECK-DEFINES C2Y
// SLATE-FILECHECK-STD C2Y c2y
// SLATE-FILECHECK-WARNING C2Y

void labeled(void) {
a:
  while (1) {
    __try {
    } __finally {
      continue a;
      break a;
    b:
      while (1) {
        continue a;
        break a;
        continue b;
        break b;
      }
    }
  }
}

// SLATE-FILECHECK-BEGIN C2Y
// C2Y: -Wjump-seh-finally
// C2Y: ⚠ jump out of __finally block has undefined behavior
// C2Y: ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally_named.c:7:7]
// C2Y: 6 │     } __finally {
// C2Y: 7 │       continue a;
// C2Y: ·       ───────────
// C2Y: 8 │       break a;
// C2Y: ╰────
// C2Y: -Wjump-seh-finally
// C2Y: ⚠ jump out of __finally block has undefined behavior
// C2Y: ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally_named.c:8:7]
// C2Y: 7 │       continue a;
// C2Y: 8 │       break a;
// C2Y: ·       ────────
// C2Y: 9 │     b:
// C2Y: ╰────
// C2Y: -Wjump-seh-finally
// C2Y: ⚠ jump out of __finally block has undefined behavior
// C2Y: ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally_named.c:11:9]
// C2Y: 10 │       while (1) {
// C2Y: 11 │         continue a;
// C2Y: ·         ───────────
// C2Y: 12 │         break a;
// C2Y: ╰────
// C2Y: -Wjump-seh-finally
// C2Y: ⚠ jump out of __finally block has undefined behavior
// C2Y: ╭─[tests/fixtures/clang/windows/x86_64/seh_jump_out_of_finally_named.c:12:9]
// C2Y: 11 │         continue a;
// C2Y: 12 │         break a;
// C2Y: ·         ────────
// C2Y: 13 │         continue b;
// C2Y: ╰────
// SLATE-FILECHECK-END C2Y
// SLATE-FILECHECK-BEGIN IR-C2Y
// IR-C2Y: module {
// IR-C2Y-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-C2Y-NEXT:         endian = little;
// IR-C2Y-NEXT:         pointer [size=8, align=8];
// IR-C2Y-NEXT:         stack_alignment = 16;
// IR-C2Y-NEXT:         long_double = f64;
// IR-C2Y-NEXT:         storage bool [size=1, align=1];
// IR-C2Y-NEXT:         storage i8, u8 [size=1, align=1];
// IR-C2Y-NEXT:         storage i16, u16 [size=2, align=2];
// IR-C2Y-NEXT:         storage i32, u32 [size=4, align=4];
// IR-C2Y-NEXT:         storage i64, u64 [size=8, align=8];
// IR-C2Y-NEXT:         storage i128, u128 [size=16, align=16];
// IR-C2Y-NEXT:         storage bf16 [size=2, align=2];
// IR-C2Y-NEXT:         storage f16 [size=2, align=2];
// IR-C2Y-NEXT:         storage f32 [size=4, align=4];
// IR-C2Y-NEXT:         storage f64 [size=8, align=8];
// IR-C2Y-NEXT:         storage f128 [size=16, align=16];
// IR-C2Y-NEXT:         storage d32 [size=4, align=4];
// IR-C2Y-NEXT:         storage d64 [size=8, align=8];
// IR-C2Y-NEXT:         storage d128 [size=16, align=16];
// IR-C2Y-NEXT:     }
// IR-C2Y-NEXT:     fn %[[VALUE_labeled:[0-9]+]] @labeled() -> void [linkage=external] [fallthrough=ret_void] {
// IR-C2Y-NEXT:         label %[[VALUE_a:[0-9]+]] a:
// IR-C2Y-NEXT:             while %[[VALUE0:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// IR-C2Y-NEXT:                 {
// IR-C2Y-NEXT:                     seh_try %[[VALUE1:[0-9]+]]
// IR-C2Y-NEXT:                         body:
// IR-C2Y-NEXT:                             {
// IR-C2Y-NEXT:                             }
// IR-C2Y-NEXT:                         finally:
// IR-C2Y-NEXT:                             {
// IR-C2Y-NEXT:                                 continue %[[VALUE0]];
// IR-C2Y-NEXT:                                 break %[[VALUE0]];
// IR-C2Y-NEXT:                                 label %[[VALUE_b:[0-9]+]] b:
// IR-C2Y-NEXT:                                     while %[[VALUE2:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// IR-C2Y-NEXT:                                         {
// IR-C2Y-NEXT:                                             continue %[[VALUE0]];
// IR-C2Y-NEXT:                                             break %[[VALUE0]];
// IR-C2Y-NEXT:                                             continue %[[VALUE2]];
// IR-C2Y-NEXT:                                             break %[[VALUE2]];
// IR-C2Y-NEXT:                                         }
// IR-C2Y-NEXT:                             }
// IR-C2Y-NEXT:                 }
// IR-C2Y-NEXT:     }
// IR-C2Y-NEXT: }
// SLATE-FILECHECK-END IR-C2Y
