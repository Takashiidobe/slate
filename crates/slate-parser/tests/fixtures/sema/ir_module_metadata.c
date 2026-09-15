// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --show-metadata

static int answer(void) {
    return 42;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target [char_signed=true, short_width=16, int_width=32, long_width=64, long_long_width=64, pointer_width=64, wchar_signed=true, wchar_width=32, long_double=X87];
// IR-NEXT:     fn %0 @answer() -> i32 [linkage=internal] [c_storage="static"] [c_return="Integer(Ranked { rank: Int, signed: true })"] {
// IR-NEXT:         return const<i32>(42);
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
