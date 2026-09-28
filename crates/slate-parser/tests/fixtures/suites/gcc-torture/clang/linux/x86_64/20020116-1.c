// SLATE-FILECHECK-DEFINES DEFAULT

void noret (void) __attribute__ ((noreturn));
int foo (int, char **);
char *a, *b;
int d;

int
main (int argc, char **argv)
{
  register int c;

  d = 1;
  while ((c = foo (argc, argv)) != -1)
    switch (c) {
    case 's':
    case 'c':
    case 'f':
      a = b;
      break;
    case 'v':
      d = 1;
      break;
    case 'V':
      d = 0;
      break;
    }
  noret ();
  return 0;
}

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
// DEFAULT-NEXT:     global %2 a: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 b: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %4 d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @noret() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %1 @foo(%9 <unnamed>: i32, %10 <unnamed>: ptr<ptr<i8>>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %5 @main(%6 argc: i32, %7 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %8 c: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%4, const<i32>(1));
// DEFAULT-NEXT:         while %11 {
// DEFAULT-NEXT:             write<i32>(%8, call<i32, signature=fn(i32, ptr<ptr<i8>>) -> i32>(%1, read<i32>(%6), read<ptr<ptr<i8>>>(%7)));
// DEFAULT-NEXT:             yield ne<i32>(call<i32, signature=fn(i32, ptr<ptr<i8>>) -> i32>(%1, read<i32>(%6), read<ptr<ptr<i8>>>(%7)), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             switch %12 read<i32>(%8)
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     case %12 const<i32>(115):
// DEFAULT-NEXT:                         case %12 const<i32>(99):
// DEFAULT-NEXT:                             case %12 const<i32>(102):
// DEFAULT-NEXT:                                 write<ptr<i8>>(%2, read<ptr<i8>>(%3));
// DEFAULT-NEXT:                     break %12;
// DEFAULT-NEXT:                     case %12 const<i32>(118):
// DEFAULT-NEXT:                         write<i32>(%4, const<i32>(1));
// DEFAULT-NEXT:                     break %12;
// DEFAULT-NEXT:                     case %12 const<i32>(86):
// DEFAULT-NEXT:                         write<i32>(%4, const<i32>(0));
// DEFAULT-NEXT:                     break %12;
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
