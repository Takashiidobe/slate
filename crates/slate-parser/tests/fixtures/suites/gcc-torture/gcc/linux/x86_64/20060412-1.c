extern void abort(void);

struct S {
  long o;
};

struct T {
  long     o;
  struct S m[82];
};

struct T t;

int main() {
  struct S *p, *q;

  p = (struct S *)&t;
  p = &((struct T *)p)->m[0];
  q = p + 82;
  while (--q > p)
    q->o = -1;
  q->o = 0;

  if (q > p)
    abort();
  if (q - p > 0)
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 o: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 o: i64;
// DEFAULT-NEXT:         field1 m: array<@type[[TYPE_S]], 82>;
// DEFAULT-NEXT:     } [size=664, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: @type[[TYPE_T]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<@type[[TYPE_S]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(addr_of<ptr<@type[[TYPE_T]]>>(%[[VALUE_t]])));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]], addr_of<ptr<@type[[TYPE_S]]>>(deref(ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(array_decay<ptr<@type[[TYPE_S]]>, length=Some(82)>(field1(deref(pointer_cast<ptr<@type[[TYPE_T]]>, reason=explicit>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]))))), const<i32>(0)))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_q]], ptr_offset<ptr<@type[[TYPE_S]]>, subtract=false, element=@type[[TYPE_S]], overflow=ub>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]), const<i32>(82)));
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: ptr<@type[[TYPE_S]]> [synthetic] = read<ptr<@type[[TYPE_S]]>>(%[[VALUE_q]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: ptr<@type[[TYPE_S]]> [synthetic] = ptr_offset<ptr<@type[[TYPE_S]]>, subtract=true, element=@type[[TYPE_S]], overflow=ub>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<@type[[TYPE_S]]>>(%[[VALUE_q]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE2]]));
// DEFAULT-NEXT:             yield gt<ptr<@type[[TYPE_S]]>>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE2]]), read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             write<i64>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_q]]))), widen<i64, reason=assign>(neg<i32, overflow=ub>(const<i32>(1))));
// DEFAULT-NEXT:         write<i64>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_q]]))), widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         if gt<ptr<@type[[TYPE_S]]>>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_q]]), read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]]))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if gt<i64>(ptr_diff<i64, element=@type[[TYPE_S]], same_array=required, overflow=ub>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_q]]), read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
