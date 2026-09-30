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
// DEFAULT-NEXT:     global %[[VALUE_a:[0-9]+]] a: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: ptr<i8> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_noret:[0-9]+]] @noret() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE1:[0-9]+]] <unnamed>: ptr<ptr<i8>>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_c:[0-9]+]] c: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:         while %[[VALUE2:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: i32 [synthetic] = call<i32, signature=fn(i32, ptr<ptr<i8>>) -> i32>(%[[VALUE_foo]], read<i32>(%[[VALUE_argc]]), read<ptr<ptr<i8>>>(%[[VALUE_argv]]));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_c]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE3]]), neg<i32, overflow=ub>(const<i32>(1)));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             switch %[[VALUE4:[0-9]+]] read<i32>(%[[VALUE_c]])
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     case %[[VALUE4]] const<i32>(115):
// DEFAULT-NEXT:                         case %[[VALUE4]] const<i32>(99):
// DEFAULT-NEXT:                             case %[[VALUE4]] const<i32>(102):
// DEFAULT-NEXT:                                 write<ptr<i8>>(%[[VALUE_a]], read<ptr<i8>>(%[[VALUE_b]]));
// DEFAULT-NEXT:                     break %[[VALUE4]];
// DEFAULT-NEXT:                     case %[[VALUE4]] const<i32>(118):
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_d]], const<i32>(1));
// DEFAULT-NEXT:                     break %[[VALUE4]];
// DEFAULT-NEXT:                     case %[[VALUE4]] const<i32>(86):
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_d]], const<i32>(0));
// DEFAULT-NEXT:                     break %[[VALUE4]];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_noret]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
