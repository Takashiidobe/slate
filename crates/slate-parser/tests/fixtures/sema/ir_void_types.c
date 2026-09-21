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
// IR-NEXT:         storage f16 [size=2, align=2];
// IR-NEXT:         storage f32 [size=4, align=4];
// IR-NEXT:         storage f64 [size=8, align=8];
// IR-NEXT:         storage f80 [size=16, align=16];
// IR-NEXT:         storage f128 [size=16, align=16];
// IR-NEXT:     }
// IR-NEXT:     type @type0 alias = void;
// IR-NEXT:     extern %2 incomplete_object: void [storage=static] [linkage=external];
// IR-NEXT:     fn %1 @returns_nothing() -> void [linkage=external];
// IR-NEXT:     fn %3 @address_of_incomplete() -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return addr_of<ptr<void>>(%2);
// IR-NEXT:     }
// IR-NEXT:     fn %4 @void_size() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %5 @void_align() -> u64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return const<u64>(1);
// IR-NEXT:     }
// IR-NEXT:     fn %6 @advance(%7 p: ptr<void>, %8 n: i32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return ptr_offset<ptr<void>, subtract=false, element=void, overflow=ub>(read<ptr<void>>(%7), read<i32>(%8));
// IR-NEXT:     }
// IR-NEXT:     fn %9 @distance(%10 a: ptr<void>, %11 b: ptr<void>) -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         return ptr_diff<i64, element=void, same_array=required, overflow=ub>(read<ptr<void>>(%10), read<ptr<void>>(%11));
// IR-NEXT:     }
// IR-NEXT:     fn %12 @label_delta() -> i64 [linkage=external] [fallthrough=ub_if_used] {
// IR-NEXT:         label %13 here:
// IR-NEXT:             return ptr_diff<i64, element=void, same_array=required, overflow=ub>(label_addr<ptr<void>>(%13), label_addr<ptr<void>>(%13));
// IR-NEXT:     }
// IR-NEXT: }
// SLATE-FILECHECK-END IR
