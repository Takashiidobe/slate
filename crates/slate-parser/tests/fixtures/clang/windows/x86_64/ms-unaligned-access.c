typedef unsigned short WCHAR;
typedef WCHAR __unaligned *LPUWSTR;
typedef struct { char c; int b; } S;

int load(__unaligned int *p) { return *p; }
void store(__unaligned int *p, int x) { *p = x; }
int member(S __unaligned *p) { return p->b; }
WCHAR wide(LPUWSTR p) { return p[1]; }
int plain(int *p) { return *p; }
char byte(__unaligned char *p) { return *p; }

// SLATE-FILECHECK-STD DEFAULT c17
// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-pc-windows-msvc" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f64;
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
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type[[TYPE_WCHAR:[0-9]+]] WCHAR = u16;
// DEFAULT-NEXT:     type @type[[TYPE_LPUWSTR:[0-9]+]] LPUWSTR = ptr<u16>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 c: i8;
// DEFAULT-NEXT:         field1 b: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_load:[0-9]+]] @load(%[[VALUE_p:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32, align=1>(deref(read<ptr<i32>>(%[[VALUE_p]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_store:[0-9]+]] @store(%[[VALUE_p_2:[0-9]+]] p: ptr<i32>, %[[VALUE_x:[0-9]+]] x: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i32, align=1>(deref(read<ptr<i32>>(%[[VALUE_p_2]])), read<i32>(%[[VALUE_x]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_member:[0-9]+]] @member(%[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE0]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32, align=1>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_p_3]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_wide:[0-9]+]] @wide(%[[VALUE_p_4:[0-9]+]] p: ptr<u16>) -> u16 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u16, align=1>(deref(ptr_offset<ptr<u16>, subtract=false, element=u16, overflow=ub>(read<ptr<u16>>(%[[VALUE_p_4]]), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_plain:[0-9]+]] @plain(%[[VALUE_p_5:[0-9]+]] p: ptr<i32>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(%[[VALUE_p_5]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_byte:[0-9]+]] @byte(%[[VALUE_p_6:[0-9]+]] p: ptr<i8>) -> i8 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i8>(deref(read<ptr<i8>>(%[[VALUE_p_6]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
