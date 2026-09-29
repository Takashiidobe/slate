/* { dg-do compile } */
/* { dg-require-alias "" } */
/* { dg-require-visibility "" } */
/* { dg-add-options bind_pic_locally } */

#define ASMNAME(cname)  ASMNAME2 (__USER_LABEL_PREFIX__, cname)
#define ASMNAME2(prefix, cname) STRING (prefix) cname
#define STRING(x)    #x

extern void syslog (int __pri, __const char *__fmt, ...)
     __attribute__ ((__format__ (__printf__, 2, 3)));
extern void vsyslog (int __pri, __const char *__fmt, int __ap)
     __attribute__ ((__format__ (__printf__, 2, 0)));
void
__vsyslog(int pri, const char *fmt, int ap)
{
}
void
__syslog_chk(int pri, int flag, const char *fmt, ...)
{
}
void
__vsyslog_chk(int pri, int flag, const char *fmt, int ap)
{
}
extern __typeof (__vsyslog_chk) __EI___vsyslog_chk __asm__("" ASMNAME ("__vsyslog_chk")); extern __typeof (__vsyslog_chk) __EI___vsyslog_chk __attribute__((alias ("" "__GI___vsyslog_chk")));
void
__syslog(int pri, const char *fmt, ...)
{
}
extern __typeof (__syslog) syslog __attribute__ ((alias ("__syslog")));
extern __typeof (syslog) __EI_syslog __asm__("" ASMNAME ("syslog")); extern __typeof (syslog) __EI_syslog __attribute__((alias ("" "__GI_syslog")));
extern __typeof (__vsyslog) vsyslog __attribute__ ((alias ("__vsyslog")));
extern __typeof (vsyslog) __EI_vsyslog __asm__("" ASMNAME ("vsyslog")); extern __typeof (vsyslog) __EI_vsyslog __attribute__((alias ("" "__GI_vsyslog")));
extern __typeof (syslog) syslog __asm__ ("" ASMNAME ("__GI_syslog")) __attribute__ ((visibility ("hidden")));
extern __typeof (vsyslog) vsyslog __asm__ ("" ASMNAME ("__GI_vsyslog")) __attribute__ ((visibility ("hidden")));
extern __typeof (__vsyslog_chk) __vsyslog_chk __asm__ ("" ASMNAME ("__GI___vsyslog_chk")) __attribute__ ((visibility ("hidden")));

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %[[VALUE_syslog:[0-9]+]] @syslog(%[[VALUE___pri:[0-9]+]] __pri: i32, %[[VALUE___fmt:[0-9]+]] __fmt: ptr<const i8>, ...) -> void [linkage=external] [asm_name="__GI_syslog"] [visibility=hidden] [alias="__syslog"];
// DEFAULT-NEXT:     fn %[[VALUE_vsyslog:[0-9]+]] @vsyslog(%[[VALUE___pri_2:[0-9]+]] __pri: i32, %[[VALUE___fmt_2:[0-9]+]] __fmt: ptr<const i8>, %[[VALUE___ap:[0-9]+]] __ap: i32) -> void [linkage=external] [asm_name="__GI_vsyslog"] [visibility=hidden] [alias="__vsyslog"];
// DEFAULT-NEXT:     fn %[[VALUE___vsyslog:[0-9]+]] @__vsyslog(%[[VALUE_pri:[0-9]+]] pri: i32, %[[VALUE_fmt:[0-9]+]] fmt: ptr<const i8>, %[[VALUE_ap:[0-9]+]] ap: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___syslog_chk:[0-9]+]] @__syslog_chk(%[[VALUE_pri_2:[0-9]+]] pri: i32, %[[VALUE_flag:[0-9]+]] flag: i32, %[[VALUE_fmt_2:[0-9]+]] fmt: ptr<const i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___vsyslog_chk:[0-9]+]] @__vsyslog_chk(%[[VALUE_pri_3:[0-9]+]] pri: i32, %[[VALUE_flag_2:[0-9]+]] flag: i32, %[[VALUE_fmt_3:[0-9]+]] fmt: ptr<const i8>, %[[VALUE_ap_2:[0-9]+]] ap: i32) -> void [linkage=external] [asm_name="__GI___vsyslog_chk"] [visibility=hidden] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___EI___vsyslog_chk:[0-9]+]] @__EI___vsyslog_chk(%[[VALUE0:[0-9]+]] <unnamed>: i32, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE3:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [asm_name="__vsyslog_chk"] [alias="__GI___vsyslog_chk"];
// DEFAULT-NEXT:     fn %[[VALUE___syslog:[0-9]+]] @__syslog(%[[VALUE_pri_4:[0-9]+]] pri: i32, %[[VALUE_fmt_4:[0-9]+]] fmt: ptr<const i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___EI_syslog:[0-9]+]] @__EI_syslog(%[[VALUE4:[0-9]+]] <unnamed>: i32, %[[VALUE5:[0-9]+]] <unnamed>: ptr<const i8>, ...) -> void [linkage=external] [asm_name="syslog"] [alias="__GI_syslog"];
// DEFAULT-NEXT:     fn %[[VALUE___EI_vsyslog:[0-9]+]] @__EI_vsyslog(%[[VALUE6:[0-9]+]] <unnamed>: i32, %[[VALUE7:[0-9]+]] <unnamed>: ptr<const i8>, %[[VALUE8:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [asm_name="vsyslog"] [alias="__GI_vsyslog"];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
