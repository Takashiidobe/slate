struct VEC_char_base {
  unsigned num;
  unsigned alloc;
  union {
    short vec[1];
    struct {
      int i;
      int j;
      int k;
    } a;
  } u;
};

short __attribute__((noinline)) foo(struct VEC_char_base *p, int i) {
  short *q;
  p->u.vec[i] = 0;
  q           = &p->u.vec[16];
  *q          = 1;
  return p->u.vec[i];
}

extern void  abort(void);
extern void *malloc(__SIZE_TYPE__);

int main() {
  struct VEC_char_base *p = malloc(sizeof(struct VEC_char_base) + 256);
  if (foo(p, 16) != 1)
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
// DEFAULT-NEXT:     type @type[[TYPE_VEC_char_base:[0-9]+]] VEC_char_base = struct {
// DEFAULT-NEXT:         field0 num: u32;
// DEFAULT-NEXT:         field1 alloc: u32;
// DEFAULT-NEXT:         field2 u: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:     } [size=20, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = union {
// DEFAULT-NEXT:         field0 vec: array<i16, 1>;
// DEFAULT-NEXT:         field1 a: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = struct {
// DEFAULT-NEXT:         field0 i: i32;
// DEFAULT-NEXT:         field1 j: i32;
// DEFAULT-NEXT:         field2 k: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_VEC_char_base]]>, %[[VALUE_i:[0-9]+]] i: i32) -> i16 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_q:[0-9]+]] q: ptr<i16> [storage=automatic];
// DEFAULT-NEXT:         write<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1)>(field0(field2(deref(read<ptr<@type[[TYPE_VEC_char_base]]>>(%[[VALUE_p]]))))), read<i32>(%[[VALUE_i]]))), truncate<i16, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         write<ptr<i16>>(%[[VALUE_q]], addr_of<ptr<i16>>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1)>(field0(field2(deref(read<ptr<@type[[TYPE_VEC_char_base]]>>(%[[VALUE_p]]))))), const<i32>(16)))));
// DEFAULT-NEXT:         write<i16>(deref(read<ptr<i16>>(%[[VALUE_q]])), truncate<i16, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         return read<i16>(deref(ptr_offset<ptr<i16>, subtract=false, element=i16, overflow=ub>(array_decay<ptr<i16>, length=Some(1)>(field0(field2(deref(read<ptr<@type[[TYPE_VEC_char_base]]>>(%[[VALUE_p]]))))), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE0:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_VEC_char_base]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_VEC_char_base]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], add<u64, overflow=wrap>(const<u64>(20), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(256))))));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(call<i16, signature=fn(ptr<@type[[TYPE_VEC_char_base]]>, i32) -> i16>(%[[VALUE_foo]], read<ptr<@type[[TYPE_VEC_char_base]]>>(%[[VALUE_p_2]]), const<i32>(16))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
