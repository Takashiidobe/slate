void abort(void);
void exit(int);

int f(char *p) {}

int main(void) {
  char  c;
  char  c2;
  int   i   = 0;
  char *pc  = &c;
  char *pc2 = &c2;
  int  *pi  = &i;

  *pc2  = 1;
  *pi   = 1;
  *pc2 &= *pi;
  f(pc2);
  *pc2  = 1;
  *pc2 &= *pi;
  if (*pc2 != 1)
    abort();
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
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_p:[0-9]+]] p: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_c2:[0-9]+]] c2: i8 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_pc:[0-9]+]] pc: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_c]]);
// DEFAULT-NEXT:         let %[[VALUE_pc2:[0-9]+]] pc2: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%[[VALUE_c2]]);
// DEFAULT-NEXT:         let %[[VALUE_pi:[0-9]+]] pi: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%[[VALUE_i]]);
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_pc2]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_pi]])), const<i32>(1));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_pc2]]);
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%[[VALUE1]])));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(and<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE2]])), read<i32>(deref(read<ptr<i32>>(%[[VALUE_pi]])))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE1]])), read<i8>(%[[VALUE3]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>) -> i32>(%[[VALUE_f]], read<ptr<i8>>(%[[VALUE_pc2]]));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE_pc2]])), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_pc2]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%[[VALUE4]])));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(and<i32>(widen<i32, reason=promotion>(read<i8>(%[[VALUE5]])), read<i32>(deref(read<ptr<i32>>(%[[VALUE_pi]])))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%[[VALUE4]])), read<i8>(%[[VALUE6]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE_pc2]])))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
