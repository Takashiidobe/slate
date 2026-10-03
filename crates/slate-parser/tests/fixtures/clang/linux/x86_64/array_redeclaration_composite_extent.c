struct handler {
  const char *name;
  int value;
};

static struct handler handlers[8];
static struct handler handlers[] = {
    {"start", 1},
    {"end", 2},
};

int external[4];
int external[] = {1, 2};

extern int declared[3];
int declared[] = {7};

int sizes(void) {
  return sizeof handlers + sizeof external + sizeof declared;
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
// DEFAULT-NEXT:     type @type[[TYPE_handler:[0-9]+]] handler = struct {
// DEFAULT-NEXT:         field0 name: ptr<const i8>;
// DEFAULT-NEXT:         field1 value: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_handlers:[0-9]+]] handlers: array<@type[[TYPE_handler]], 8> [storage=static] [align=16] = aggregate<array<@type[[TYPE_handler]], 8>, zero_fill=true>(index0 = aggregate<@type[[TYPE_handler]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(6)>(%[[VALUE_str:[0-9]+]])), field1 = const<i32>(1)), index1 = aggregate<@type[[TYPE_handler]], zero_fill=false>(field0 = pointer_cast<ptr<const i8>, reason=assign>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str_2:[0-9]+]])), field1 = const<i32>(2))) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str]] .str[[VALUE_str]]: array<i8, 6> [storage=static] = code_units<array<i8, 6>>([115, 116, 97, 114, 116, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2]] .str[[VALUE_str_2]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([101, 110, 100, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_external:[0-9]+]] external: array<i32, 4> [storage=static] [align=16] = aggregate<array<i32, 4>, zero_fill=true>(index0 = const<i32>(1), index1 = const<i32>(2)) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_declared:[0-9]+]] declared: array<i32, 3> [storage=static] = aggregate<array<i32, 3>, zero_fill=true>(index0 = const<i32>(7)) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_sizes:[0-9]+]] @sizes() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(add<u64, overflow=wrap>(add<u64, overflow=wrap>(const<u64>(128), const<u64>(16)), const<u64>(12))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
