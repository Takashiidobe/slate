typedef struct descriptor_dimension {
  int stride;
  int lbound;
  int ubound;
} descriptor_dimension;
typedef struct {
  int                 *data;
  int                  dtype;
  descriptor_dimension dim[7];
} gfc_array_i4;

void msum_i4(gfc_array_i4 *const retarray, gfc_array_i4 *const array,
             const int *const pdim) {
  int        count[7];
  int        extent[7];
  int       *dest;
  const int *base;
  int        dim;
  int        n;
  int        len;

  dim = (*pdim) - 1;
  len = array->dim[dim].ubound + 1 - array->dim[dim].lbound;

  for (n = 0; n < dim; n++) {
    extent[n] = array->dim[n].ubound + 1 - array->dim[n].lbound;
    count[n]  = 0;
  }

  dest = retarray->data;
  base = array->data;

  do {
    int result = 0;

    for (n = 0; n < len; n++, base++)
      result += *base;
    *dest = result;

    count[0]++;
    dest += 1;
  } while (count[0] != extent[0]);
}

int main() {
  int          rdata[3];
  int          adata[9];
  gfc_array_i4 retarray = {rdata, 265, {{1, 1, 3}}};
  gfc_array_i4 array    = {adata, 266, {{1, 1, 3}, {3, 1, 3}}};
  int          dim      = 2;
  msum_i4(&retarray, &array, &dim);
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
// DEFAULT-NEXT:     type @type0 descriptor_dimension = struct {
// DEFAULT-NEXT:         field0 stride: i32;
// DEFAULT-NEXT:         field1 lbound: i32;
// DEFAULT-NEXT:         field2 ubound: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type1 descriptor_dimension = @type0;
// DEFAULT-NEXT:     type @type2 = struct {
// DEFAULT-NEXT:         field0 data: ptr<i32>;
// DEFAULT-NEXT:         field1 dtype: i32;
// DEFAULT-NEXT:         field2 dim: array<@type0, 7>;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type3 gfc_array_i4 = @type2;
// DEFAULT-NEXT:     fn %4 @msum_i4(%5 retarray: ptr<@type2> [const], %6 array: ptr<@type2> [const], %7 pdim: ptr<const i32> [const]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %8 count: array<i32, 7> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %9 extent: array<i32, 7> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %10 dest: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %11 base: ptr<const i32> [storage=automatic];
// DEFAULT-NEXT:         let %12 dim: i32 [storage=automatic];
// DEFAULT-NEXT:         let %13 n: i32 [storage=automatic];
// DEFAULT-NEXT:         let %14 len: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%12, sub<i32, overflow=ub>(read<i32>(deref(read<ptr<const i32>>(%7))), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%14, sub<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field2(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(7)>(field2(deref(read<ptr<@type2>>(%6)))), read<i32>(%12))))), const<i32>(1)), read<i32>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(7)>(field2(deref(read<ptr<@type2>>(%6)))), read<i32>(%12)))))));
// DEFAULT-NEXT:         for %22
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), read<i32>(%12))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %25: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %26: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%25), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%26));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%9), read<i32>(%13))), sub<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field2(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(7)>(field2(deref(read<ptr<@type2>>(%6)))), read<i32>(%13))))), const<i32>(1)), read<i32>(field1(deref(ptr_offset<ptr<@type0>, subtract=false, element=@type0, overflow=ub>(array_decay<ptr<@type0>, length=Some(7)>(field2(deref(read<ptr<@type2>>(%6)))), read<i32>(%13)))))));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%8), read<i32>(%13))), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<ptr<i32>>(%10, read<ptr<i32>>(field0(deref(read<ptr<@type2>>(%5)))));
// DEFAULT-NEXT:         write<ptr<const i32>>(%11, pointer_cast<ptr<const i32>, reason=assign>(read<ptr<i32>>(field0(deref(read<ptr<@type2>>(%6))))));
// DEFAULT-NEXT:         do %23
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %15 result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 for %24
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%13, const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%13), read<i32>(%14))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %27: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                         let %28: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%27), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%13, read<i32>(%28));
// DEFAULT-NEXT:                         let %29: ptr<const i32> [synthetic] = read<ptr<const i32>>(%11);
// DEFAULT-NEXT:                         let %30: ptr<const i32> [synthetic] = ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%29), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<const i32>>(%11, read<ptr<const i32>>(%30));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         let %31: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                         let %32: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%31), read<i32>(deref(read<ptr<const i32>>(%11))));
// DEFAULT-NEXT:                         write<i32>(%15, read<i32>(%32));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%10)), read<i32>(%15));
// DEFAULT-NEXT:                 let %33: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%8), const<i32>(0));
// DEFAULT-NEXT:                 let %34: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%33)));
// DEFAULT-NEXT:                 let %35: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%34), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%33)), read<i32>(%35));
// DEFAULT-NEXT:                 let %36: ptr<i32> [synthetic] = read<ptr<i32>>(%10);
// DEFAULT-NEXT:                 let %37: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%36), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%10, read<ptr<i32>>(%37));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%8), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%9), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %17 rdata: array<i32, 3> [storage=automatic];
// DEFAULT-NEXT:         let %18 adata: array<i32, 9> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %19 retarray: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = array_decay<ptr<i32>, length=Some(3)>(%17), field1 = const<i32>(265), field2 = aggregate<array<@type0, 7>, zero_fill=true>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(1), field2 = const<i32>(3))));
// DEFAULT-NEXT:         let %20 array: @type2 [storage=automatic] = aggregate<@type2, zero_fill=false>(field0 = array_decay<ptr<i32>, length=Some(9)>(%18), field1 = const<i32>(266), field2 = aggregate<array<@type0, 7>, zero_fill=true>(index0 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(1), field2 = const<i32>(3)), index1 = aggregate<@type0, zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(1), field2 = const<i32>(3))));
// DEFAULT-NEXT:         let %21 dim: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type2>, ptr<@type2>, ptr<const i32>) -> void>(%4, addr_of<ptr<@type2>>(%19), addr_of<ptr<@type2>>(%20), pointer_cast<ptr<const i32>, reason=arg>(addr_of<ptr<i32>>(%21)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
