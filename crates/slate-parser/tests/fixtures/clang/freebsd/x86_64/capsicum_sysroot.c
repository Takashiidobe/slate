// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir
#include <sys/param.h>
#include <sys/capsicum.h>

int freebsd_release = __FreeBSD_version;
unsigned long long freebsd_cap_read = CAP_READ;

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-freebsd" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=8, align=8];
// IR-NEXT:         stack_alignment = 16;
// IR-NEXT:         long_double = f80;
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
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_freebsd_release:[0-9]+]] freebsd_release: i32 [storage=static] = const<i32>(1501000) [linkage=external];
// IR-NEXT:     global %[[VALUE_freebsd_cap_read:[0-9]+]] freebsd_cap_read: u64 [storage=static] = or<u64>(shl<u64, overflow=wrap, amount_out_of_range=ub>(const<u64>(1), add<i32, overflow=ub>(const<i32>(57), const<i32>(0))), const<u64>(1)) [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
