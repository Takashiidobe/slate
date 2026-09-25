struct Point {
  int x;
  int y;
};

typedef int MyInt;

struct Point global_point;
int global_array[4];

int compute(struct Point *point_ptr, int a, int b) {
  a += b;
  a -= 1;
  a *= 2;
  a /= 3;
  a %= 4;
  a &= 255;
  a |= 1;
  a ^= 2;
  a <<= 1;
  a >>= 1;
  b = (a > b) ? a : b;
  b = (a, b);
  point_ptr->x = a;
  (*point_ptr).y = b;
  global_array[0] = a;
  a++;
  a--;
  ++a;
  --a;
  b = *&a;
  b = (int)a;
  b = (MyInt)b;
  b = _Alignof(int);
  return a + b + point_ptr->x + global_array[0];
}

int main(void) {
  return compute(&global_point, 1, 2);
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
// DEFAULT-NEXT:     type @type0 Point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type1 MyInt = i32;
// DEFAULT-NEXT:     global %2 global_point: @type0 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %3 global_array: array<i32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %4 @compute(%5 point_ptr: ptr<@type0>, %6 a: i32, %7 b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %9: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %10: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%9), read<i32>(%7));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%10));
// DEFAULT-NEXT:         let %11: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %12: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%11), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%12));
// DEFAULT-NEXT:         let %13: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %14: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%13), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%14));
// DEFAULT-NEXT:         let %15: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %16: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%15), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%16));
// DEFAULT-NEXT:         let %17: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %18: i32 [synthetic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%17), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%18));
// DEFAULT-NEXT:         let %19: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %20: i32 [synthetic] = and<i32>(read<i32>(%19), const<i32>(255));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%20));
// DEFAULT-NEXT:         let %21: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %22: i32 [synthetic] = or<i32>(read<i32>(%21), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%22));
// DEFAULT-NEXT:         let %23: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %24: i32 [synthetic] = xor<i32>(read<i32>(%23), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%24));
// DEFAULT-NEXT:         let %25: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %26: i32 [synthetic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%26));
// DEFAULT-NEXT:         let %27: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %28: i32 [synthetic] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%28));
// DEFAULT-NEXT:         write<i32>(%7, conditional<i32>(gt<i32>(read<i32>(%6), read<i32>(%7)), read<i32>(%6), read<i32>(%7)));
// DEFAULT-NEXT:         read<i32>(%6);
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%7));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type0>>(%5))), read<i32>(%6));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type0>>(%5))), read<i32>(%7));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%3), const<i32>(0))), read<i32>(%6));
// DEFAULT-NEXT:         let %29: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %30: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%29), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%30));
// DEFAULT-NEXT:         let %31: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %32: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%31), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%32));
// DEFAULT-NEXT:         let %33: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %34: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%33), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%34));
// DEFAULT-NEXT:         let %35: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:         let %36: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%35), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%6, read<i32>(%36));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(deref(addr_of<ptr<i32>>(%6))));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%6));
// DEFAULT-NEXT:         write<i32>(%7, read<i32>(%7));
// DEFAULT-NEXT:         write<i32>(%7, reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%6), read<i32>(%7)), read<i32>(field0(deref(read<ptr<@type0>>(%5))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%3), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<@type0>, i32, i32) -> i32>(%4, addr_of<ptr<@type0>>(%2), const<i32>(1), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
