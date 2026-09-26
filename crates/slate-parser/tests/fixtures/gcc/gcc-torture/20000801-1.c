extern void abort(void);
extern void exit(int);

void foo(char *bp, unsigned n) {
  register char  c;
  register char *ep = bp + n;
  register char *sp;

  while (bp < ep) {
    sp     = bp + 3;
    c      = *sp;
    *sp    = *bp;
    *bp++  = c;
    sp     = bp + 1;
    c      = *sp;
    *sp    = *bp;
    *bp++  = c;
    bp    += 2;
  }
}

int main(void) {
  int one = 1;

  if (sizeof(int) != 4 * sizeof(char))
    exit(0);

  foo((char *)&one, sizeof(one));
  foo((char *)&one, sizeof(one));

  if (one != 1)
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
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @exit(%10 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @foo(%3 bp: ptr<i8>, %4 n: u32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %5 c: i8 [storage=automatic];
// DEFAULT-NEXT:         let %6 ep: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), read<u32>(%4));
// DEFAULT-NEXT:         let %7 sp: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         while %11 lt<ptr<i8>>(read<ptr<i8>>(%3), read<ptr<i8>>(%6))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<ptr<i8>>(%7, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), const<i32>(3)));
// DEFAULT-NEXT:                 write<i8>(%5, read<i8>(deref(read<ptr<i8>>(%7))));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%7)), read<i8>(deref(read<ptr<i8>>(%3))));
// DEFAULT-NEXT:                 let %12: ptr<i8> [synthetic] = read<ptr<i8>>(%3);
// DEFAULT-NEXT:                 let %13: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%12), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%3, read<ptr<i8>>(%13));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%12)), read<i8>(%5));
// DEFAULT-NEXT:                 write<ptr<i8>>(%7, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), const<i32>(1)));
// DEFAULT-NEXT:                 write<i8>(%5, read<i8>(deref(read<ptr<i8>>(%7))));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%7)), read<i8>(deref(read<ptr<i8>>(%3))));
// DEFAULT-NEXT:                 let %14: ptr<i8> [synthetic] = read<ptr<i8>>(%3);
// DEFAULT-NEXT:                 let %15: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%14), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%3, read<ptr<i8>>(%15));
// DEFAULT-NEXT:                 write<i8>(deref(read<ptr<i8>>(%14)), read<i8>(%5));
// DEFAULT-NEXT:                 let %16: ptr<i8> [synthetic] = read<ptr<i8>>(%3);
// DEFAULT-NEXT:                 let %17: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%16), const<i32>(2));
// DEFAULT-NEXT:                 write<ptr<i8>>(%3, read<ptr<i8>>(%17));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 one: i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(4), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4))), const<u64>(1)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, u32) -> void>(%2, pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%9)), truncate<u32, reason=arg, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, u32) -> void>(%2, pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(%9)), truncate<u32, reason=arg, fits=always>(const<u64>(4)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%9), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
