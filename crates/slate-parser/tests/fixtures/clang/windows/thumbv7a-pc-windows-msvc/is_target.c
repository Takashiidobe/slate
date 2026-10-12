// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir --compact-ir

#if !__is_target_arch(thumb)
#error "mismatch: __is_target_arch(thumb)"
#endif

#if !__is_target_arch(thumbv7)
#error "mismatch: __is_target_arch(thumbv7)"
#endif

#if !__is_target_arch(arm)
#error "mismatch: __is_target_arch(arm)"
#endif

#if !__is_target_arch(armv7)
#error "mismatch: __is_target_arch(armv7)"
#endif

#if __is_target_arch(thumbv6)
#error "unexpected match: __is_target_arch(thumbv6)"
#endif

#if __is_target_arch(armv6)
#error "unexpected match: __is_target_arch(armv6)"
#endif

#if __is_target_arch(aarch64)
#error "unexpected match: __is_target_arch(aarch64)"
#endif

#if !__is_target_os(windows)
#error "mismatch: __is_target_os(windows)"
#endif

#if !__is_target_environment(msvc)
#error "mismatch: __is_target_environment(msvc)"
#endif

#if __is_target_arch(foo)
#error "unexpected match: __is_target_arch(foo)"
#endif

#if __is_target_vendor(foo)
#error "unexpected match: __is_target_vendor(foo)"
#endif

#if __is_target_os(foo)
#error "unexpected match: __is_target_os(foo)"
#endif

#if __is_target_environment(foo)
#error "unexpected match: __is_target_environment(foo)"
#endif

#if __is_target_variant_os(darwin)
#error "unexpected match: __is_target_variant_os(darwin)"
#endif

#if __is_target_variant_environment(macabi)
#error "unexpected match: __is_target_variant_environment(macabi)"
#endif

#if !(__has_builtin(__is_target_arch) && __has_builtin(__is_target_vendor) && __has_builtin(__is_target_os) && __has_builtin(__is_target_environment) && __has_builtin(__is_target_variant_os) && __has_builtin(__is_target_variant_environment))
#error "mismatch: (__has_builtin(__is_target_arch) && __has_builtin(__is_target_vendor) && __has_builtin(__is_target_os) && __has_builtin(__is_target_environment) && __has_builtin(__is_target_variant_os) && __has_builtin(__is_target_variant_environment))"
#endif

int is_target_ok;

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "thumbv7a-pc-windows-msvc" {
// IR-NEXT:         endian = little;
// IR-NEXT:         pointer [size=4, align=4];
// IR-NEXT:         stack_alignment = 8;
// IR-NEXT:         long_double = f64;
// IR-NEXT:         storage bool [size=1, align=1];
// IR-NEXT:         storage i8, u8 [size=1, align=1];
// IR-NEXT:         storage i16, u16 [size=2, align=2];
// IR-NEXT:         storage i32, u32 [size=4, align=4];
// IR-NEXT:         storage i64, u64 [size=8, align=8];
// IR-NEXT:         storage bf16 [size=2, align=2];
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:         storage d32 [size=4, align=4];
// IR-NEXT:         storage d64 [size=8, align=8];
// IR-NEXT:         storage d128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     global %[[VALUE_is_target_ok:[0-9]+]] is_target_ok: i32 [storage=static] [linkage=external];
// IR-NEXT: }
// SLATE-FILECHECK-END IR
