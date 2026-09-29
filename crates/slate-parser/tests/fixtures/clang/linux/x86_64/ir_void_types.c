// SLATE-FILECHECK-DEFINES IR
// SLATE-FILECHECK-ARGS --dump-ir -std=gnu17

typedef void alias;
alias returns_nothing(void);

extern void incomplete_object;

void *address_of_incomplete(void) { return &incomplete_object; }

unsigned long void_size(void) { return sizeof(void); }
unsigned long void_align(void) { return _Alignof(void); }

void *advance(void *p, int n) { return p + n; }
long distance(void *a, void *b) { return a - b; }

long label_delta(void) {
here:
  return &&here - &&here;
}

// SLATE-FILECHECK-BEGIN IR
// IR: module {
// IR-NEXT:     target "x86_64-unknown-linux-gnu" {
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
// IR-NEXT:     type @type[[TYPE_alias:[0-9]+]] alias = void;
// IR-NEXT:     extern %[[VALUE_incomplete_object:[0-9]+]] incomplete_object: void [storage=static] [linkage=external];
// IR-NEXT:     fn %[[VALUE_returns_nothing:[0-9]+]] @returns_nothing() -> void [linkage=external];
// IR-NEXT:     fn %[[VALUE_address_of_incomplete:[0-9]+]] @address_of_incomplete() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<void>>(%[[VALUE_incomplete_object]]);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_void_size:[0-9]+]] @void_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_void_align:[0-9]+]] @void_align() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_advance:[0-9]+]] @advance(%[[VALUE_p:[0-9]+]] p: ptr<void>, %[[VALUE_n:[0-9]+]] n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(read<ptr<void>>(%[[VALUE_p]]), read<i32>(%[[VALUE_n]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_distance:[0-9]+]] @distance(%[[VALUE_a:[0-9]+]] a: ptr<void>, %[[VALUE_b:[0-9]+]] b: ptr<void>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return ptr_diff<i64, element=void, same_array=required, overflow=ub>(read<ptr<void>>(%[[VALUE_a]]), read<ptr<void>>(%[[VALUE_b]]));
// IR-NEXT:     }
// IR-NEXT:     fn %[[VALUE_label_delta:[0-9]+]] @label_delta() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         label %[[VALUE_here:[0-9]+]] here:
// IR-NEXT:             return ptr_diff<i64, element=void, same_array=required, overflow=ub>(label_addr<ptr<void>>(%[[VALUE_here]]), label_addr<ptr<void>>(%[[VALUE_here]]));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
