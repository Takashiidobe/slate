// SLATE-FILECHECK-DEFINES DEFAULT

typedef signed int      s32;
typedef signed long     s64;
typedef unsigned int    u32;
typedef unsigned long   u64;

extern __inline__ u32 foobar(int logmask)
{
        u32 ret = ~(1 << logmask);      // fails
        // s32 ret = ~(1 << logmask);   // ok
        // u64 ret = ~(1 << logmask);   // ok
        // s64 ret = ~(1 << logmask);   // ok
        return ret;
}

// This procedure compiles fine...
u32 good(u32 var)
{
        var = foobar(0);
        return var;
}

// This procedure does not compile...
// Same as above, but formal parameter is a pointer
// Both good() and fails() compile ok if we choose
// a different type for "ret" in foobar().
u32 fails(u32 *var)
{
        *var = foobar(0);
        return *var;
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
// DEFAULT-NEXT:     type @type[[TYPE_s32:[0-9]+]] s32 = i32;
// DEFAULT-NEXT:     type @type[[TYPE_s64:[0-9]+]] s64 = i64;
// DEFAULT-NEXT:     type @type[[TYPE_u32:[0-9]+]] u32 = u32;
// DEFAULT-NEXT:     type @type[[TYPE_u64:[0-9]+]] u64 = u64;
// DEFAULT-NEXT:     fn %[[VALUE_foobar:[0-9]+]] @foobar(%[[VALUE_logmask:[0-9]+]] logmask: i32) -> u32 [linkage=external] [inline=hint] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ret:[0-9]+]] ret: u32 [storage=automatic] = reinterpret<u32, reason=assign, fits=unknown>(not<i32>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), read<i32>(%[[VALUE_logmask]]))));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_ret]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_good:[0-9]+]] @good(%[[VALUE_var:[0-9]+]] var: u32) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u32>(%[[VALUE_var]], call<u32, signature=fn(i32) -> u32>(%[[VALUE_foobar]], const<i32>(0)));
// DEFAULT-NEXT:         call<u32, signature=fn(i32) -> u32>(%[[VALUE_foobar]], const<i32>(0));
// DEFAULT-NEXT:         return read<u32>(%[[VALUE_var]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fails:[0-9]+]] @fails(%[[VALUE_var_2:[0-9]+]] var: ptr<u32>) -> u32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%[[VALUE_var_2]])), call<u32, signature=fn(i32) -> u32>(%[[VALUE_foobar]], const<i32>(0)));
// DEFAULT-NEXT:         call<u32, signature=fn(i32) -> u32>(%[[VALUE_foobar]], const<i32>(0));
// DEFAULT-NEXT:         return read<u32>(deref(read<ptr<u32>>(%[[VALUE_var_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
