void abort(void);
void exit(int);

void b(int *);
void c(int, int);
void d(int);

int e;

void a(int x, int y) {
  int f = x ? e : 0;
  int z = y;

  b(&y);
  c(z, y);
  d(f);
}

void b(int *y) { (*y)++; }

void c(int x, int y) {
  if (x == y)
    abort();
}

void d(int x) {}

int main(void) {
  a(0, 0);
  exit(0);
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
// DEFAULT-NEXT:     global %5 e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%16 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @b(%11 y: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %21: ptr<i32> [synthetic] = read<ptr<i32>>(%11);
// DEFAULT-NEXT:         let %22: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%21)));
// DEFAULT-NEXT:         let %23: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%22), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%21)), read<i32>(%23));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @c(%12 x: i32, %13 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%12), read<i32>(%13))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @d(%14 x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %6 @a(%7 x: i32, %8 y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %9 f: i32 [storage=automatic] = conditional<i32>(ne<i32>(read<i32>(%7), const<i32>(0)), read<i32>(%5), const<i32>(0));
// DEFAULT-NEXT:         let %10 z: i32 [storage=automatic] = read<i32>(%8);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%2, addr_of<ptr<i32>>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%3, read<i32>(%10), read<i32>(%8));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%4, read<i32>(%9));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%6, const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
