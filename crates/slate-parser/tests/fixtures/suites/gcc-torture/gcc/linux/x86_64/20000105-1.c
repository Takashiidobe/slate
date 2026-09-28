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
// DEFAULT-NEXT:     fn %8 @exit(%7 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %0 @main(%2 na: i32, %3 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %4 wflg: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %5 tflg: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         let %6 dflg: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%8, const<i32>(0));
// DEFAULT-NEXT:         while %9 ne<i32>(const<i32>(1), const<i32>(0))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 switch %10 widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%3), const<i32>(1)))), const<i32>(0)))))
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         label %1 help:
// DEFAULT-NEXT:                             call<void, signature=fn(i32) -> void>(%8, const<i32>(0));
// DEFAULT-NEXT:                         case %10 const<i32>(119):
// DEFAULT-NEXT:                             case %10 const<i32>(87):
// DEFAULT-NEXT:                                 write<i32>(%4, const<i32>(1));
// DEFAULT-NEXT:                         break %10;
// DEFAULT-NEXT:                         case %10 const<i32>(116):
// DEFAULT-NEXT:                             case %10 const<i32>(84):
// DEFAULT-NEXT:                                 write<i32>(%5, const<i32>(1));
// DEFAULT-NEXT:                         break %10;
// DEFAULT-NEXT:                         case %10 const<i32>(100):
// DEFAULT-NEXT:                             write<i32>(%6, const<i32>(1));
// DEFAULT-NEXT:                         break %10;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
