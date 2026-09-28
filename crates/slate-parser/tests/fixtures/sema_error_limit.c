int f0(void) { return u0; }
int f1(void) { return u1; }
int f2(void) { return u2; }
int f3(void) { return u3; }
int f4(void) { return u4; }
int f5(void) { return u5; }
int f6(void) { return u6; }
int f7(void) { return u7; }
int f8(void) { return u8; }
int f9(void) { return u9; }
int f10(void) { return u10; }
int f11(void) { return u11; }
int f12(void) { return u12; }
int f13(void) { return u13; }
int f14(void) { return u14; }
int f15(void) { return u15; }
int f16(void) { return u16; }
int f17(void) { return u17; }
int f18(void) { return u18; }
int f19(void) { return u19; }
int f20(void) { return u20; }
int f21(void) { return u21; }
int f22(void) { return u22; }
int f23(void) { return u23; }
int f24(void) { return u24; }

// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-IR-ERROR DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: Error:   × semantic analysis failed
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u0`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:1:23]
// DEFAULT: 1 │ int f0(void) { return u0; }
// DEFAULT: ·                       ──
// DEFAULT: 2 │ int f1(void) { return u1; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u1`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:2:23]
// DEFAULT: 1 │ int f0(void) { return u0; }
// DEFAULT: 2 │ int f1(void) { return u1; }
// DEFAULT: ·                       ──
// DEFAULT: 3 │ int f2(void) { return u2; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u2`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:3:23]
// DEFAULT: 2 │ int f1(void) { return u1; }
// DEFAULT: 3 │ int f2(void) { return u2; }
// DEFAULT: ·                       ──
// DEFAULT: 4 │ int f3(void) { return u3; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u3`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:4:23]
// DEFAULT: 3 │ int f2(void) { return u2; }
// DEFAULT: 4 │ int f3(void) { return u3; }
// DEFAULT: ·                       ──
// DEFAULT: 5 │ int f4(void) { return u4; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u4`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:5:23]
// DEFAULT: 4 │ int f3(void) { return u3; }
// DEFAULT: 5 │ int f4(void) { return u4; }
// DEFAULT: ·                       ──
// DEFAULT: 6 │ int f5(void) { return u5; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u5`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:6:23]
// DEFAULT: 5 │ int f4(void) { return u4; }
// DEFAULT: 6 │ int f5(void) { return u5; }
// DEFAULT: ·                       ──
// DEFAULT: 7 │ int f6(void) { return u6; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u6`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:7:23]
// DEFAULT: 6 │ int f5(void) { return u5; }
// DEFAULT: 7 │ int f6(void) { return u6; }
// DEFAULT: ·                       ──
// DEFAULT: 8 │ int f7(void) { return u7; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u7`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:8:23]
// DEFAULT: 7 │ int f6(void) { return u6; }
// DEFAULT: 8 │ int f7(void) { return u7; }
// DEFAULT: ·                       ──
// DEFAULT: 9 │ int f8(void) { return u8; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u8`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:9:23]
// DEFAULT: 8 │ int f7(void) { return u7; }
// DEFAULT: 9 │ int f8(void) { return u8; }
// DEFAULT: ·                       ──
// DEFAULT: 10 │ int f9(void) { return u9; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u9`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:10:23]
// DEFAULT: 9 │ int f8(void) { return u8; }
// DEFAULT: 10 │ int f9(void) { return u9; }
// DEFAULT: ·                       ──
// DEFAULT: 11 │ int f10(void) { return u10; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u10`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:11:24]
// DEFAULT: 10 │ int f9(void) { return u9; }
// DEFAULT: 11 │ int f10(void) { return u10; }
// DEFAULT: ·                        ───
// DEFAULT: 12 │ int f11(void) { return u11; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u11`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:12:24]
// DEFAULT: 11 │ int f10(void) { return u10; }
// DEFAULT: 12 │ int f11(void) { return u11; }
// DEFAULT: ·                        ───
// DEFAULT: 13 │ int f12(void) { return u12; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u12`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:13:24]
// DEFAULT: 12 │ int f11(void) { return u11; }
// DEFAULT: 13 │ int f12(void) { return u12; }
// DEFAULT: ·                        ───
// DEFAULT: 14 │ int f13(void) { return u13; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u13`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:14:24]
// DEFAULT: 13 │ int f12(void) { return u12; }
// DEFAULT: 14 │ int f13(void) { return u13; }
// DEFAULT: ·                        ───
// DEFAULT: 15 │ int f14(void) { return u14; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u14`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:15:24]
// DEFAULT: 14 │ int f13(void) { return u13; }
// DEFAULT: 15 │ int f14(void) { return u14; }
// DEFAULT: ·                        ───
// DEFAULT: 16 │ int f15(void) { return u15; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u15`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:16:24]
// DEFAULT: 15 │ int f14(void) { return u14; }
// DEFAULT: 16 │ int f15(void) { return u15; }
// DEFAULT: ·                        ───
// DEFAULT: 17 │ int f16(void) { return u16; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u16`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:17:24]
// DEFAULT: 16 │ int f15(void) { return u15; }
// DEFAULT: 17 │ int f16(void) { return u16; }
// DEFAULT: ·                        ───
// DEFAULT: 18 │ int f17(void) { return u17; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u17`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:18:24]
// DEFAULT: 17 │ int f16(void) { return u16; }
// DEFAULT: 18 │ int f17(void) { return u17; }
// DEFAULT: ·                        ───
// DEFAULT: 19 │ int f18(void) { return u18; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u18`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:19:24]
// DEFAULT: 18 │ int f17(void) { return u17; }
// DEFAULT: 19 │ int f18(void) { return u18; }
// DEFAULT: ·                        ───
// DEFAULT: 20 │ int f19(void) { return u19; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × unresolved ordinary name `u19`
// DEFAULT: ╭─[tests/fixtures/sema_error_limit.c:20:24]
// DEFAULT: 19 │ int f18(void) { return u18; }
// DEFAULT: 20 │ int f19(void) { return u19; }
// DEFAULT: ·                        ───
// DEFAULT: 21 │ int f20(void) { return u20; }
// DEFAULT: ╰────
// DEFAULT: Error:
// DEFAULT: × too many errors emitted, stopping now
// DEFAULT: ╭─[<unknown>:1:1]
// DEFAULT: ╰────
// SLATE-FILECHECK-END DEFAULT
