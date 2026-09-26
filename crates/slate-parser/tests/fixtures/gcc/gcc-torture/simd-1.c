/* Origin: Aldy Hernandez <aldyh@redhat.com>

   Purpose: Test generic SIMD support.  This test should work
   regardless of if the target has SIMD instructions.
*/

void abort(void);
void exit(int);

typedef int __attribute__((mode(SI))) __attribute__((vector_size(16))) vecint;
typedef int __attribute__((mode(SI)))                                  siint;

vecint i = {150, 100, 150, 200};
vecint j = {10, 13, 20, 30};
vecint k;

union {
  vecint v;
  siint  i[4];
} res;

/* This should go away once we can use == and != on vector types.  */
void verify(siint a1, siint a2, siint a3, siint a4, siint b1, siint b2,
            siint b3, siint b4) {
  if (a1 != b1 || a2 != b2 || a3 != b3 || a4 != b4)
    abort();
}

int main() {
  k     = i + j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 160, 113, 170, 230);

  k     = i * j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 1500, 1300, 3000, 6000);

  k     = i / j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 15, 7, 7, 6);

  k     = i & j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 2, 4, 20, 8);

  k     = i | j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 158, 109, 150, 222);

  k     = i ^ j;
  res.v = k;

  verify(res.i[0], res.i[1], res.i[2], res.i[3], 156, 105, 130, 214);

  k     = -i;
  res.v = k;
  verify(res.i[0], res.i[1], res.i[2], res.i[3], -150, -100, -150, -200);

  k     = ~i;
  res.v = k;
  verify(res.i[0], res.i[1], res.i[2], res.i[3], -151, -101, -151, -201);

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
// DEFAULT-NEXT:     type @type0 vecint = vector<i32, 4>;
// DEFAULT-NEXT:     type @type1 siint = i32;
// DEFAULT-NEXT:     type @type2 = union {
// DEFAULT-NEXT:         field0 v: vector<i32, 4>;
// DEFAULT-NEXT:         field1 i: array<i32, 4>;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 0]];
// DEFAULT-NEXT:     global %4 i: vector<i32, 4> [storage=static] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(150), index1 = const<i32>(100), index2 = const<i32>(150), index3 = const<i32>(200)) [linkage=external];
// DEFAULT-NEXT:     global %5 j: vector<i32, 4> [storage=static] = aggregate<vector<i32, 4>, zero_fill=false>(index0 = const<i32>(10), index1 = const<i32>(13), index2 = const<i32>(20), index3 = const<i32>(30)) [linkage=external];
// DEFAULT-NEXT:     global %6 k: vector<i32, 4> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 res: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %1 @exit(%19 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @verify(%10 a1: i32, %11 a2: i32, %12 a3: i32, %13 a4: i32, %14 b1: i32, %15 b2: i32, %16 b3: i32, %17 b4: i32) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<i32>(read<i32>(%10), read<i32>(%14)), ne<i32>(read<i32>(%11), read<i32>(%15))), ne<i32>(read<i32>(%12), read<i32>(%16))), ne<i32>(read<i32>(%13), read<i32>(%17)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         write<vector<i32, 4>>(%6, add<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%4), read<vector<i32, 4>>(%5)));
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%8), read<vector<i32, 4>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32) -> void>(%9, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(3)))), const<i32>(160), const<i32>(113), const<i32>(170), const<i32>(230));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%6, mul<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%4), read<vector<i32, 4>>(%5)));
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%8), read<vector<i32, 4>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32) -> void>(%9, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(3)))), const<i32>(1500), const<i32>(1300), const<i32>(3000), const<i32>(6000));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%6, div<vector<i32, 4>, elementwise=true, by_zero=ub, min_by_neg_one=ub>(read<vector<i32, 4>>(%4), read<vector<i32, 4>>(%5)));
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%8), read<vector<i32, 4>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32) -> void>(%9, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(3)))), const<i32>(15), const<i32>(7), const<i32>(7), const<i32>(6));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%6, and<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%4), read<vector<i32, 4>>(%5)));
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%8), read<vector<i32, 4>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32) -> void>(%9, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(3)))), const<i32>(2), const<i32>(4), const<i32>(20), const<i32>(8));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%6, or<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%4), read<vector<i32, 4>>(%5)));
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%8), read<vector<i32, 4>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32) -> void>(%9, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(3)))), const<i32>(158), const<i32>(109), const<i32>(150), const<i32>(222));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%6, xor<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%4), read<vector<i32, 4>>(%5)));
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%8), read<vector<i32, 4>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32) -> void>(%9, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(3)))), const<i32>(156), const<i32>(105), const<i32>(130), const<i32>(214));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%6, neg<vector<i32, 4>, elementwise=true, overflow=wrap>(read<vector<i32, 4>>(%4)));
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%8), read<vector<i32, 4>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32) -> void>(%9, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(3)))), neg<i32, overflow=ub>(const<i32>(150)), neg<i32, overflow=ub>(const<i32>(100)), neg<i32, overflow=ub>(const<i32>(150)), neg<i32, overflow=ub>(const<i32>(200)));
// DEFAULT-NEXT:         write<vector<i32, 4>>(%6, not<vector<i32, 4>, elementwise=true>(read<vector<i32, 4>>(%4)));
// DEFAULT-NEXT:         write<vector<i32, 4>>(field0(%8), read<vector<i32, 4>>(%6));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32) -> void>(%9, read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(1)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(2)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(field1(%8)), const<i32>(3)))), neg<i32, overflow=ub>(const<i32>(151)), neg<i32, overflow=ub>(const<i32>(101)), neg<i32, overflow=ub>(const<i32>(151)), neg<i32, overflow=ub>(const<i32>(201)));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
