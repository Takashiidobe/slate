void abort(void);
void exit(int);

int main(void) {
  int j = 1081;
  struct {
    signed int m : 11;
  } l;
  if ((l.m = j) == j)
    abort();
  exit(0);
}



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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 m: i32 : 11;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0], bit_offsets=[Some(0)], bit_units=[(0, 2)], field_units=[Some(0)]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(1081);
// DEFAULT-NEXT:         let %[[VALUE_l:[0-9]+]] l: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:         write<i32>(bitfield0<unit=0, bytes=0..2, bits=0..11>(%[[VALUE_l]]), read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         if eq<i32>(widen<i32, reason=assign>(truncate<i11b, reason=assign, fits=unknown>(read<i32>(%[[VALUE1]]))), read<i32>(%[[VALUE_j]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
