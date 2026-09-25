/* PR optimization/8726 */
/* Originator: Paul Eggert <eggert@twinsun.com> */

/* Verify that GCC doesn't miscompile tail calls on Sparc. */

extern void abort(void);

int fcntl_lock(int fd, int op, long long offset, long long count, int type);

int vfswrap_lock(char *fsp, int fd, int op, long long offset, long long count,
                 int type) {
  return fcntl_lock(fd, op, offset, count, type);
}

int fcntl_lock(int fd, int op, long long offset, long long count, int type) {
  return type;
}

int main(void) {
  if (vfswrap_lock(0, 1, 2, 3, 4, 5) != 5)
    abort();

  return 0;
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @fcntl_lock(%9 fd: i32, %10 op: i32, %11 offset: i64, %12 count: i64, %13 type: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %2 @vfswrap_lock(%3 fsp: ptr<i8>, %4 fd: i32, %5 op: i32, %6 offset: i64, %7 count: i64, %8 type: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, i32, i64, i64, i32) -> i32>(%1, read<i32>(%4), read<i32>(%5), read<i64>(%6), read<i64>(%7), read<i32>(%8));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(ptr<i8>, i32, i32, i64, i64, i32) -> i32>(%2, null<ptr<i8>>, const<i32>(1), const<i32>(2), widen<i64, reason=arg>(const<i32>(3)), widen<i64, reason=arg>(const<i32>(4)), const<i32>(5)), const<i32>(5))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
