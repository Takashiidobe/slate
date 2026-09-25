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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 t = struct {
// DEFAULT-NEXT:         field0 a: u32 : 16;
// DEFAULT-NEXT:         field1 b: u32 : 8;
// DEFAULT-NEXT:         field2 c: u32 : 8;
// DEFAULT-NEXT:         field3 d: array<i64, 4>;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 2, 3, 8], bit_offsets=[Some(0), Some(16), Some(24), None], bit_units=[(0, 4)], field_units=[Some(0), Some(0), Some(0), None]];
// DEFAULT-NEXT:     type @type2 T = ptr<@type1>;
// DEFAULT-NEXT:     type @type3 = struct {
// DEFAULT-NEXT:         field0 r: array<i64, 3>;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type4 U = @type3;
// DEFAULT-NEXT:     global %19 t: @type1 [storage=static] = aggregate<@type1, zero_fill=false>(field0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(26)), field1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(0)), field3 = aggregate<array<i64, 4>, zero_fill=false>(index0 = widen<i64, reason=assign>(const<i32>(0)), index1 = widen<i64, reason=assign>(const<i32>(21)), index2 = widen<i64, reason=assign>(const<i32>(22)), index3 = widen<i64, reason=assign>(const<i32>(23)))) [linkage=external];
// DEFAULT-NEXT:     fn %1 @memcpy(%21 <unnamed>: ptr<void> [restrict], %22 <unnamed>: ptr<const void> [restrict], %23 <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @exit(%24 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %8 @bar(%17 d: @type3, %18 m: u32) -> ptr<@type1> [linkage=external] [abi=sysv64(native_c, scalar) -> scalar] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%17)), const<i32>(0)))), widen<i64, reason=usual_arith>(const<i32>(21))), ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%17)), const<i32>(1)))), widen<i64, reason=usual_arith>(const<i32>(22)))), ne<i64>(read<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%17)), const<i32>(2)))), widen<i64, reason=usual_arith>(const<i32>(23))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return null<ptr<@type1>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %9 @foo(%10 x: ptr<@type1>) -> ptr<@type1> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 d: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %12 u: @type3 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type3>>(%12)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i64>>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(4)>(field3(deref(read<ptr<@type1>>(%10)))), const<i32>(1))))), const<u64>(24));
// DEFAULT-NEXT:         write<@type3>(%11, copy<@type3, reason=assign>(read<@type3>(%12)));
// DEFAULT-NEXT:         return call<ptr<@type1>, signature=fn(@type3, u32) -> ptr<@type1>, abi=sysv64(native_c, scalar) -> scalar>(%8, copy<@type3, reason=arg>(read<@type3>(%11)), reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(deref(read<ptr<@type1>>(%10)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @baz(%14 x: ptr<@type1>) -> ptr<@type1> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15 d: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %16 u: @type3 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%15)), const<i32>(0))), const<i64>(4886718345));
// DEFAULT-NEXT:         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%15)), const<i32>(1))), const<i64>(68414056839));
// DEFAULT-NEXT:         write<i64>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(3)>(field0(%15)), const<i32>(2))), const<i64>(46118400291));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%1, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type3>>(%16)), pointer_cast<ptr<const void>, reason=arg>(addr_of<ptr<i64>>(deref(ptr_offset<ptr<i64>, subtract=false, element=i64, overflow=ub>(array_decay<ptr<i64>, length=Some(4)>(field3(deref(read<ptr<@type1>>(%14)))), const<i32>(1))))), const<u64>(24));
// DEFAULT-NEXT:         write<@type3>(%15, copy<@type3, reason=assign>(read<@type3>(%16)));
// DEFAULT-NEXT:         return call<ptr<@type1>, signature=fn(@type3, u32) -> ptr<@type1>, abi=sysv64(native_c, scalar) -> scalar>(%8, copy<@type3, reason=arg>(read<@type3>(%15)), reinterpret<u32, reason=arg, fits=unknown>(reinterpret<i32, reason=promotion, fits=unknown>(read<u32>(bitfield1<unit=0, bytes=0..4, bits=16..24>(deref(read<ptr<@type1>>(%14)))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(ptr<@type1>) -> ptr<@type1>>(%13, addr_of<ptr<@type1>>(%19));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(ptr<@type1>) -> ptr<@type1>>(%9, addr_of<ptr<@type1>>(%19));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
