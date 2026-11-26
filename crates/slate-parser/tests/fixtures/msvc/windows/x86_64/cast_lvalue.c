int *advance(void **p) { return ((int *)*p)++; }
void replace(void **p, int *next) { ((int *)*p) = next; }
void add_byte(unsigned *p) { ((unsigned char)*p) += 1; }
int **address(void **p) { return &((int *)*p); }
float cast_value(unsigned x) { return (float)x; }

// SLATE-FILECHECK-ARGS /W4
// SLATE-FILECHECK-DEFINES WARN
// SLATE-FILECHECK-WARNING WARN

// SLATE-FILECHECK-BEGIN WARN
// WARN: -Wcast-lvalue
// WARN:   ⚠ nonstandard extension used: cast on lvalue
// WARN:    ╭─[tests/fixtures/msvc/windows/x86_64/cast_lvalue.c:1:34]
// WARN:  1 │ int *advance(void **p) { return ((int *)*p)++; }
// WARN:    ·                                  ─────────
// WARN:  2 │ void replace(void **p, int *next) { ((int *)*p) = next; }
// WARN:    ╰────
// WARN: -Wcast-lvalue
// WARN:   ⚠ nonstandard extension used: cast on lvalue
// WARN:    ╭─[tests/fixtures/msvc/windows/x86_64/cast_lvalue.c:2:38]
// WARN:  1 │ int *advance(void **p) { return ((int *)*p)++; }
// WARN:  2 │ void replace(void **p, int *next) { ((int *)*p) = next; }
// WARN:    ·                                      ─────────
// WARN:  3 │ void add_byte(unsigned *p) { ((unsigned char)*p) += 1; }
// WARN:    ╰────
// WARN: -Wcast-lvalue
// WARN:   ⚠ nonstandard extension used: cast on lvalue
// WARN:    ╭─[tests/fixtures/msvc/windows/x86_64/cast_lvalue.c:3:31]
// WARN:  2 │ void replace(void **p, int *next) { ((int *)*p) = next; }
// WARN:  3 │ void add_byte(unsigned *p) { ((unsigned char)*p) += 1; }
// WARN:    ·                               ─────────────────
// WARN:  4 │ int **address(void **p) { return &((int *)*p); }
// WARN:    ╰────
// SLATE-FILECHECK-END WARN
// SLATE-FILECHECK-BEGIN IR-WARN
// IR-WARN: module {
// IR-WARN-NEXT:     target "x86_64-pc-windows-msvc" {
// IR-WARN-NEXT:         endian = little;
// IR-WARN-NEXT:         pointer [size=8, align=8];
// IR-WARN-NEXT:         stack_alignment = 16;
// IR-WARN-NEXT:         long_double = f64;
// IR-WARN-NEXT:         storage bool [size=1, align=1];
// IR-WARN-NEXT:         storage i8, u8 [size=1, align=1];
// IR-WARN-NEXT:         storage i16, u16 [size=2, align=2];
// IR-WARN-NEXT:         storage i32, u32 [size=4, align=4];
// IR-WARN-NEXT:         storage i64, u64 [size=8, align=8];
// IR-WARN-NEXT:         storage i128, u128 [size=16, align=16];
// IR-WARN-NEXT:         storage bf16 [size=2, align=2];
// IR-WARN-NEXT:         storage f16 [size=2, align=2];
// IR-WARN-NEXT:         storage f32 [size=4, align=4];
// IR-WARN-NEXT:         storage f64 [size=8, align=8];
// IR-WARN-NEXT:         storage f128 [size=16, align=16];
// IR-WARN-NEXT:         storage d32 [size=4, align=4];
// IR-WARN-NEXT:         storage d64 [size=8, align=8];
// IR-WARN-NEXT:         storage d128 [size=16, align=16];
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_advance:[0-9]+]] @advance(%[[VALUE_p:[0-9]+]] p: ptr<ptr<void>>) -> ptr<i32> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<ptr<i32>> [synthetic] = pointer_cast<ptr<ptr<i32>>, reason=explicit>(addr_of<ptr<ptr<void>>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p]]))));
// IR-WARN-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE0]])));
// IR-WARN-NEXT:         let %[[VALUE2:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE1]]), const<i32>(1));
// IR-WARN-NEXT:         write<ptr<i32>>(deref(read<ptr<ptr<i32>>>(%[[VALUE0]])), read<ptr<i32>>(%[[VALUE2]]));
// IR-WARN-NEXT:         return read<ptr<i32>>(%[[VALUE1]]);
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_replace:[0-9]+]] @replace(%[[VALUE_p_2:[0-9]+]] p: ptr<ptr<void>>, %[[VALUE_next:[0-9]+]] next: ptr<i32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:         write<ptr<i32>>(deref(pointer_cast<ptr<ptr<i32>>, reason=explicit>(addr_of<ptr<ptr<void>>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p_2]]))))), read<ptr<i32>>(%[[VALUE_next]]));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_add_byte:[0-9]+]] @add_byte(%[[VALUE_p_3:[0-9]+]] p: ptr<u32>) -> void [linkage=external] [fallthrough=ret_void] {
// IR-WARN-NEXT:         let %[[VALUE3:[0-9]+]]: ptr<u8> [synthetic] = pointer_cast<ptr<u8>, reason=explicit>(addr_of<ptr<u32>>(deref(read<ptr<u32>>(%[[VALUE_p_3]]))));
// IR-WARN-NEXT:         let %[[VALUE4:[0-9]+]]: u8 [synthetic] = read<u8>(deref(read<ptr<u8>>(%[[VALUE3]])));
// IR-WARN-NEXT:         let %[[VALUE5:[0-9]+]]: u8 [synthetic] = reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=unknown>(add<i32, overflow=ub>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(%[[VALUE4]]))), const<i32>(1))));
// IR-WARN-NEXT:         write<u8>(deref(read<ptr<u8>>(%[[VALUE3]])), read<u8>(%[[VALUE5]]));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_address:[0-9]+]] @address(%[[VALUE_p_4:[0-9]+]] p: ptr<ptr<void>>) -> ptr<ptr<i32>> [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return addr_of<ptr<ptr<i32>>>(deref(pointer_cast<ptr<ptr<i32>>, reason=explicit>(addr_of<ptr<ptr<void>>>(deref(read<ptr<ptr<void>>>(%[[VALUE_p_4]]))))));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT:     fn %[[VALUE_cast_value:[0-9]+]] @cast_value(%[[VALUE_x:[0-9]+]] x: u32) -> f32 [linkage=external] [fallthrough=ub_if_used] {
// IR-WARN-NEXT:         return int_to_float<f32, reason=explicit, exact=false, rounding=nearest_even, exceptions=ignore>(read<u32>(%[[VALUE_x]]));
// IR-WARN-NEXT:     }
// IR-WARN-NEXT: }
// SLATE-FILECHECK-END IR-WARN
