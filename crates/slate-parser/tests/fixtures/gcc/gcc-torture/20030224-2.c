/* Make sure that we don't free any temp stack slots associated with
   initializing marker before we're finished with them.  */

extern void abort();

typedef struct {
  short v16;
} __attribute__((packed)) jint16_t;

struct node {
  jint16_t magic;
  jint16_t nodetype;
  int      totlen;
} __attribute__((packed));

struct node node, *node_p = &node;

int main() {
  struct node marker = {.magic    = (jint16_t){0x1985},
                        .nodetype = (jint16_t){0x2003},
                        .totlen   = node_p->totlen};
  if (marker.magic.v16 != 0x1985)
    abort();
  if (marker.nodetype.v16 != 0x2003)
    abort();
  return 0;
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
// DEFAULT-NEXT:     type @type0 = struct {
// DEFAULT-NEXT:         field0 v16: i16;
// DEFAULT-NEXT:     } [size=2, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 jint16_t = @type0;
// DEFAULT-NEXT:     type @type2 node = struct {
// DEFAULT-NEXT:         field0 magic: @type0;
// DEFAULT-NEXT:         field1 nodetype: @type0;
// DEFAULT-NEXT:         field2 totlen: i32;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0, 2, 4]];
// DEFAULT-NEXT:     global %4 node: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %5 node_p: ptr<@type2> [storage=static] = addr_of<ptr<@type2>>(%4) [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %6 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %7 marker: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = copy<@type0, reason=assign>(read<@type0>(compound_literal %8 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(6533))))), field1 = copy<@type0, reason=assign>(read<@type0>(compound_literal %9 [storage=automatic] = aggregate<@type0, zero_fill=false>(field0 = truncate<i16, reason=assign, fits=always>(const<i32>(8195))))), field2 = read<i32>(field2(deref(read<ptr<@type2>>(%5)))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(field0(%7)))), const<i32>(6533))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i16>(field0(field1(%7)))), const<i32>(8195))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
