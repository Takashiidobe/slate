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
// DEFAULT-NEXT:     type @type[[TYPE_Point:[0-9]+]] Point = struct {
// DEFAULT-NEXT:         field0 x: i32;
// DEFAULT-NEXT:         field1 y: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_MyInt:[0-9]+]] MyInt = i32;
// DEFAULT-NEXT:     global %[[VALUE_global_point:[0-9]+]] global_point: @type[[TYPE_Point]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_global_array:[0-9]+]] global_array: array<i32, 4> [storage=static] [align=16] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_compute:[0-9]+]] @compute(%[[VALUE_point_ptr:[0-9]+]] point_ptr: ptr<@type[[TYPE_Point]]>, %[[VALUE_a:[0-9]+]] a: i32, %[[VALUE_b:[0-9]+]] b: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE3]]));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = mul<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE7:[0-9]+]]: i32 [synthetic] = div<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE6]]), const<i32>(3));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE7]]));
// DEFAULT-NEXT:         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = rem<i32, by_zero=ub, min_by_neg_one=ub>(read<i32>(%[[VALUE8]]), const<i32>(4));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = and<i32>(read<i32>(%[[VALUE10]]), const<i32>(255));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:         let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE13:[0-9]+]]: i32 [synthetic] = or<i32>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:         let %[[VALUE14:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = xor<i32>(read<i32>(%[[VALUE14]]), const<i32>(2));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE15]]));
// DEFAULT-NEXT:         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE17:[0-9]+]]: i32 [synthetic] = shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:         let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE19:[0-9]+]]: i32 [synthetic] = shr<i32, amount_out_of_range=ub, fill=sign_extend>(read<i32>(%[[VALUE18]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], conditional<i32>(gt<i32>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])));
// DEFAULT-NEXT:         read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_Point]]>>(%[[VALUE_point_ptr]]))), read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE_Point]]>>(%[[VALUE_point_ptr]]))), read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_global_array]]), const<i32>(0))), read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         let %[[VALUE20:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE21:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE20]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE21]]));
// DEFAULT-NEXT:         let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE23:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:         let %[[VALUE24:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE25:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE24]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE25]]));
// DEFAULT-NEXT:         let %[[VALUE26:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_a]]);
// DEFAULT-NEXT:         let %[[VALUE27:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE26]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_a]], read<i32>(%[[VALUE27]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(deref(addr_of<ptr<i32>>(%[[VALUE_a]]))));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], read<i32>(%[[VALUE_b]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_b]], reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(4))));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(%[[VALUE_a]]), read<i32>(%[[VALUE_b]])), read<i32>(field0(deref(read<ptr<@type[[TYPE_Point]]>>(%[[VALUE_point_ptr]]))))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(%[[VALUE_global_array]]), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         return call<i32, signature=fn(ptr<@type[[TYPE_Point]]>, i32, i32) -> i32>(%[[VALUE_compute]], addr_of<ptr<@type[[TYPE_Point]]>>(%[[VALUE_global_point]]), const<i32>(1), const<i32>(2));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
