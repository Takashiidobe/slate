void abort(void);
void exit(int);

char *f(char *p) {
  short x = *p++ << 16;
  return p;
}

int main(void) {
  char *p = "";
  if (f(p) != p + 1)
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
// DEFAULT-NEXT:     global %8 .str8: array<i8, 1> [storage=static] = code_units<array<i8, 1>>([0]) [linkage=internal];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @f(%3 p: ptr<i8>) -> ptr<i8> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 x: i16 [storage=automatic];
// DEFAULT-NEXT:         let %9: ptr<i8> [synthetic] = read<ptr<i8>>(%3);
// DEFAULT-NEXT:         let %10: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i8>>(%3, read<ptr<i8>>(%10));
// DEFAULT-NEXT:         write<i16>(%4, truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%9)))), const<i32>(16))));
// DEFAULT-NEXT:         return read<ptr<i8>>(%3);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 p: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(1)>(%8);
// DEFAULT-NEXT:         if ne<ptr<i8>>(call<ptr<i8>, signature=fn(ptr<i8>) -> ptr<i8>>(%2, read<ptr<i8>>(%6)), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%6), const<i32>(1)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
