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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%11 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%3 p: ptr<i8>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %4 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %5 c: i8 [storage=automatic];
// DEFAULT-NEXT:         let %6 c2: i8 [storage=automatic];
// DEFAULT-NEXT:         let %7 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %8 pc: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%5);
// DEFAULT-NEXT:         let %9 pc2: ptr<i8> [storage=automatic] = addr_of<ptr<i8>>(%6);
// DEFAULT-NEXT:         let %10 pi: ptr<i32> [storage=automatic] = addr_of<ptr<i32>>(%7);
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%9)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%10)), const<i32>(1));
// DEFAULT-NEXT:         let %12: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:         let %13: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%12)));
// DEFAULT-NEXT:         let %14: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(and<i32>(widen<i32, reason=promotion>(read<i8>(%13)), read<i32>(deref(read<ptr<i32>>(%10)))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%12)), read<i8>(%14));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>) -> i32>(%2, read<ptr<i8>>(%9));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%9)), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         let %15: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:         let %16: i8 [synthetic] = read<i8>(deref(read<ptr<i8>>(%15)));
// DEFAULT-NEXT:         let %17: i8 [synthetic] = truncate<i8, reason=assign, fits=unknown>(and<i32>(widen<i32, reason=promotion>(read<i8>(%16)), read<i32>(deref(read<ptr<i32>>(%10)))));
// DEFAULT-NEXT:         write<i8>(deref(read<ptr<i8>>(%15)), read<i8>(%17));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
