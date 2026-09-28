extern void abort(void);

int main() {
  int            hicount = 0;
  unsigned char *c;
  char          *str = "\x7f\xff";
  for (c = (unsigned char *)str; *c; c++) {
    if (!(((unsigned int)(*c)) < 0x80))
      hicount++;
  }
  if (hicount != 1)
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
// DEFAULT-NEXT:     global %5 .str5: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([127, 255, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %2 hicount: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %3 c: ptr<u8> [storage=automatic];
// DEFAULT-NEXT:         let %4 str: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(3)>(%5);
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<u8>>(%3, pointer_cast<ptr<u8>, reason=explicit>(read<ptr<i8>>(%4)));
// DEFAULT-NEXT:             condition: ne<u8>(read<u8>(deref(read<ptr<u8>>(%3))), const<u8>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: ptr<u8> [synthetic] = read<ptr<u8>>(%3);
// DEFAULT-NEXT:                 let %8: ptr<u8> [synthetic] = ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(read<ptr<u8>>(%7), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<u8>>(%3, read<ptr<u8>>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if not<bool>(lt<u32>(widen<u32, reason=explicit>(read<u8>(deref(read<ptr<u8>>(%3)))), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(128))))
// DEFAULT-NEXT:                         let %9: i32 [synthetic] = read<i32>(%2);
// DEFAULT-NEXT:                         let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%2, read<i32>(%10));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%2), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
