// SLATE-FILECHECK-DEFINES DEFAULT

int
main(int na, char* argv[])
{
	int wflg = 0, tflg = 0;
	int dflg = 0;
	__builtin_exit(0);
	while(1)
	{
		switch(argv[1][0])
		{
			help:
				__builtin_exit(0);
			case 'w':
			case 'W':
				wflg = 1;
				break;
			case 't':
			case 'T':
				tflg = 1;
				break;
			case 'd':
				dflg = 1;
				break;
		}
	}
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
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_na:[0-9]+]] na: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_wflg:[0-9]+]] wflg: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_tflg:[0-9]+]] tflg: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_dflg:[0-9]+]] dflg: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         while %[[VALUE1:[0-9]+]] ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 switch %[[VALUE2:[0-9]+]] widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), const<i32>(1)))), const<i32>(0)))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         label %[[VALUE_help:[0-9]+]] help:
// DEFAULT-NEXT:                             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:                         case %[[VALUE2]] const<i32>(119):
// DEFAULT-NEXT:                             case %[[VALUE2]] const<i32>(87):
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_wflg]], const<i32>(1));
// DEFAULT-NEXT:                         break %[[VALUE2]];
// DEFAULT-NEXT:                         case %[[VALUE2]] const<i32>(116):
// DEFAULT-NEXT:                             case %[[VALUE2]] const<i32>(84):
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_tflg]], const<i32>(1));
// DEFAULT-NEXT:                         break %[[VALUE2]];
// DEFAULT-NEXT:                         case %[[VALUE2]] const<i32>(100):
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_dflg]], const<i32>(1));
// DEFAULT-NEXT:                         break %[[VALUE2]];
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
