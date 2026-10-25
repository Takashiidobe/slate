// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir /nologo /c /O2 /Ob2 /Oi- /Oy /Oxs /GS- /Gy /Gw- /GL /GF /Gs4096 /Gh /GH /GT /Gu /GA /Qspectre-load /Qpar-report:1 /Qvec-report:2 /QIntel-jcc-erratum /guard:ehcont- /homeparams /jumptablerdata /vlen=256 /favor:AMD64 /bigobj /Z7 /Zi /Zo- /ZH:SHA_256 /JMC- /F4096 /Fa /FAcs /Fd: out.pdb /Feout.exe /Fm: out.map /Fo: out.obj /Fpout.pch /FR /Fiout.i /FC /FS /MP /MP4 /showIncludes /validate-charset- /fastfail /volatileMetadata- /sourcelink:out.json /diagnostics:caret /external:W4 /external:I inc /external:anglebrackets /analyze /analyze:plugin out.dll /wd4996 /we 4013 /wo4100 /w14242 /WX- /WL /Wv:17 /Yuout.h /Y- /TC /Tc probe.c /EHsc /GR- /permissive- /Zc:wchar_t- /Zc:forScope /Zc:inline /Zc:threadSafeInit- /Zc:twoPhase- /presetPadding /MT
#if _MSVC_WARNING_LEVEL != 1
#error "codegen-only options leave the warning level alone"
#endif
#ifdef _NO_PREFETCHW
#error "/favor:AMD64 does not define _NO_PREFETCHW"
#endif

int square(int value) { return value * value; }

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage i128, u128 [size=16, align=16];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_square:[0-9]+]] @square(%[[VALUE_value:[0-9]+]] value: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return mul<i32, overflow=ub>(read<i32>(%[[VALUE_value]]), read<i32>(%[[VALUE_value]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
