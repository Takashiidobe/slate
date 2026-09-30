/* PR tree-optimization/103255 */

struct H {
  unsigned a;
  unsigned b;
  unsigned c;
};

#if __SIZEOF_POINTER__ >= 4
#define ADDR 0x400000
#else
#define ADDR 0x4000
#endif
#define OFF 0x20

int main() {
  struct H     *h = 0;
  unsigned long o;
  volatile int  t = 1;

  for (o = OFF; o <= OFF; o += 0x1000) {
    struct H *u;
    u = (struct H *)(ADDR + o);
    if (t) {
      h = u;
      break;
    }
  }

  if (h == 0)
    return 0;
  unsigned *tt = &h->b;
  if ((__SIZE_TYPE__)tt != (ADDR + OFF + __builtin_offsetof(struct H, b)))
    __builtin_abort();
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
// DEFAULT-NEXT:     type @type[[TYPE_H:[0-9]+]] H = struct {
// DEFAULT-NEXT:         field0 a: u32;
// DEFAULT-NEXT:         field1 b: u32;
// DEFAULT-NEXT:         field2 c: u32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_h:[0-9]+]] h: ptr<@type[[TYPE_H]]> [storage=automatic] = null<ptr<@type[[TYPE_H]]>>;
// DEFAULT-NEXT:         let %[[VALUE_o:[0-9]+]] o: u64 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_t:[0-9]+]] t: volatile i32 [storage=automatic] = const<i32>(1);
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_o]], reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(32))));
// DEFAULT-NEXT:             condition: le<u64>(read<u64>(%[[VALUE_o]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(32))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_o]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u64 [synthetic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE1]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4096))));
// DEFAULT-NEXT:                 write<u64>(%[[VALUE_o]], read<u64>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_u:[0-9]+]] u: ptr<@type[[TYPE_H]]> [storage=automatic];
// DEFAULT-NEXT:                     write<ptr<@type[[TYPE_H]]>>(%[[VALUE_u]], int_to_ptr<ptr<@type[[TYPE_H]]>, reason=explicit>(add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(4194304))), read<u64>(%[[VALUE_o]]))));
// DEFAULT-NEXT:                     if ne<i32>(read<i32, volatile>(%[[VALUE_t]]), const<i32>(0))
// DEFAULT-NEXT:                         {
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE_H]]>>(%[[VALUE_h]], read<ptr<@type[[TYPE_H]]>>(%[[VALUE_u]]));
// DEFAULT-NEXT:                             break %[[VALUE0]];
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         if eq<ptr<@type[[TYPE_H]]>>(read<ptr<@type[[TYPE_H]]>>(%[[VALUE_h]]), null<ptr<@type[[TYPE_H]]>>)
// DEFAULT-NEXT:             return const<i32>(0);
// DEFAULT-NEXT:         let %[[VALUE_tt:[0-9]+]] tt: ptr<u32> [storage=automatic] = addr_of<ptr<u32>>(field1(deref(read<ptr<@type[[TYPE_H]]>>(%[[VALUE_h]]))));
// DEFAULT-NEXT:         if ne<u64>(ptr_to_int<u64, reason=explicit>(read<ptr<u32>>(%[[VALUE_tt]])), add<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(add<i32, overflow=ub>(const<i32>(4194304), const<i32>(32)))), const<u64>(4)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
