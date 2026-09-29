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
// DEFAULT-NEXT:     global %[[VALUE__offsetTable:[0-9]+]] _offsetTable: ptr<i16> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_proc4WithoutFDFE:[0-9]+]] @proc4WithoutFDFE(%[[VALUE_dst:[0-9]+]] dst: ptr<i8>, %[[VALUE_src:[0-9]+]] src: ptr<const i8>, %[[VALUE_next_offs:[0-9]+]] next_offs: i32, %[[VALUE_bw:[0-9]+]] bw: i32, %[[VALUE_bh:[0-9]+]] bh: i32, %[[VALUE_pitch:[0-9]+]] pitch: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         do %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = read<i32>(%[[VALUE_bw]]);
// DEFAULT-NEXT:                 let %[[VALUE_code:[0-9]+]] code: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_src]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const i8>>(%[[VALUE_src]], read<ptr<const i8>>(%[[VALUE2]]));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_code]], widen<i32, reason=assign>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE1]])))));
// DEFAULT-NEXT:                 let %[[VALUE_x:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_l:[0-9]+]] l: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE_length:[0-9]+]] length: i32 [storage=automatic];
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_src]]);
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<const i8>>(%[[VALUE_src]], read<ptr<const i8>>(%[[VALUE4]]));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_length]], add<i32, overflow=ub>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<const i8>>(%[[VALUE3]])))), const<i32>(1)));
// DEFAULT-NEXT:                 for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_l]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_l]]), read<i32>(%[[VALUE_length]]))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_l]]);
// DEFAULT-NEXT:                         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_l]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_x_2:[0-9]+]] x: i32 [storage=automatic];
// DEFAULT-NEXT:                             for %[[VALUE8:[0-9]+]]
// DEFAULT-NEXT:                                 init:
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_x_2]], const<i32>(0));
// DEFAULT-NEXT:                                 condition: lt<i32>(read<i32>(%[[VALUE_x_2]]), const<i32>(4))
// DEFAULT-NEXT:                                 increment: {
// DEFAULT-NEXT:                                     let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:                                     let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), const<i32>(1));
// DEFAULT-NEXT:                                     write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                                     yield void;
// DEFAULT-NEXT:                                 }
// DEFAULT-NEXT:                                 body:
// DEFAULT-NEXT:                                     ;
// DEFAULT-NEXT:                             if eq<i32>(read<i32>(%[[VALUE_i]]), const<i32>(0))
// DEFAULT-NEXT:                                 let %[[VALUE11:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_dst]]);
// DEFAULT-NEXT:                                 let %[[VALUE12:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE11]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_pitch]]), const<i32>(3)));
// DEFAULT-NEXT:                                 write<ptr<i8>>(%[[VALUE_dst]], read<ptr<i8>>(%[[VALUE12]]));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 let %[[VALUE_dst2:[0-9]+]] dst2: ptr<i8> [storage=automatic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_dst]]), widen<i32, reason=promotion>(read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(read<ptr<i16>>(%[[VALUE__offsetTable]]), read<i32>(%[[VALUE_code]])))))), read<i32>(%[[VALUE_next_offs]]));
// DEFAULT-NEXT:                 for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_x]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_x]]), const<i32>(4))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:                         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                             write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_dst]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_pitch]]), read<i32>(%[[VALUE_x]]))), read<i32>(%[[VALUE_j]]))), read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_dst2]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_pitch]]), read<i32>(%[[VALUE_x]]))), read<i32>(%[[VALUE_j]])))));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 let %[[VALUE16:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(%[[VALUE_dst]]);
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE16]]), mul<i32, overflow=ub>(read<i32>(%[[VALUE_pitch]]), const<i32>(3)));
// DEFAULT-NEXT:                 write<ptr<i8>>(%[[VALUE_dst]], read<ptr<i8>>(%[[VALUE17]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while {
// DEFAULT-NEXT:             let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_bh]]);
// DEFAULT-NEXT:             let %[[VALUE19:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_bh]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE19]]), const<i32>(0));
// DEFAULT-NEXT:         };
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
