#ifdef _REENTRANT
int reentrant = _REENTRANT;
#else
int reentrant = -1;
#endif

int __attribute__((bogus_attr)) warned_only;

#line 9 "a/b/remapped.c"
const char *file = __FILE__;
const char *file_name = __FILE_NAME__;

// SLATE-FILECHECK-ARGS -pthread -w -Werror -fmacro-prefix-map=a/b/=Y/ -fmacro-prefix-map=a/=X/ -ffile-prefix-map=rem=N
// SLATE-FILECHECK-ARGS -mllvm -max-store-memset=4294967294 -mllvm=-x86-asm-syntax=att -S -mno-red-zone -mskip-rax-setup -mretpoline -fintegrated-as -Qunused-arguments
// SLATE-FILECHECK-ARGS -fno-delete-null-pointer-checks -ftrivial-auto-var-init=zero -fexcess-precision=standard -ftls-model=initial-exec


// SLATE-FILECHECK-ARGS --dump-ir --compact-ir
// SLATE-FILECHECK-DEFINES DEFAULT

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
// DEFAULT-NEXT:     global %[[VALUE_reentrant:[0-9]+]] reentrant: i32 [storage=static] = const<i32>(1) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_warned_only:[0-9]+]] warned_only: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([89, 47, 114, 101, 109, 97, 112, 112, 101, 100, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_file:[0-9]+]] file: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([78, 97, 112, 112, 101, 100, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_file_name:[0-9]+]] file_name: ptr<const i8> [storage=static] = pointer_cast<ptr<const i8>>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_2]])) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
