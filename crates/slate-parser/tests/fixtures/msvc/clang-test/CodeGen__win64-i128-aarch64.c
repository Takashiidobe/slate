
typedef int int128_t __attribute__((mode(TI)));

int128_t foo(void) { return 0; }


int128_t bar(int128_t a, int128_t b) { return a * b; }


void vararg(int a, ...) {
  __builtin_va_list ap;
  __builtin_va_start(ap, a);
  int128_t i = __builtin_va_arg(ap, int128_t);

  // __int128 is passed indirectly, so there is a double load.
  //

  // Explicitly check that the read is properly aligned.
  //

  // On ARM64EC __int128 is passed indirectly, so there is a double load.
  //
  __builtin_va_end(ap);
}

struct Align16 {
  char x[16];
} __attribute__((aligned(16)));

void vararg_struct(int a, ...) {
  __builtin_va_list ap;
  __builtin_va_start(ap, a);
  struct Align16 i = __builtin_va_arg(ap, struct Align16);

  // X64,ARM64EC: %argp.cur = load ptr, ptr %ap
  // X64,ARM64EC: %argp.next = getelementptr inbounds i8, ptr %argp.cur, i64 8
  // X64,ARM64EC: store ptr %argp.next, ptr %ap
  // X64,ARM64EC: [[P:%.*]] = load ptr, ptr %argp.cur
  // X64,ARM64EC: call void @llvm.memcpy.p0.p0.i64(ptr align 16 %i, ptr align 16 [[P]], i64 16, i1 false)


  __builtin_va_end(ap);
}

// SLATE-FILECHECK-FLAVOR clang
// SLATE-FILECHECK-ARGS -target=aarch64-pc-windows-msvc
// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT gnu17

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "aarch64-pc-windows-msvc" {
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
// DEFAULT-NEXT:     type @type0 int128_t = i128;
// DEFAULT-NEXT:     type @type1 Align16 = struct {
// DEFAULT-NEXT:         field0 x: array<i8, 16>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0]];
// DEFAULT-NEXT:     fn %1 @foo() -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return widen<i128, reason=return>(const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @bar(%3 a: i128, %4 b: i128) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return mul<i128, overflow=ub>(read<i128>(%3), read<i128>(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @vararg(%6 a: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %7 ap: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_start(%7);
// DEFAULT-NEXT:         let %8 i: i128 [storage=automatic] = va_arg<i128>(%7);
// DEFAULT-NEXT:         va_end(%7);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @vararg_struct(%11 a: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 ap: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_start(%12);
// DEFAULT-NEXT:         let %13 i: @type1 [storage=automatic] = copy<@type1, reason=assign>(va_arg<@type1>(%12));
// DEFAULT-NEXT:         va_end(%12);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
