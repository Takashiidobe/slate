// SLATE-FILECHECK-DEFINES DEFAULT
// SLATE-FILECHECK-STD DEFAULT c17

/* Origin: PR c/3116 from Andreas Jaeger <aj@suse.de>.  */
/* When determining type compatibility of function types, we must remove
   qualifiers from argument types.  We used to fail to do this properly
   in store_parm_decls when comparing prototype and non-prototype
   declarations.  */
struct _IO_FILE {
  int _flags;
};

typedef struct _IO_FILE __FILE;
typedef struct _IO_FILE _IO_FILE;
typedef long int wchar_t;

extern wchar_t *fgetws (wchar_t *__restrict __ws, int __n,
                        __FILE *__restrict __stream);

wchar_t *
fgetws (buf, n, fp)
     wchar_t *buf;
     int n;
     _IO_FILE *fp;
{
  return (wchar_t *)0;
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
// DEFAULT-NEXT:     type @type0 _IO_FILE = struct {
// DEFAULT-NEXT:         field0 _flags: i32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0]];
// DEFAULT-NEXT:     type @type1 __FILE = @type0;
// DEFAULT-NEXT:     type @type2 _IO_FILE = @type0;
// DEFAULT-NEXT:     type @type3 wchar_t = i64;
// DEFAULT-NEXT:     fn %7 @fgetws(%8 buf: ptr<i64>, %9 n: i32, %10 fp: ptr<@type0>) -> ptr<i64> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return null<ptr<i64>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
