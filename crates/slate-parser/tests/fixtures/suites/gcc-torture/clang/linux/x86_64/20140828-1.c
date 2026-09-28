short *f(short *a, int b, int *d) __attribute__((noinline, noclone));

short *f(short *a, int b, int *d) {
  short c = *a;
  a++;
  c  = b << c;
  *d = c;
  return a;
}

int main(void) {
  int   d;
  short a[2];
  a[0] = 0;
  if (f(a, 1, &d) != &a[1])
    __builtin_abort();
  if (d != 1)
    __builtin_abort();
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
// DEFAULT-NEXT:     fn %3 @f(%4 a: ptr<i16>, %5 b: i32, %6 d: ptr<i32>) -> ptr<i16> [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %7 c: i16 [storage=automatic] = read<i16>(deref(read<ptr<i16>>(%4)));
// DEFAULT-NEXT:         let %15: ptr<i16> [synthetic] = read<ptr<i16>>(%4);
// DEFAULT-NEXT:         let %16: ptr<i16> [synthetic] = ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%15), const<i32>(1));
// DEFAULT-NEXT:         write<ptr<i16>>(%4, read<ptr<i16>>(%16));
// DEFAULT-NEXT:         write<i16>(%7, truncate<i16, reason=assign, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%5), widen<i32, reason=promotion>(read<i16>(%7)))));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%6)), widen<i32, reason=assign>(read<i16>(%7)));
// DEFAULT-NEXT:         return read<ptr<i16>>(%4);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %9 d: i32 [storage=automatic];
// DEFAULT-NEXT:         let %10 a: array<i16, 2> [storage=automatic];
// DEFAULT-NEXT:         write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(2)>(%10), const<i32>(0))), truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         if ne<ptr<i16>>(call<ptr<i16>, signature=fn(ptr<i16>, i32, ptr<i32>) -> ptr<i16>>(%3, array_decay<ptr<i16>, length=Some(2)>(%10), const<i32>(1), addr_of<ptr<i32>>(%9)), addr_of<ptr<i16>>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(2)>(%10), const<i32>(1)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%9), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%14);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
