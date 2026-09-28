// SLATE-FILECHECK-DEFINES DEFAULT

void make_file_symbol_completion_list (char *);
/* This tests to make sure PRE doesn't choose the wrong name when
   inserting phi nodes.  Otherwise, we get uses that aren't dominated
   by defs.  
   PR 13177.  */
void location_completer (char *text)
{
	char *p, *symbol_start = text;
	for (p = text; *p != '\0'; ++p) {
		if (*p == '\\' && p[1] == '\'')
			p++;
		else if (*p == ':')
			symbol_start = p + 1;
		else 
			symbol_start = p + 1;
		make_file_symbol_completion_list(symbol_start);
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
// DEFAULT-NEXT:     fn %0 @make_file_symbol_completion_list(%5 <unnamed>: ptr<i8>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @location_completer(%2 text: ptr<i8>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %3 p: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %4 symbol_start: ptr<i8> [storage=automatic] = read<ptr<i8>>(%2);
// DEFAULT-NEXT:         for %6
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<ptr<i8>>(%3, read<ptr<i8>>(%2));
// DEFAULT-NEXT:             condition: ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%3)))), const<i32>(0))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %7: ptr<i8> [synthetic] = read<ptr<i8>>(%3);
// DEFAULT-NEXT:                 let %8: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%7), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i8>>(%3, read<ptr<i8>>(%8));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     if logical_and<bool>(eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%3)))), const<i32>(92)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), const<i32>(1))))), const<i32>(39)))
// DEFAULT-NEXT:                         let %9: ptr<i8> [synthetic] = read<ptr<i8>>(%3);
// DEFAULT-NEXT:                         let %10: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%9), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<i8>>(%3, read<ptr<i8>>(%10));
// DEFAULT-NEXT:                     else
// DEFAULT-NEXT:                         if eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%3)))), const<i32>(58))
// DEFAULT-NEXT:                             write<ptr<i8>>(%4, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), const<i32>(1)));
// DEFAULT-NEXT:                         else
// DEFAULT-NEXT:                             write<ptr<i8>>(%4, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%3), const<i32>(1)));
// DEFAULT-NEXT:                     call<void, signature=fn(ptr<i8>) -> void>(%0, read<ptr<i8>>(%4));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
