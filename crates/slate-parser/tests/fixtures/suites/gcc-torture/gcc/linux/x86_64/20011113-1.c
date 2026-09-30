typedef __SIZE_TYPE__ size_t;
extern void          *memcpy(void *__restrict, const void *__restrict, size_t);
extern void           abort(void);
extern void           exit(int);

typedef struct t {
  unsigned a : 16;
  unsigned b : 8;
  unsigned c : 8;
  long     d[4];
} *T;

typedef struct {
  long r[3];
} U;

T bar(U, unsigned int);

T foo(T x) {
  U d, u;

  memcpy(&u, &x->d[1], sizeof u);
  d = u;
  return bar(d, x->b);
}

T baz(T x) {
  U d, u;

  d.r[0] = 0x123456789;
  d.r[1] = 0xfedcba987;
  d.r[2] = 0xabcdef123;
  memcpy(&u, &x->d[1], sizeof u);
  d = u;
  return bar(d, x->b);
}

T bar(U d, unsigned int m) {
  if (d.r[0] != 21 || d.r[1] != 22 || d.r[2] != 23)
    abort();
  return 0;
}

struct t t = {26, 0, 0, {0, 21, 22, 23}};

int main(void) {
  baz(&t);
  foo(&t);
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_t:[0-9]+]] t = struct {
// DEFAULT-NEXT:         field0 a: u32 : 16;
// DEFAULT-NEXT:         field1 b: u32 : 8;
// DEFAULT-NEXT:         field2 c: u32 : 8;
// DEFAULT-NEXT:         field3 d: array<i64, 4>;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 2, 3, 8], bit_offsets=[Some(0), Some(16), Some(24), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = ptr<@type[[TYPE_t]]>;
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 r: array<i64, 3>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = @type[[TYPE0]];
// DEFAULT-NEXT:     global %[[VALUE_t:[0-9]+]] t: @type[[TYPE_t]] [storage=static] = aggregate<@type[[TYPE_t]], zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(26)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field3 = aggregate<array<i64, 4>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(0)), index1 = widen<i64, reason=assign>(const<i32>(21)), index2 = widen<i64, reason=assign>(const<i32>(22)), index3 = widen<i64, reason=assign>(const<i32>(23)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void> [restrict], %[[VALUE1:[0-9]+]] <unnamed>: ptr<const void> [restrict], %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE3:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_d:[0-9]+]] d: @type[[TYPE0]], %[[VALUE_m:[0-9]+]] m: u32) -> ptr<@type[[TYPE_t]]> [linkage=external] [abi=sysv64(native_c, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%[[VALUE_d]])), const<i32>(0)))), widen<i64, reason=usual_arith>(const<i32>(21))), ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%[[VALUE_d]])), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(22)))), ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%[[VALUE_d]])), const<i32>(2)))), widen<i64, reason=usual_arith>(const<i32>(23))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return null<ptr<@type[[TYPE_t]]>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_x:[0-9]+]] x: ptr<@type[[TYPE_t]]>) -> ptr<@type[[TYPE_t]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d_2:[0-9]+]] d: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_u]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i64>>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(4)>(field3(deref(read<ptr<@type[[TYPE_t]]>>(%[[VALUE_x]])))), const<i32>(1))))), const<u64>(24));
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_d_2]], copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(%[[VALUE_u]])));
// DEFAULT-NEXT:         return call<ptr<@type[[TYPE_t]]>, signature=fn(@type[[TYPE0]], u32) -> ptr<@type[[TYPE_t]]>, abi=sysv64(native_c, scalar) -> scalar>(%[[VALUE_bar]], copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_d_2]])), reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(deref(read<ptr<@type[[TYPE_t]]>>(%[[VALUE_x]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_x_2:[0-9]+]] x: ptr<@type[[TYPE_t]]>) -> ptr<@type[[TYPE_t]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_d_3:[0-9]+]] d: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_u_2:[0-9]+]] u: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%[[VALUE_d_3]])), const<i32>(0))), const<i64>(4886718345));
// DEFAULT-NEXT:         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%[[VALUE_d_3]])), const<i32>(1))), const<i64>(68414056839));
// DEFAULT-NEXT:         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%[[VALUE_d_3]])), const<i32>(2))), const<i64>(46118400291));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_u_2]])), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i64>>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(4)>(field3(deref(read<ptr<@type[[TYPE_t]]>>(%[[VALUE_x_2]])))), const<i32>(1))))), const<u64>(24));
// DEFAULT-NEXT:         write<@type[[TYPE0]]>(%[[VALUE_d_3]], copy<@type[[TYPE0]], reason=assign>(read<@type[[TYPE0]]>(%[[VALUE_u_2]])));
// DEFAULT-NEXT:         return call<ptr<@type[[TYPE_t]]>, signature=fn(@type[[TYPE0]], u32) -> ptr<@type[[TYPE_t]]>, abi=sysv64(native_c, scalar) -> scalar>(%[[VALUE_bar]], copy<@type[[TYPE0]], reason=arg>(read<@type[[TYPE0]]>(%[[VALUE_d_3]])), reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(deref(read<ptr<@type[[TYPE_t]]>>(%[[VALUE_x_2]])))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_t]]>, signature=fn(ptr<@type[[TYPE_t]]>) -> ptr<@type[[TYPE_t]]>>(%[[VALUE_baz]], addr_of<ptr<@type[[TYPE_t]]>>(%[[VALUE_t]]));
// DEFAULT-NEXT:         call<ptr<@type[[TYPE_t]]>, signature=fn(ptr<@type[[TYPE_t]]>) -> ptr<@type[[TYPE_t]]>>(%[[VALUE_foo]], addr_of<ptr<@type[[TYPE_t]]>>(%[[VALUE_t]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
