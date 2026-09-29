// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-DEFINES AMBIGUOUS AMBIGUOUS
// SLATE-FILECHECK-DEFINES ANONYMOUS ANONYMOUS
// SLATE-FILECHECK-DEFINES UNKNOWN UNKNOWN
// SLATE-FILECHECK-DEFINES LATER LATER
// SLATE-FILECHECK-DEFINES TWO_WORDS TWO_WORDS
// SLATE-FILECHECK-DEFINES SIZE_KEYWORD SIZE_KEYWORD
// SLATE-FILECHECK-IR-ERROR AMBIGUOUS
// SLATE-FILECHECK-IR-ERROR ANONYMOUS
// SLATE-FILECHECK-IR-ERROR UNKNOWN
// SLATE-FILECHECK-IR-ERROR LATER
// SLATE-FILECHECK-IR-ERROR TWO_WORDS
// SLATE-FILECHECK-IR-ERROR SIZE_KEYWORD

struct pair { int first; int second; };
struct other { int zero; int second; };
struct wide { int w0; short half; char tail; int last; };
struct nest { int n0; struct { int n1; int deep; }; };
int global;
int *pointer;

int members(void) {
  int local;
  struct scoped { int s0; int s1; int s2; int inner; };
  __asm {
    mov eax, [ebx].last
    mov eax, [ebx + 4].last
    movzx eax, [ebx].tail
    movzx eax, [ebx].half
    mov [ebx].last, 1
    mov eax, global.last
    mov eax, pointer.last
    movzx eax, local.tail
    mov eax, [ebx].inner
    mov eax, [ebx].w0.last
  }
  return local;
}

int keywords(void) {
  __asm {
    mov eax, TYPE char
    mov eax, TYPE short
    mov eax, TYPE int
    mov eax, TYPE long
    mov eax, TYPE __int64
    mov eax, TYPE float
    mov eax, TYPE double
    mov eax, TYPE signed
    mov eax, TYPE unsigned
  }
}

#if defined(AMBIGUOUS)
int ambiguous(void) { __asm mov eax, [ebx].second }
#endif

#if defined(ANONYMOUS)
int anonymous(void) { __asm mov eax, [ebx].deep }
#endif

#if defined(UNKNOWN)
int unknown(void) { __asm mov eax, global.missing }
#endif

#if defined(LATER)
int later(void) { __asm mov eax, [ebx].after }
struct after_use { int a0; int after; };
#endif

#if defined(TWO_WORDS)
int two_words(void) { __asm mov eax, TYPE unsigned int }
#endif

#if defined(SIZE_KEYWORD)
int size_keyword(void) { __asm mov eax, SIZE int }
#endif

// SLATE-FILECHECK-BEGIN AMBIGUOUS
// AMBIGUOUS: Error:   × semantic analysis failed
// AMBIGUOUS: Error:
// AMBIGUOUS: × ambiguous member name in `__asm`
// AMBIGUOUS: ╭─[tests/fixtures/msvc/windows/i686/ms_asm_msvc_members.c:42:23]
// AMBIGUOUS: 41 │ #if defined(AMBIGUOUS)
// AMBIGUOUS: 42 │ int ambiguous(void) { __asm mov eax, [ebx].second }
// AMBIGUOUS: ·                       ───────────────────────────
// AMBIGUOUS: 43 │ #endif
// AMBIGUOUS: ╰────
// SLATE-FILECHECK-END AMBIGUOUS
// SLATE-FILECHECK-BEGIN ANONYMOUS
// ANONYMOUS: Error:   × semantic analysis failed
// ANONYMOUS: Error:
// ANONYMOUS: × ambiguous member name in `__asm`
// ANONYMOUS: ╭─[tests/fixtures/msvc/windows/i686/ms_asm_msvc_members.c:46:23]
// ANONYMOUS: 45 │ #if defined(ANONYMOUS)
// ANONYMOUS: 46 │ int anonymous(void) { __asm mov eax, [ebx].deep }
// ANONYMOUS: ·                       ─────────────────────────
// ANONYMOUS: 47 │ #endif
// ANONYMOUS: ╰────
// SLATE-FILECHECK-END ANONYMOUS
// SLATE-FILECHECK-BEGIN UNKNOWN
// UNKNOWN: Error:   × semantic analysis failed
// UNKNOWN: Error:
// UNKNOWN: × illegal struct/union member in `__asm`
// UNKNOWN: ╭─[tests/fixtures/msvc/windows/i686/ms_asm_msvc_members.c:50:21]
// UNKNOWN: 49 │ #if defined(UNKNOWN)
// UNKNOWN: 50 │ int unknown(void) { __asm mov eax, global.missing }
// UNKNOWN: ·                     ─────────────────────────────
// UNKNOWN: 51 │ #endif
// UNKNOWN: ╰────
// SLATE-FILECHECK-END UNKNOWN
// SLATE-FILECHECK-BEGIN LATER
// LATER: Error:   × semantic analysis failed
// LATER: Error:
// LATER: × illegal struct/union member in `__asm`
// LATER: ╭─[tests/fixtures/msvc/windows/i686/ms_asm_msvc_members.c:54:19]
// LATER: 53 │ #if defined(LATER)
// LATER: 54 │ int later(void) { __asm mov eax, [ebx].after }
// LATER: ·                   ──────────────────────────
// LATER: 55 │ struct after_use { int a0; int after; };
// LATER: ╰────
// SLATE-FILECHECK-END LATER
// SLATE-FILECHECK-BEGIN TWO_WORDS
// TWO_WORDS: Error:   × unexpected token in `__asm` operand
// TWO_WORDS: ╰─▶ unexpected token in `__asm` operand
// TWO_WORDS: ╭─[tests/fixtures/msvc/windows/i686/ms_asm_msvc_members.c:59:52]
// TWO_WORDS: 58 │ #if defined(TWO_WORDS)
// TWO_WORDS: 59 │ int two_words(void) { __asm mov eax, TYPE unsigned int }
// TWO_WORDS: ·                                                    ───
// TWO_WORDS: 60 │ #endif
// TWO_WORDS: ╰────
// SLATE-FILECHECK-END TWO_WORDS
// SLATE-FILECHECK-BEGIN SIZE_KEYWORD
// SIZE_KEYWORD: Error:   × expected `__asm` operand
// SIZE_KEYWORD: ╰─▶ expected `__asm` operand
// SIZE_KEYWORD: ╭─[tests/fixtures/msvc/windows/i686/ms_asm_msvc_members.c:63:46]
// SIZE_KEYWORD: 62 │ #if defined(SIZE_KEYWORD)
// SIZE_KEYWORD: 63 │ int size_keyword(void) { __asm mov eax, SIZE int }
// SIZE_KEYWORD: ·                                              ───
// SIZE_KEYWORD: 64 │ #endif
// SIZE_KEYWORD: ╰────
// SLATE-FILECHECK-END SIZE_KEYWORD
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
// DEFAULT-NEXT:     type @type[[TYPE_pair:[0-9]+]] pair = struct {
// DEFAULT-NEXT:         field0 first: i32;
// DEFAULT-NEXT:         field1 second: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_other:[0-9]+]] other = struct {
// DEFAULT-NEXT:         field0 zero: i32;
// DEFAULT-NEXT:         field1 second: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_wide:[0-9]+]] wide = struct {
// DEFAULT-NEXT:         field0 w0: i32;
// DEFAULT-NEXT:         field1 half: i16;
// DEFAULT-NEXT:         field2 tail: i8;
// DEFAULT-NEXT:         field3 last: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 6, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_nest:[0-9]+]] nest = struct {
// DEFAULT-NEXT:         field0 n0: i32;
// DEFAULT-NEXT:         field1 <anonymous>: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 n1: i32;
// DEFAULT-NEXT:         field1 deep: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_scoped:[0-9]+]] scoped = struct {
// DEFAULT-NEXT:         field0 s0: i32;
// DEFAULT-NEXT:         field1 s1: i32;
// DEFAULT-NEXT:         field2 s2: i32;
// DEFAULT-NEXT:         field3 inner: i32;
// DEFAULT-NEXT:     } [size=16, align=4, offsets=[0, 4, 8, 12]];
// DEFAULT-NEXT:     global %[[VALUE_global:[0-9]+]] global: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_pointer:[0-9]+]] pointer: ptr<i32> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_members:[0-9]+]] @members() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE0:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE0]]: u32 [synthetic];
// DEFAULT-NEXT:         let %[[VALUE_local:[0-9]+]] local: i32 [storage=automatic];
// DEFAULT-NEXT:         asm volatile "mov eax, [ebx].last\nmov eax, [ebx + 4].last\nmovzx eax, [ebx].tail\nmovzx eax, [ebx].half\nmov [ebx].last, 1\nmov eax, global.last\nmov eax, pointer.last\nmovzx eax, local.tail\nmov eax, [ebx].inner\nmov eax, [ebx].w0.last" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, [ebx + 8]\nmov eax, [ebx + 12]\nmovzx eax, byte ptr [ebx + 6]\nmovzx eax, word ptr [ebx + 4]\nmov dword ptr [ebx + 8], 1\nmov eax, " addr(%1 + 8) "\nmov eax, " addr(%2 + 8) "\nmovzx eax, " addr<byte>(%3 + 6) "\nmov eax, [ebx + 12]\nmov eax, [ebx + 8]";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE0]]);
// DEFAULT-NEXT:             in 1 [global] mem<read> place<i32>(%[[VALUE_global]]);
// DEFAULT-NEXT:             in 2 [pointer] mem<read> place<ptr<i32>>(%[[VALUE_pointer]]);
// DEFAULT-NEXT:             in 3 [local] mem<read> place<i32>(%[[VALUE_local]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_local]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_keywords:[0-9]+]] @keywords() -> i32 [linkage=external] [fallthrough=ret(reinterpret<i32, reason=return, fits=unknown>(read<u32>(%[[VALUE1:[0-9]+]])))] {
// DEFAULT-NEXT:         let %[[VALUE1]]: u32 [synthetic];
// DEFAULT-NEXT:         asm volatile "mov eax, type char\nmov eax, type short\nmov eax, type int\nmov eax, type long\nmov eax, type __int64\nmov eax, type float\nmov eax, type double\nmov eax, type signed\nmov eax, type unsigned" [dialect=intel] [alternative=none] {
// DEFAULT-NEXT:             template: "mov eax, 1\nmov eax, 2\nmov eax, 4\nmov eax, 4\nmov eax, 8\nmov eax, 4\nmov eax, 8\nmov eax, 1\nmov eax, 1";
// DEFAULT-NEXT:             out 0 "{eax}" [{ax}] width 32 place<u32>(%[[VALUE1]]);
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
