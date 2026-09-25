void abort(void);
void exit(int);

int gfbyte(void) { return 0; }

int main(void) {
  int i, j, k;

  i = gfbyte();

  i = i + 1;

  if (i == 0)
    k = -0;
  else
    k = i + 0;

  if (i != 1)
    abort();

  k = 1;
  if (k <= i)
    do
      j = gfbyte();
    while (k++ < i);

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
// DEFAULT-NEXT:     fn %1 @exit(%7 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @gfbyte() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %3 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %5 j: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 k: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%4, call<i32, signature=fn() -> i32>(%2));
// DEFAULT-NEXT:         call<i32, signature=fn() -> i32>(%2);
// DEFAULT-NEXT:         write<i32>(%4, add<i32, overflow=ub>(read<i32>(%4), const<i32>(1)));
// DEFAULT-NEXT:         if eq<i32>(read<i32>(%4), const<i32>(0))
// DEFAULT-NEXT:             write<i32>(%6, neg<i32, overflow=ub>(const<i32>(0)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<i32>(%6, add<i32, overflow=ub>(read<i32>(%4), const<i32>(0)));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%4), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:         if le<i32>(read<i32>(%6), read<i32>(%4))
// DEFAULT-NEXT:             do %8
// DEFAULT-NEXT:                 write<i32>(%5, call<i32, signature=fn() -> i32>(%2));
// DEFAULT-NEXT:                 call<i32, signature=fn() -> i32>(%2);
// DEFAULT-NEXT:             while {
// DEFAULT-NEXT:                 let %9: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%10));
// DEFAULT-NEXT:                 yield lt<i32>(read<i32>(%9), read<i32>(%4));
// DEFAULT-NEXT:             };
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%1, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
