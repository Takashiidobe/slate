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
// DEFAULT-NEXT:     type @type[[TYPE_BlobSpan:[0-9]+]] BlobSpan = struct {
// DEFAULT-NEXT:         field0 right: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     fn %[[VALUE_render_blob_line:[0-9]+]] @render_blob_line(%[[VALUE_blobdata:[0-9]+]] blobdata: @type[[TYPE_BlobSpan]]) -> void [linkage=external] [abi=sysv64(native_c) -> void] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_buf:[0-9]+]] buf: array<i32, 32> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_data:[0-9]+]] data: ptr<i32> [storage=automatic] = array_decay<ptr<i32>, length=Some(32)>(%[[VALUE_buf]]);
// DEFAULT-NEXT:         let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field0(%[[VALUE_blobdata]])), const<i32>(0))
// DEFAULT-NEXT:             let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// DEFAULT-NEXT:             for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:                 init:
// DEFAULT-NEXT:                 condition: lt<i32>(read<i32>(%[[VALUE_i]]), mul<i32, overflow=ub>(const<i32>(2), read<i32>(%[[VALUE_n]])))
// DEFAULT-NEXT:                 increment: omitted
// DEFAULT-NEXT:                 body:
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE_data]]), read<i32>(%[[VALUE_i]]))), const<i32>(0));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         for %[[VALUE5:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: ne<i32>(read<i32>(%[[VALUE_n]]), const<i32>(0))
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 ;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
