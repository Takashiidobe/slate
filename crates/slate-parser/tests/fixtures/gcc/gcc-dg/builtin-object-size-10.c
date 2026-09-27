/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-early_objsz-details" } */
// { dg-skip-if "packed attribute missing for drone_source_packet" { "epiphany-*-*" } }

typedef struct {
    char sentinel[4];
    char data[0];
} drone_packet;
typedef struct {
    char type_str[16];
    char channel_hop;
} drone_source_packet;
drone_packet *
foo(char *x)
{
  drone_packet *dpkt = __builtin_malloc(sizeof(drone_packet)
					+ sizeof(drone_source_packet));
  drone_source_packet *spkt = (drone_source_packet *) dpkt->data;
  __builtin___snprintf_chk (spkt->type_str, 16,
			    1, __builtin_object_size (spkt->type_str, 1),
			    "%s", x);
  return dpkt;
}

/* { dg-final { scan-tree-dump "maximum object size 21" "early_objsz" } } */
/* { dg-final { scan-tree-dump "maximum subobject size 16" "early_objsz" } } */

// SLATE-FILECHECK-FLAVOR gcc
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 sentinel: array<i8, 4>;
// DEFAULT-NEXT:         field1 data: array<i8, 0>;
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 drone_packet = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 type_str: array<i8, 16>;
// DEFAULT-NEXT:         field1 channel_hop: i8;
// DEFAULT-NEXT:     } [size=17, align=1, offsets=[0, 16]];
// DEFAULT-NEXT:     type @type3 drone_source_packet = @type2;
// DEFAULT-NEXT:     global %19 .str19: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([37, 115, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %9 @__builtin_malloc(%8 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %15 @__builtin___snprintf_chk(%10 <unnamed>: ptr<i8>, %11 <unnamed>: u64, %12 <unnamed>: i32, %13 <unnamed>: u64, %14 <unnamed>: ptr<const i8>, ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %18 @__builtin_object_size(%16 <unnamed>: ptr<const void>, %17 <unnamed>: i32) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @foo(%5 x: ptr<i8>) -> ptr<@type0> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %6 dpkt: ptr<@type0> [storage=automatic] = pointer_cast<ptr<@type0>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%9, add<u64, overflow=wrap>(const<u64>(4), const<u64>(17))));
// DEFAULT-NEXT:         let %7 spkt: ptr<@type2> [storage=automatic] = pointer_cast<ptr<@type2>, reason=explicit>(array_decay<ptr<i8>, length=Some(0)>(field1(deref(read<ptr<@type0>>(%6)))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<i8>, u64, i32, u64, ptr<const i8>, ...) -> i32>(%15, array_decay<ptr<i8>, length=Some(16)>(field0(deref(read<ptr<@type2>>(%7)))), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))), const<i32>(1), call<u64, signature=fn(ptr<const void>, i32) -> u64>(%18, pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(field0(deref(read<ptr<@type2>>(%7))))), const<i32>(1)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(3)>(%19)), read<ptr<i8>>(%5));
// DEFAULT-NEXT:         return read<ptr<@type0>>(%6);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
