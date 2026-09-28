// SLATE-FILECHECK-DEFINES DEFAULT

/* { dg-skip-if "too many arguments in function call" { bpf-*-* } } */

short *_offsetTable;
/* This tests to make sure PRE splits the entry block ->block 0 edge
   when there are multiple block 0 predecessors.
   This is done so that we don't end up with an insertion on the 
   entry block -> block 0 edge which would require a split at insertion
   time.  
   PR 13163.  */
void proc4WithoutFDFE(char *dst, const char *src, int next_offs, int bw,
		int bh, int pitch)
{
	do {
		int i = bw;
		int code = *src++;
		int x, l;
		int length = *src++ + 1;

		for (l = 0; l < length; l++) {
			int x;

			for (x = 0; x < 4; x++) ;
			if (i == 0)
				dst += pitch * 3;
		}
		char *dst2 = dst + _offsetTable[code] + next_offs;

		for (x = 0; x < 4; x++) {
			int j = 0;
			(dst + pitch * x)[j] = (dst2 + pitch * x)[j];
		}
		dst += pitch * 3;
	} while (--bh);
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
// DEFAULT-NEXT:     global %0 _offsetTable: ptr<i16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %1 @proc4WithoutFDFE(%2 dst: ptr<i8>, %3 src: ptr<const i8>, %4 next_offs: i32, %5 bw: i32, %6 bh: i32, %7 pitch: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %16
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %8 i: i32 [storage=automatic] = read<i32>(%5);
// DEFAULT-NEXT:                 let %9 code: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %20: ptr<const i8> [synthetic] = read<ptr<const i8>>(%3);
// DEFAULT-NEXT:                 let %21: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%20), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const i8>>(%3, read<ptr<const i8>>(%21));
// DEFAULT-NEXT:                 write<i32>(%9, widen<i32, reason=assign>(read<i8>(deref(read<ptr<const i8>>(%20)))));
// DEFAULT-NEXT:                 let %10 x: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %11 l: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %12 length: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %22: ptr<const i8> [synthetic] = read<ptr<const i8>>(%3);
// DEFAULT-NEXT:                 let %23: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%22), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const i8>>(%3, read<ptr<const i8>>(%23));
// DEFAULT-NEXT:                 write<i32>(%12, add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%22)))), const<i32>(1)));
// DEFAULT-NEXT:                 for %17
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%11), read<i32>(%12))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %24: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:                         let %25: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%24), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%11, read<i32>(%25));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %13 x: i32 [storage=automatic];
// DEFAULT-NEXT:                             for %18
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%13), const<i32>(4))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %26: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                                     let %27: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%26), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%13, read<i32>(%27));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     ;
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%8), const<i32>(0))
// DEFAULT-NEXT:                                 let %28: ptr<i8> [synthetic] = read<ptr<i8>>(%2);
// DEFAULT-NEXT:                                 let %29: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%28), mul<i32, overflow=ub>(read<i32>(%7), const<i32>(3)));
// DEFAULT-NEXT:                                 write<ptr<i8>>(%2, read<ptr<i8>>(%29));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 let %14 dst2: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%2), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%0), read<i32>(%9)))))), read<i32>(%4));
// DEFAULT-NEXT:                 for %19
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%10, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%10), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %30: i32 [synthetic] = read<i32>(%10);
// DEFAULT-NEXT:                         let %31: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%30), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%10, read<i32>(%31));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %15 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%2), mul<i32, overflow=ub>(read<i32>(%7), read<i32>(%10))), read<i32>(%15))), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%14), mul<i32, overflow=ub>(read<i32>(%7), read<i32>(%10))), read<i32>(%15)))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 let %32: ptr<i8> [synthetic] = read<ptr<i8>>(%2);
// DEFAULT-NEXT:                 let %33: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%32), mul<i32, overflow=ub>(read<i32>(%7), const<i32>(3)));
// DEFAULT-NEXT:                 write<ptr<i8>>(%2, read<ptr<i8>>(%33));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             let %34: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:             let %35: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%6, read<i32>(%35));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%35), const<i32>(0));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
