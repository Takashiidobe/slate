// SLATE-FILECHECK-DEFINES DEFAULT

extern char letters[26+1];
char letter;
int letter_number;
char letters[] = "AbCdefghiJklmNopQrStuVwXyZ";

static void
pad_home1 ()
{
  letter = letters[letter_number =
		   letters[letter_number + 1] ? letter_number +
		   1 : 0];
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
// DEFAULT-NEXT:     global %0 letters: array<i8, 27> [storage=static] = code_units<array<i8, 27>>([65, 98, 67, 100, 101, 102, 103, 104, 105, 74, 107, 108, 109, 78, 111, 112, 81, 114, 83, 116, 117, 86, 119, 88, 121, 90, 0]) [linkage=external];
// DEFAULT-NEXT:     global %1 letter: i8 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %2 letter_number: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %3 @pad_home1() -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32>(%2, conditional<i32>(ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(27)>(%0), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))))), const<i8>(0)), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1)), const<i32>(0)));
// DEFAULT-NEXT:         write<i8>(%1, read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(27)>(%0), conditional<i32>(ne<i8>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(27)>(%0), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1))))), const<i8>(0)), add<i32, overflow=ub>(read<i32>(%2), const<i32>(1)), const<i32>(0))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
