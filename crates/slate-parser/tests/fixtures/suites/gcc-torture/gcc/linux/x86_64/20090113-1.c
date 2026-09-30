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
// DEFAULT-NEXT:     type @type[[TYPE_descriptor_dimension:[0-9]+]] descriptor_dimension = struct {
// DEFAULT-NEXT:         field0 stride: i32;
// DEFAULT-NEXT:         field1 lbound: i32;
// DEFAULT-NEXT:         field2 ubound: i32;
// DEFAULT-NEXT:     } [size=12, align=4, offsets=[0, 4, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_descriptor_dimension_2:[0-9]+]] descriptor_dimension = @type[[TYPE_descriptor_dimension]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 data: ptr<i32>;
// DEFAULT-NEXT:         field1 dtype: i32;
// DEFAULT-NEXT:         field2 dim: array<@type[[TYPE_descriptor_dimension]], 7>;
// DEFAULT-NEXT:     } [size=96, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_gfc_array_i4:[0-9]+]] gfc_array_i4 = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_msum_i4:[0-9]+]] @msum_i4(%[[VALUE_retarray:[0-9]+]] retarray: ptr<@type[[TYPE0]]> [const], %[[VALUE_array:[0-9]+]] array: ptr<@type[[TYPE0]]> [const], %[[VALUE_pdim:[0-9]+]] pdim: ptr<const i32> [const]) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: array<i32, 7> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_extent:[0-9]+]] extent: array<i32, 7> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_dest:[0-9]+]] dest: ptr<i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_base:[0-9]+]] base: ptr<const i32> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_dim:[0-9]+]] dim: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_n:[0-9]+]] n: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_len:[0-9]+]] len: i32 [storage=automatic];
// DEFAULT-NEXT:         write<i32>(%[[VALUE_dim]], sub<i32, overflow=ub>(read<i32>(deref(read<ptr<const i32>>(%[[VALUE_pdim]]))), const<i32>(1)));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_len]], sub<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field2(deref(ptr_offset<ptr<@type[[TYPE_descriptor_dimension]]>, subtract=false, element=@type[[TYPE_descriptor_dimension]], overflow=ub>(array_decay<ptr<@type[[TYPE_descriptor_dimension]]>, length=Some(7)>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_array]])))), read<i32>(%[[VALUE_dim]]))))), const<i32>(1)), read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_descriptor_dimension]]>, subtract=false, element=@type[[TYPE_descriptor_dimension]], overflow=ub>(array_decay<ptr<@type[[TYPE_descriptor_dimension]]>, length=Some(7)>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_array]])))), read<i32>(%[[VALUE_dim]])))))));
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_n]], const<i32>(0));
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_n]]), read<i32>(%[[VALUE_dim]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%[[VALUE_extent]]), read<i32>(%[[VALUE_n]]))), sub<i32, overflow=ub>(add<i32, overflow=ub>(read<i32>(field2(deref(ptr_offset<ptr<@type[[TYPE_descriptor_dimension]]>, subtract=false, element=@type[[TYPE_descriptor_dimension]], overflow=ub>(array_decay<ptr<@type[[TYPE_descriptor_dimension]]>, length=Some(7)>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_array]])))), read<i32>(%[[VALUE_n]]))))), const<i32>(1)), read<i32>(field1(deref(ptr_offset<ptr<@type[[TYPE_descriptor_dimension]]>, subtract=false, element=@type[[TYPE_descriptor_dimension]], overflow=ub>(array_decay<ptr<@type[[TYPE_descriptor_dimension]]>, length=Some(7)>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_array]])))), read<i32>(%[[VALUE_n]])))))));
// DEFAULT-NEXT:                     write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%[[VALUE_count]]), read<i32>(%[[VALUE_n]]))), const<i32>(0));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         write<ptr<i32>>(%[[VALUE_dest]], read<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_retarray]])))));
// DEFAULT-NEXT:         write<ptr<const i32>>(%[[VALUE_base]], pointer_cast<ptr<const i32>, reason=assign>(read<ptr<i32>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_array]]))))));
// DEFAULT-NEXT:         do %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_result:[0-9]+]] result: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                 for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_n]], const<i32>(0));
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_n]]), read<i32>(%[[VALUE_len]]))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_n]]);
// DEFAULT-NEXT:                         let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_n]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                         let %[[VALUE7:[0-9]+]]: ptr<const i32> [synthetic] = read<ptr<const i32>>(%[[VALUE_base]]);
// DEFAULT-NEXT:                         let %[[VALUE8:[0-9]+]]: ptr<const i32> [synthetic] = ptr_offset<ptr<const i32>, subtract=false, element=i32, overflow=ub>(read<ptr<const i32>>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                         write<ptr<const i32>>(%[[VALUE_base]], read<ptr<const i32>>(%[[VALUE8]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_result]]);
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE9]]), read<i32>(deref(read<ptr<const i32>>(%[[VALUE_base]]))));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_result]], read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE_dest]])), read<i32>(%[[VALUE_result]]));
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%[[VALUE_count]]), const<i32>(0));
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(deref(read<ptr<i32>>(%[[VALUE11]])));
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(deref(read<ptr<i32>>(%[[VALUE11]])), read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: ptr<i32> [synthetic] = read<ptr<i32>>(%[[VALUE_dest]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: ptr<i32> [synthetic] = ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(read<ptr<i32>>(%[[VALUE14]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<i32>>(%[[VALUE_dest]], read<ptr<i32>>(%[[VALUE15]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%[[VALUE_count]]), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(7)>(%[[VALUE_extent]]), const<i32>(0)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_rdata:[0-9]+]] rdata: array<i32, 3> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_adata:[0-9]+]] adata: array<i32, 9> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_retarray_2:[0-9]+]] retarray: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = array_decay<ptr<i32>, length=Some(3)>(%[[VALUE_rdata]]), field1 = const<i32>(265), field2 = aggregate<array<@type[[TYPE_descriptor_dimension]], 7>, zero_fill=true>(index0 = aggregate<@type[[TYPE_descriptor_dimension]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(1), field2 = const<i32>(3))));
// DEFAULT-NEXT:         let %[[VALUE_array_2:[0-9]+]] array: @type[[TYPE0]] [storage=automatic] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = array_decay<ptr<i32>, length=Some(9)>(%[[VALUE_adata]]), field1 = const<i32>(266), field2 = aggregate<array<@type[[TYPE_descriptor_dimension]], 7>, zero_fill=true>(index0 = aggregate<@type[[TYPE_descriptor_dimension]], zero_fill=false>(field0 = const<i32>(1), field1 = const<i32>(1), field2 = const<i32>(3)), index1 = aggregate<@type[[TYPE_descriptor_dimension]], zero_fill=false>(field0 = const<i32>(3), field1 = const<i32>(1), field2 = const<i32>(3))));
// DEFAULT-NEXT:         let %[[VALUE_dim_2:[0-9]+]] dim: i32 [storage=automatic] = const<i32>(2);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE0]]>, ptr<@type[[TYPE0]]>, ptr<const i32>) -> void>(%[[VALUE_msum_i4]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_retarray_2]]), addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_array_2]]), pointer_cast<ptr<const i32>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_dim_2]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
