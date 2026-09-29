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
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_b:[0-9]+]] @b(%[[VALUE_y:[0-9]+]] y: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_y]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE1]])));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE1]])), read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_c:[0-9]+]] @c(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_y_2:[0-9]+]] y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%[[VALUE_x]]), read<i32>(%[[VALUE_y_2]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_d:[0-9]+]] @d(%[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_a:[0-9]+]] @a(%[[VALUE_x_3:[0-9]+]] x: i32, %[[VALUE_y_3:[0-9]+]] y: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_f:[0-9]+]] f: i32 [storage=automatic] = conditional<i32>(ne<i32>(read<i32>(%[[VALUE_x_3]]), const<i32>(0)), read<i32>(%[[VALUE_e]]), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE_z:[0-9]+]] z: i32 [storage=automatic] = read<i32>(%[[VALUE_y_3]]);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i32>) -> void>(%[[VALUE_b]], addr_of<ptr<i32>>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_c]], read<i32>(%[[VALUE_z]]), read<i32>(%[[VALUE_y_3]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_d]], read<i32>(%[[VALUE_f]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_a]], const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
