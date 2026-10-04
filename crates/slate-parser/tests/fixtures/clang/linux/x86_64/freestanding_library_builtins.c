void exit(int);
void abort(void);

#if __has_include(<stddef.h>)
#define HAS_STDDEF 1
#else
#define HAS_STDDEF 0
#endif

#ifdef __GCC_HAVE_DWARF2_CFI_ASM
#define CFI_ASM 1
#else
#define CFI_ASM 0
#endif

int hosted = __STDC_HOSTED__;
int has_stddef = HAS_STDDEF;
int cfi_asm = CFI_ASM;

int leave(void) { exit(1); }

int stop(void) { abort(); }

void copy(void *to, const void *from) { __builtin_memcpy(to, from, 4); }

// SLATE-FILECHECK-ARGS --dump-ir --compact-ir --show-metadata
// SLATE-FILECHECK-DEFINES HOSTED
// SLATE-FILECHECK-PREFIX-ARGS HOSTED
// SLATE-FILECHECK-DEFINES FREE
// SLATE-FILECHECK-PREFIX-ARGS FREE -ffreestanding
// SLATE-FILECHECK-DEFINES REHOSTED
// SLATE-FILECHECK-PREFIX-ARGS REHOSTED -ffreestanding -fhosted
// SLATE-FILECHECK-DEFINES NOBUILTIN
// SLATE-FILECHECK-PREFIX-ARGS NOBUILTIN -fno-builtin
// SLATE-FILECHECK-DEFINES REBUILTIN
// SLATE-FILECHECK-PREFIX-ARGS REBUILTIN -fno-builtin -fbuiltin
// SLATE-FILECHECK-DEFINES FREEBUILTIN
// SLATE-FILECHECK-PREFIX-ARGS FREEBUILTIN -ffreestanding -fbuiltin
// SLATE-FILECHECK-DEFINES NOEXIT
// SLATE-FILECHECK-PREFIX-ARGS NOEXIT -fno-builtin-exit
// SLATE-FILECHECK-DEFINES NOSTDINC
// SLATE-FILECHECK-PREFIX-ARGS NOSTDINC -nostdinc
// SLATE-FILECHECK-DEFINES UNWIND
// SLATE-FILECHECK-PREFIX-ARGS UNWIND -ffreestanding -fasynchronous-unwind-tables
// SLATE-FILECHECK-DEFINES NOUNWIND
// SLATE-FILECHECK-PREFIX-ARGS NOUNWIND -fno-asynchronous-unwind-tables

// SLATE-FILECHECK-BEGIN HOSTED
// HOSTED: module {
// HOSTED-NEXT:     target "x86_64-unknown-linux-gnu" {
// HOSTED-NEXT:         endian = little;
// HOSTED-NEXT:         pointer [size=8, align=8];
// HOSTED-NEXT:         stack_alignment = 16;
// HOSTED-NEXT:         long_double = f80;
// HOSTED-NEXT:         storage bool [size=1, align=1];
// HOSTED-NEXT:         storage i8, u8 [size=1, align=1];
// HOSTED-NEXT:         storage i16, u16 [size=2, align=2];
// HOSTED-NEXT:         storage i32, u32 [size=4, align=4];
// HOSTED-NEXT:         storage i64, u64 [size=8, align=8];
// HOSTED-NEXT:         storage i128, u128 [size=16, align=16];
// HOSTED-NEXT:         storage bf16 [size=2, align=2];
// HOSTED-NEXT:         storage f16 [size=2, align=2];
// HOSTED-NEXT:         storage f32 [size=4, align=4];
// HOSTED-NEXT:         storage f64 [size=8, align=8];
// HOSTED-NEXT:         storage f80 [size=16, align=16];
// HOSTED-NEXT:         storage f128 [size=16, align=16];
// HOSTED-NEXT:         storage d32 [size=4, align=4];
// HOSTED-NEXT:         storage d64 [size=8, align=8];
// HOSTED-NEXT:         storage d128 [size=16, align=16];
// HOSTED-NEXT:     }
// HOSTED-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// HOSTED-NEXT:     global %[[VALUE_has_stddef:[0-9]+]] has_stddef: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// HOSTED-NEXT:     global %[[VALUE_cfi_asm:[0-9]+]] cfi_asm: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// HOSTED-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [noreturn] [c="void(int)"] [c_builtin="exit"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// HOSTED-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn] [c="void(void)"] [c_builtin="abort"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// HOSTED-NEXT:     fn %[[VALUE_leave:[0-9]+]] @leave() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// HOSTED-NEXT:         call<void>(%[[VALUE_exit]], const<i32>(1));
// HOSTED-NEXT:     }
// HOSTED-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// HOSTED-NEXT:         call<void>(%[[VALUE_abort]]);
// HOSTED-NEXT:     }
// HOSTED-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"] [c_builtin_kind="builtin"] [c_builtin_header="string.h"];
// HOSTED-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_to:[0-9]+]] to: ptr<void> [c="void *"], %[[VALUE_from:[0-9]+]] from: ptr<const void> [c="const void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *)"] {
// HOSTED-NEXT:         call<ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_to]]), read<ptr<const void>>(%[[VALUE_from]]), reinterpret<u64>(widen<i64>(const<i32>(4))));
// HOSTED-NEXT:     }
// HOSTED-NEXT: }
// SLATE-FILECHECK-END HOSTED
// SLATE-FILECHECK-BEGIN FREE
// FREE: module {
// FREE-NEXT:     target "x86_64-unknown-linux-gnu" {
// FREE-NEXT:         endian = little;
// FREE-NEXT:         pointer [size=8, align=8];
// FREE-NEXT:         stack_alignment = 16;
// FREE-NEXT:         long_double = f80;
// FREE-NEXT:         storage bool [size=1, align=1];
// FREE-NEXT:         storage i8, u8 [size=1, align=1];
// FREE-NEXT:         storage i16, u16 [size=2, align=2];
// FREE-NEXT:         storage i32, u32 [size=4, align=4];
// FREE-NEXT:         storage i64, u64 [size=8, align=8];
// FREE-NEXT:         storage i128, u128 [size=16, align=16];
// FREE-NEXT:         storage bf16 [size=2, align=2];
// FREE-NEXT:         storage f16 [size=2, align=2];
// FREE-NEXT:         storage f32 [size=4, align=4];
// FREE-NEXT:         storage f64 [size=8, align=8];
// FREE-NEXT:         storage f80 [size=16, align=16];
// FREE-NEXT:         storage f128 [size=16, align=16];
// FREE-NEXT:         storage d32 [size=4, align=4];
// FREE-NEXT:         storage d64 [size=8, align=8];
// FREE-NEXT:         storage d128 [size=16, align=16];
// FREE-NEXT:     }
// FREE-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(0) [linkage=external] [c="int"];
// FREE-NEXT:     global %[[VALUE_has_stddef:[0-9]+]] has_stddef: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// FREE-NEXT:     global %[[VALUE_cfi_asm:[0-9]+]] cfi_asm: i32 [storage=static] = const<i32>(0) [linkage=external] [c="int"];
// FREE-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [c="void(int)"];
// FREE-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [c="void(void)"];
// FREE-NEXT:     fn %[[VALUE_leave:[0-9]+]] @leave() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// FREE-NEXT:         call<void>(%[[VALUE_exit]], const<i32>(1));
// FREE-NEXT:     }
// FREE-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// FREE-NEXT:         call<void>(%[[VALUE_abort]]);
// FREE-NEXT:     }
// FREE-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"] [c_builtin_kind="builtin"] [c_builtin_header="string.h"];
// FREE-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_to:[0-9]+]] to: ptr<void> [c="void *"], %[[VALUE_from:[0-9]+]] from: ptr<const void> [c="const void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *)"] {
// FREE-NEXT:         call<ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_to]]), read<ptr<const void>>(%[[VALUE_from]]), reinterpret<u64>(widen<i64>(const<i32>(4))));
// FREE-NEXT:     }
// FREE-NEXT: }
// SLATE-FILECHECK-END FREE
// SLATE-FILECHECK-BEGIN REHOSTED
// REHOSTED: module {
// REHOSTED-NEXT:     target "x86_64-unknown-linux-gnu" {
// REHOSTED-NEXT:         endian = little;
// REHOSTED-NEXT:         pointer [size=8, align=8];
// REHOSTED-NEXT:         stack_alignment = 16;
// REHOSTED-NEXT:         long_double = f80;
// REHOSTED-NEXT:         storage bool [size=1, align=1];
// REHOSTED-NEXT:         storage i8, u8 [size=1, align=1];
// REHOSTED-NEXT:         storage i16, u16 [size=2, align=2];
// REHOSTED-NEXT:         storage i32, u32 [size=4, align=4];
// REHOSTED-NEXT:         storage i64, u64 [size=8, align=8];
// REHOSTED-NEXT:         storage i128, u128 [size=16, align=16];
// REHOSTED-NEXT:         storage bf16 [size=2, align=2];
// REHOSTED-NEXT:         storage f16 [size=2, align=2];
// REHOSTED-NEXT:         storage f32 [size=4, align=4];
// REHOSTED-NEXT:         storage f64 [size=8, align=8];
// REHOSTED-NEXT:         storage f80 [size=16, align=16];
// REHOSTED-NEXT:         storage f128 [size=16, align=16];
// REHOSTED-NEXT:         storage d32 [size=4, align=4];
// REHOSTED-NEXT:         storage d64 [size=8, align=8];
// REHOSTED-NEXT:         storage d128 [size=16, align=16];
// REHOSTED-NEXT:     }
// REHOSTED-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// REHOSTED-NEXT:     global %[[VALUE_has_stddef:[0-9]+]] has_stddef: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// REHOSTED-NEXT:     global %[[VALUE_cfi_asm:[0-9]+]] cfi_asm: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// REHOSTED-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [noreturn] [c="void(int)"] [c_builtin="exit"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// REHOSTED-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn] [c="void(void)"] [c_builtin="abort"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// REHOSTED-NEXT:     fn %[[VALUE_leave:[0-9]+]] @leave() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// REHOSTED-NEXT:         call<void>(%[[VALUE_exit]], const<i32>(1));
// REHOSTED-NEXT:     }
// REHOSTED-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// REHOSTED-NEXT:         call<void>(%[[VALUE_abort]]);
// REHOSTED-NEXT:     }
// REHOSTED-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"] [c_builtin_kind="builtin"] [c_builtin_header="string.h"];
// REHOSTED-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_to:[0-9]+]] to: ptr<void> [c="void *"], %[[VALUE_from:[0-9]+]] from: ptr<const void> [c="const void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *)"] {
// REHOSTED-NEXT:         call<ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_to]]), read<ptr<const void>>(%[[VALUE_from]]), reinterpret<u64>(widen<i64>(const<i32>(4))));
// REHOSTED-NEXT:     }
// REHOSTED-NEXT: }
// SLATE-FILECHECK-END REHOSTED
// SLATE-FILECHECK-BEGIN NOBUILTIN
// NOBUILTIN: module {
// NOBUILTIN-NEXT:     target "x86_64-unknown-linux-gnu" {
// NOBUILTIN-NEXT:         endian = little;
// NOBUILTIN-NEXT:         pointer [size=8, align=8];
// NOBUILTIN-NEXT:         stack_alignment = 16;
// NOBUILTIN-NEXT:         long_double = f80;
// NOBUILTIN-NEXT:         storage bool [size=1, align=1];
// NOBUILTIN-NEXT:         storage i8, u8 [size=1, align=1];
// NOBUILTIN-NEXT:         storage i16, u16 [size=2, align=2];
// NOBUILTIN-NEXT:         storage i32, u32 [size=4, align=4];
// NOBUILTIN-NEXT:         storage i64, u64 [size=8, align=8];
// NOBUILTIN-NEXT:         storage i128, u128 [size=16, align=16];
// NOBUILTIN-NEXT:         storage bf16 [size=2, align=2];
// NOBUILTIN-NEXT:         storage f16 [size=2, align=2];
// NOBUILTIN-NEXT:         storage f32 [size=4, align=4];
// NOBUILTIN-NEXT:         storage f64 [size=8, align=8];
// NOBUILTIN-NEXT:         storage f80 [size=16, align=16];
// NOBUILTIN-NEXT:         storage f128 [size=16, align=16];
// NOBUILTIN-NEXT:         storage d32 [size=4, align=4];
// NOBUILTIN-NEXT:         storage d64 [size=8, align=8];
// NOBUILTIN-NEXT:         storage d128 [size=16, align=16];
// NOBUILTIN-NEXT:     }
// NOBUILTIN-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// NOBUILTIN-NEXT:     global %[[VALUE_has_stddef:[0-9]+]] has_stddef: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// NOBUILTIN-NEXT:     global %[[VALUE_cfi_asm:[0-9]+]] cfi_asm: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// NOBUILTIN-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [c="void(int)"];
// NOBUILTIN-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [c="void(void)"];
// NOBUILTIN-NEXT:     fn %[[VALUE_leave:[0-9]+]] @leave() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// NOBUILTIN-NEXT:         call<void>(%[[VALUE_exit]], const<i32>(1));
// NOBUILTIN-NEXT:     }
// NOBUILTIN-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// NOBUILTIN-NEXT:         call<void>(%[[VALUE_abort]]);
// NOBUILTIN-NEXT:     }
// NOBUILTIN-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"] [c_builtin_kind="builtin"] [c_builtin_header="string.h"];
// NOBUILTIN-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_to:[0-9]+]] to: ptr<void> [c="void *"], %[[VALUE_from:[0-9]+]] from: ptr<const void> [c="const void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *)"] {
// NOBUILTIN-NEXT:         call<ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_to]]), read<ptr<const void>>(%[[VALUE_from]]), reinterpret<u64>(widen<i64>(const<i32>(4))));
// NOBUILTIN-NEXT:     }
// NOBUILTIN-NEXT: }
// SLATE-FILECHECK-END NOBUILTIN
// SLATE-FILECHECK-BEGIN REBUILTIN
// REBUILTIN: module {
// REBUILTIN-NEXT:     target "x86_64-unknown-linux-gnu" {
// REBUILTIN-NEXT:         endian = little;
// REBUILTIN-NEXT:         pointer [size=8, align=8];
// REBUILTIN-NEXT:         stack_alignment = 16;
// REBUILTIN-NEXT:         long_double = f80;
// REBUILTIN-NEXT:         storage bool [size=1, align=1];
// REBUILTIN-NEXT:         storage i8, u8 [size=1, align=1];
// REBUILTIN-NEXT:         storage i16, u16 [size=2, align=2];
// REBUILTIN-NEXT:         storage i32, u32 [size=4, align=4];
// REBUILTIN-NEXT:         storage i64, u64 [size=8, align=8];
// REBUILTIN-NEXT:         storage i128, u128 [size=16, align=16];
// REBUILTIN-NEXT:         storage bf16 [size=2, align=2];
// REBUILTIN-NEXT:         storage f16 [size=2, align=2];
// REBUILTIN-NEXT:         storage f32 [size=4, align=4];
// REBUILTIN-NEXT:         storage f64 [size=8, align=8];
// REBUILTIN-NEXT:         storage f80 [size=16, align=16];
// REBUILTIN-NEXT:         storage f128 [size=16, align=16];
// REBUILTIN-NEXT:         storage d32 [size=4, align=4];
// REBUILTIN-NEXT:         storage d64 [size=8, align=8];
// REBUILTIN-NEXT:         storage d128 [size=16, align=16];
// REBUILTIN-NEXT:     }
// REBUILTIN-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// REBUILTIN-NEXT:     global %[[VALUE_has_stddef:[0-9]+]] has_stddef: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// REBUILTIN-NEXT:     global %[[VALUE_cfi_asm:[0-9]+]] cfi_asm: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// REBUILTIN-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [noreturn] [c="void(int)"] [c_builtin="exit"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// REBUILTIN-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn] [c="void(void)"] [c_builtin="abort"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// REBUILTIN-NEXT:     fn %[[VALUE_leave:[0-9]+]] @leave() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// REBUILTIN-NEXT:         call<void>(%[[VALUE_exit]], const<i32>(1));
// REBUILTIN-NEXT:     }
// REBUILTIN-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// REBUILTIN-NEXT:         call<void>(%[[VALUE_abort]]);
// REBUILTIN-NEXT:     }
// REBUILTIN-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"] [c_builtin_kind="builtin"] [c_builtin_header="string.h"];
// REBUILTIN-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_to:[0-9]+]] to: ptr<void> [c="void *"], %[[VALUE_from:[0-9]+]] from: ptr<const void> [c="const void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *)"] {
// REBUILTIN-NEXT:         call<ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_to]]), read<ptr<const void>>(%[[VALUE_from]]), reinterpret<u64>(widen<i64>(const<i32>(4))));
// REBUILTIN-NEXT:     }
// REBUILTIN-NEXT: }
// SLATE-FILECHECK-END REBUILTIN
// SLATE-FILECHECK-BEGIN FREEBUILTIN
// FREEBUILTIN: module {
// FREEBUILTIN-NEXT:     target "x86_64-unknown-linux-gnu" {
// FREEBUILTIN-NEXT:         endian = little;
// FREEBUILTIN-NEXT:         pointer [size=8, align=8];
// FREEBUILTIN-NEXT:         stack_alignment = 16;
// FREEBUILTIN-NEXT:         long_double = f80;
// FREEBUILTIN-NEXT:         storage bool [size=1, align=1];
// FREEBUILTIN-NEXT:         storage i8, u8 [size=1, align=1];
// FREEBUILTIN-NEXT:         storage i16, u16 [size=2, align=2];
// FREEBUILTIN-NEXT:         storage i32, u32 [size=4, align=4];
// FREEBUILTIN-NEXT:         storage i64, u64 [size=8, align=8];
// FREEBUILTIN-NEXT:         storage i128, u128 [size=16, align=16];
// FREEBUILTIN-NEXT:         storage bf16 [size=2, align=2];
// FREEBUILTIN-NEXT:         storage f16 [size=2, align=2];
// FREEBUILTIN-NEXT:         storage f32 [size=4, align=4];
// FREEBUILTIN-NEXT:         storage f64 [size=8, align=8];
// FREEBUILTIN-NEXT:         storage f80 [size=16, align=16];
// FREEBUILTIN-NEXT:         storage f128 [size=16, align=16];
// FREEBUILTIN-NEXT:         storage d32 [size=4, align=4];
// FREEBUILTIN-NEXT:         storage d64 [size=8, align=8];
// FREEBUILTIN-NEXT:         storage d128 [size=16, align=16];
// FREEBUILTIN-NEXT:     }
// FREEBUILTIN-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(0) [linkage=external] [c="int"];
// FREEBUILTIN-NEXT:     global %[[VALUE_has_stddef:[0-9]+]] has_stddef: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// FREEBUILTIN-NEXT:     global %[[VALUE_cfi_asm:[0-9]+]] cfi_asm: i32 [storage=static] = const<i32>(0) [linkage=external] [c="int"];
// FREEBUILTIN-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [c="void(int)"];
// FREEBUILTIN-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [c="void(void)"];
// FREEBUILTIN-NEXT:     fn %[[VALUE_leave:[0-9]+]] @leave() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// FREEBUILTIN-NEXT:         call<void>(%[[VALUE_exit]], const<i32>(1));
// FREEBUILTIN-NEXT:     }
// FREEBUILTIN-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// FREEBUILTIN-NEXT:         call<void>(%[[VALUE_abort]]);
// FREEBUILTIN-NEXT:     }
// FREEBUILTIN-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"] [c_builtin_kind="builtin"] [c_builtin_header="string.h"];
// FREEBUILTIN-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_to:[0-9]+]] to: ptr<void> [c="void *"], %[[VALUE_from:[0-9]+]] from: ptr<const void> [c="const void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *)"] {
// FREEBUILTIN-NEXT:         call<ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_to]]), read<ptr<const void>>(%[[VALUE_from]]), reinterpret<u64>(widen<i64>(const<i32>(4))));
// FREEBUILTIN-NEXT:     }
// FREEBUILTIN-NEXT: }
// SLATE-FILECHECK-END FREEBUILTIN
// SLATE-FILECHECK-BEGIN NOEXIT
// NOEXIT: module {
// NOEXIT-NEXT:     target "x86_64-unknown-linux-gnu" {
// NOEXIT-NEXT:         endian = little;
// NOEXIT-NEXT:         pointer [size=8, align=8];
// NOEXIT-NEXT:         stack_alignment = 16;
// NOEXIT-NEXT:         long_double = f80;
// NOEXIT-NEXT:         storage bool [size=1, align=1];
// NOEXIT-NEXT:         storage i8, u8 [size=1, align=1];
// NOEXIT-NEXT:         storage i16, u16 [size=2, align=2];
// NOEXIT-NEXT:         storage i32, u32 [size=4, align=4];
// NOEXIT-NEXT:         storage i64, u64 [size=8, align=8];
// NOEXIT-NEXT:         storage i128, u128 [size=16, align=16];
// NOEXIT-NEXT:         storage bf16 [size=2, align=2];
// NOEXIT-NEXT:         storage f16 [size=2, align=2];
// NOEXIT-NEXT:         storage f32 [size=4, align=4];
// NOEXIT-NEXT:         storage f64 [size=8, align=8];
// NOEXIT-NEXT:         storage f80 [size=16, align=16];
// NOEXIT-NEXT:         storage f128 [size=16, align=16];
// NOEXIT-NEXT:         storage d32 [size=4, align=4];
// NOEXIT-NEXT:         storage d64 [size=8, align=8];
// NOEXIT-NEXT:         storage d128 [size=16, align=16];
// NOEXIT-NEXT:     }
// NOEXIT-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// NOEXIT-NEXT:     global %[[VALUE_has_stddef:[0-9]+]] has_stddef: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// NOEXIT-NEXT:     global %[[VALUE_cfi_asm:[0-9]+]] cfi_asm: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// NOEXIT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [c="void(int)"];
// NOEXIT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn] [c="void(void)"] [c_builtin="abort"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// NOEXIT-NEXT:     fn %[[VALUE_leave:[0-9]+]] @leave() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// NOEXIT-NEXT:         call<void>(%[[VALUE_exit]], const<i32>(1));
// NOEXIT-NEXT:     }
// NOEXIT-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// NOEXIT-NEXT:         call<void>(%[[VALUE_abort]]);
// NOEXIT-NEXT:     }
// NOEXIT-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"] [c_builtin_kind="builtin"] [c_builtin_header="string.h"];
// NOEXIT-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_to:[0-9]+]] to: ptr<void> [c="void *"], %[[VALUE_from:[0-9]+]] from: ptr<const void> [c="const void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *)"] {
// NOEXIT-NEXT:         call<ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_to]]), read<ptr<const void>>(%[[VALUE_from]]), reinterpret<u64>(widen<i64>(const<i32>(4))));
// NOEXIT-NEXT:     }
// NOEXIT-NEXT: }
// SLATE-FILECHECK-END NOEXIT
// SLATE-FILECHECK-BEGIN NOSTDINC
// NOSTDINC: module {
// NOSTDINC-NEXT:     target "x86_64-unknown-linux-gnu" {
// NOSTDINC-NEXT:         endian = little;
// NOSTDINC-NEXT:         pointer [size=8, align=8];
// NOSTDINC-NEXT:         stack_alignment = 16;
// NOSTDINC-NEXT:         long_double = f80;
// NOSTDINC-NEXT:         storage bool [size=1, align=1];
// NOSTDINC-NEXT:         storage i8, u8 [size=1, align=1];
// NOSTDINC-NEXT:         storage i16, u16 [size=2, align=2];
// NOSTDINC-NEXT:         storage i32, u32 [size=4, align=4];
// NOSTDINC-NEXT:         storage i64, u64 [size=8, align=8];
// NOSTDINC-NEXT:         storage i128, u128 [size=16, align=16];
// NOSTDINC-NEXT:         storage bf16 [size=2, align=2];
// NOSTDINC-NEXT:         storage f16 [size=2, align=2];
// NOSTDINC-NEXT:         storage f32 [size=4, align=4];
// NOSTDINC-NEXT:         storage f64 [size=8, align=8];
// NOSTDINC-NEXT:         storage f80 [size=16, align=16];
// NOSTDINC-NEXT:         storage f128 [size=16, align=16];
// NOSTDINC-NEXT:         storage d32 [size=4, align=4];
// NOSTDINC-NEXT:         storage d64 [size=8, align=8];
// NOSTDINC-NEXT:         storage d128 [size=16, align=16];
// NOSTDINC-NEXT:     }
// NOSTDINC-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// NOSTDINC-NEXT:     global %[[VALUE_has_stddef:[0-9]+]] has_stddef: i32 [storage=static] = const<i32>(0) [linkage=external] [c="int"];
// NOSTDINC-NEXT:     global %[[VALUE_cfi_asm:[0-9]+]] cfi_asm: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// NOSTDINC-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [noreturn] [c="void(int)"] [c_builtin="exit"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// NOSTDINC-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn] [c="void(void)"] [c_builtin="abort"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// NOSTDINC-NEXT:     fn %[[VALUE_leave:[0-9]+]] @leave() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// NOSTDINC-NEXT:         call<void>(%[[VALUE_exit]], const<i32>(1));
// NOSTDINC-NEXT:     }
// NOSTDINC-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// NOSTDINC-NEXT:         call<void>(%[[VALUE_abort]]);
// NOSTDINC-NEXT:     }
// NOSTDINC-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"] [c_builtin_kind="builtin"] [c_builtin_header="string.h"];
// NOSTDINC-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_to:[0-9]+]] to: ptr<void> [c="void *"], %[[VALUE_from:[0-9]+]] from: ptr<const void> [c="const void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *)"] {
// NOSTDINC-NEXT:         call<ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_to]]), read<ptr<const void>>(%[[VALUE_from]]), reinterpret<u64>(widen<i64>(const<i32>(4))));
// NOSTDINC-NEXT:     }
// NOSTDINC-NEXT: }
// SLATE-FILECHECK-END NOSTDINC
// SLATE-FILECHECK-BEGIN UNWIND
// UNWIND: module {
// UNWIND-NEXT:     target "x86_64-unknown-linux-gnu" {
// UNWIND-NEXT:         endian = little;
// UNWIND-NEXT:         pointer [size=8, align=8];
// UNWIND-NEXT:         stack_alignment = 16;
// UNWIND-NEXT:         long_double = f80;
// UNWIND-NEXT:         storage bool [size=1, align=1];
// UNWIND-NEXT:         storage i8, u8 [size=1, align=1];
// UNWIND-NEXT:         storage i16, u16 [size=2, align=2];
// UNWIND-NEXT:         storage i32, u32 [size=4, align=4];
// UNWIND-NEXT:         storage i64, u64 [size=8, align=8];
// UNWIND-NEXT:         storage i128, u128 [size=16, align=16];
// UNWIND-NEXT:         storage bf16 [size=2, align=2];
// UNWIND-NEXT:         storage f16 [size=2, align=2];
// UNWIND-NEXT:         storage f32 [size=4, align=4];
// UNWIND-NEXT:         storage f64 [size=8, align=8];
// UNWIND-NEXT:         storage f80 [size=16, align=16];
// UNWIND-NEXT:         storage f128 [size=16, align=16];
// UNWIND-NEXT:         storage d32 [size=4, align=4];
// UNWIND-NEXT:         storage d64 [size=8, align=8];
// UNWIND-NEXT:         storage d128 [size=16, align=16];
// UNWIND-NEXT:     }
// UNWIND-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(0) [linkage=external] [c="int"];
// UNWIND-NEXT:     global %[[VALUE_has_stddef:[0-9]+]] has_stddef: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// UNWIND-NEXT:     global %[[VALUE_cfi_asm:[0-9]+]] cfi_asm: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// UNWIND-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [c="void(int)"];
// UNWIND-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [c="void(void)"];
// UNWIND-NEXT:     fn %[[VALUE_leave:[0-9]+]] @leave() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// UNWIND-NEXT:         call<void>(%[[VALUE_exit]], const<i32>(1));
// UNWIND-NEXT:     }
// UNWIND-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// UNWIND-NEXT:         call<void>(%[[VALUE_abort]]);
// UNWIND-NEXT:     }
// UNWIND-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"] [c_builtin_kind="builtin"] [c_builtin_header="string.h"];
// UNWIND-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_to:[0-9]+]] to: ptr<void> [c="void *"], %[[VALUE_from:[0-9]+]] from: ptr<const void> [c="const void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *)"] {
// UNWIND-NEXT:         call<ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_to]]), read<ptr<const void>>(%[[VALUE_from]]), reinterpret<u64>(widen<i64>(const<i32>(4))));
// UNWIND-NEXT:     }
// UNWIND-NEXT: }
// SLATE-FILECHECK-END UNWIND
// SLATE-FILECHECK-BEGIN NOUNWIND
// NOUNWIND: module {
// NOUNWIND-NEXT:     target "x86_64-unknown-linux-gnu" {
// NOUNWIND-NEXT:         endian = little;
// NOUNWIND-NEXT:         pointer [size=8, align=8];
// NOUNWIND-NEXT:         stack_alignment = 16;
// NOUNWIND-NEXT:         long_double = f80;
// NOUNWIND-NEXT:         storage bool [size=1, align=1];
// NOUNWIND-NEXT:         storage i8, u8 [size=1, align=1];
// NOUNWIND-NEXT:         storage i16, u16 [size=2, align=2];
// NOUNWIND-NEXT:         storage i32, u32 [size=4, align=4];
// NOUNWIND-NEXT:         storage i64, u64 [size=8, align=8];
// NOUNWIND-NEXT:         storage i128, u128 [size=16, align=16];
// NOUNWIND-NEXT:         storage bf16 [size=2, align=2];
// NOUNWIND-NEXT:         storage f16 [size=2, align=2];
// NOUNWIND-NEXT:         storage f32 [size=4, align=4];
// NOUNWIND-NEXT:         storage f64 [size=8, align=8];
// NOUNWIND-NEXT:         storage f80 [size=16, align=16];
// NOUNWIND-NEXT:         storage f128 [size=16, align=16];
// NOUNWIND-NEXT:         storage d32 [size=4, align=4];
// NOUNWIND-NEXT:         storage d64 [size=8, align=8];
// NOUNWIND-NEXT:         storage d128 [size=16, align=16];
// NOUNWIND-NEXT:     }
// NOUNWIND-NEXT:     global %[[VALUE_hosted:[0-9]+]] hosted: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// NOUNWIND-NEXT:     global %[[VALUE_has_stddef:[0-9]+]] has_stddef: i32 [storage=static] = const<i32>(1) [linkage=external] [c="int"];
// NOUNWIND-NEXT:     global %[[VALUE_cfi_asm:[0-9]+]] cfi_asm: i32 [storage=static] = const<i32>(0) [linkage=external] [c="int"];
// NOUNWIND-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32 [c="int"]) -> void [linkage=external] [noreturn] [c="void(int)"] [c_builtin="exit"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// NOUNWIND-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn] [c="void(void)"] [c_builtin="abort"] [c_builtin_kind="library"] [c_builtin_header="stdlib.h"];
// NOUNWIND-NEXT:     fn %[[VALUE_leave:[0-9]+]] @leave() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// NOUNWIND-NEXT:         call<void>(%[[VALUE_exit]], const<i32>(1));
// NOUNWIND-NEXT:     }
// NOUNWIND-NEXT:     fn %[[VALUE_stop:[0-9]+]] @stop() -> i32 [linkage=external] [fallthrough=ub_if_used] [c_storage="none"] [c_return="int"] [c="int(void)"] {
// NOUNWIND-NEXT:         call<void>(%[[VALUE_abort]]);
// NOUNWIND-NEXT:     }
// NOUNWIND-NEXT:     fn %[[VALUE___builtin_memcpy:[0-9]+]] @__builtin_memcpy(%[[VALUE1:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const void>, %[[VALUE3:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external] [c_builtin="__builtin_memcpy"] [c_builtin_kind="builtin"] [c_builtin_header="string.h"];
// NOUNWIND-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_to:[0-9]+]] to: ptr<void> [c="void *"], %[[VALUE_from:[0-9]+]] from: ptr<const void> [c="const void *"]) -> void [linkage=external] [fallthrough=ret_void] [c_storage="none"] [c_return="void"] [c="void(void *, const void *)"] {
// NOUNWIND-NEXT:         call<ptr<void>>(%[[VALUE___builtin_memcpy]], read<ptr<void>>(%[[VALUE_to]]), read<ptr<const void>>(%[[VALUE_from]]), reinterpret<u64>(widen<i64>(const<i32>(4))));
// NOUNWIND-NEXT:     }
// NOUNWIND-NEXT: }
// SLATE-FILECHECK-END NOUNWIND
