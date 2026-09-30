// SLATE-FILECHECK-DEFINES DEFAULT

extern int bar(int);

int foo(int x)
{
  return 1 + bar(
	({
		int y;
		switch (x)
		{
		case 0: y = 1; break;
		case 1: y = 2; break;
		case 2: y = 3; break;
		case 3: y = 4; break;
		case 4: y = 5; break;
		case 5: y = 6; break;
		default: y = 7; break;
		}
		y;
	})
     );
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
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic];
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_y:[0-9]+]] y: i32 [storage=automatic];
// DEFAULT-NEXT:             switch %[[VALUE2:[0-9]+]] read<i32>(%[[VALUE_x]])
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     case %[[VALUE2]] const<i32>(0):
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], const<i32>(1));
// DEFAULT-NEXT:                     break %[[VALUE2]];
// DEFAULT-NEXT:                     case %[[VALUE2]] const<i32>(1):
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], const<i32>(2));
// DEFAULT-NEXT:                     break %[[VALUE2]];
// DEFAULT-NEXT:                     case %[[VALUE2]] const<i32>(2):
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], const<i32>(3));
// DEFAULT-NEXT:                     break %[[VALUE2]];
// DEFAULT-NEXT:                     case %[[VALUE2]] const<i32>(3):
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], const<i32>(4));
// DEFAULT-NEXT:                     break %[[VALUE2]];
// DEFAULT-NEXT:                     case %[[VALUE2]] const<i32>(4):
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], const<i32>(5));
// DEFAULT-NEXT:                     break %[[VALUE2]];
// DEFAULT-NEXT:                     case %[[VALUE2]] const<i32>(5):
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], const<i32>(6));
// DEFAULT-NEXT:                     break %[[VALUE2]];
// DEFAULT-NEXT:                     default %[[VALUE2]]:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_y]], const<i32>(7));
// DEFAULT-NEXT:                     break %[[VALUE2]];
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:             write<i32>(%[[VALUE1]], read<i32>(%[[VALUE_y]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         return add<i32, overflow=ub>(const<i32>(1), call<i32, signature=fn(i32) -> i32>(%[[VALUE_bar]], read<i32>(%[[VALUE1]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
