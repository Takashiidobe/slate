// SLATE-FILECHECK-DEFINES DEFAULT

struct BlobSpan {
	int right;
};
/* This test makes sure we don't accidentally cause a bad insertion to occur
   by choosing the wrong variable name so that we end up with a use not
   dominated by a def. */
void render_blob_line(struct BlobSpan blobdata) {
	int buf[4 * 8];
	int *data = buf;
	int i, n = 0;
	if (blobdata.right)
		n++;
	if (n)
		for (; i < 2 * n;)
			data[i] = 0;
	n *= 2;
	for (; n;) ;
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
// DEFAULT-NEXT:     type @type0 BlobSpan = struct {
// DEFAULT-NEXT:         field0 right: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %1 @render_blob_line(%2 blobdata: @type0) -> void [linkage=external] [abi=sysv64(coerce<i32>) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 buf: array<i32, 32> [storage=automatic];
// DEFAULT-NEXT:         let %4 data: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(32)>(%3);
// DEFAULT-NEXT:         let %5 i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %6 n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%2)), const<i32>(0))
// DEFAULT-NEXT:             let %9: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:             let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%6, read<i32>(%10));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             for %7
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%5), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%6)))
// DEFAULT-NEXT:                 increment: omitted
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%4), read<i32>(%5))), const<i32>(0));
// DEFAULT-NEXT:         let %11: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %12: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%11), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%12));
// DEFAULT-NEXT:         for %8
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%6), const<i32>(0))
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
