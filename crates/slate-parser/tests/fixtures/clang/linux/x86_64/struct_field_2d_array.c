#include <stdio.h>

struct table {
  char rows[4][3];
};

struct cube {
  int v[2][3][4];
};

static void fill(struct table *t) {
  for (int i = 0; i < 4; i++) {
    t->rows[i][0] = (char)('a' + i);
    t->rows[i][1] = (char)('0' + i);
    t->rows[i][2] = '\0';
  }
}

static void fill_via_ptr(struct table *t, int i) {
  char (*row)[3]  = t->rows;
  (row + i)[0][0] = 'X';
}

static void fill_cube(struct cube *c) {
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      for (int k = 0; k < 4; k++) {
        c->v[i][j][k] = i * 100 + j * 10 + k;
      }
    }
  }
}

static int sum_cube_via_ptr(struct cube *c) {
  int (*plane)[4] = c->v[1];
  int total       = 0;
  for (int j = 0; j < 3; j++) {
    for (int k = 0; k < 4; k++) {
      total += (plane + j)[0][k];
    }
  }
  return total;
}

int main(void) {
  struct table t;
  fill(&t);
  fill_via_ptr(&t, 2);

  for (int i = 0; i < 4; i++) {
    printf("%s\n", t.rows[i]);
  }

  struct cube c;
  fill_cube(&c);
  printf("%d %d %d\n", c.v[0][0][0], c.v[1][2][3], sum_cube_via_ptr(&c));
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
// DEFAULT-NEXT:     type @type[[TYPE_table:[0-9]+]] table = struct {
// DEFAULT-NEXT:         field0 rows: array<array<i8, 3>, 4>;
// DEFAULT-NEXT:     } [size=12, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_cube:[0-9]+]] cube = struct {
// DEFAULT-NEXT:         field0 v: array<array<array<i32, 4>, 3>, 2>;
// DEFAULT-NEXT:     } [size=96, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fill:[0-9]+]] @fill(%[[VALUE_t:[0-9]+]] t: ptr<@type[[TYPE_table]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE0:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_table]]>>(%[[VALUE_t]])))), read<i32>(%[[VALUE_i]])))), const<i32>(0))), truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(const<i32>(97), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_table]]>>(%[[VALUE_t]])))), read<i32>(%[[VALUE_i]])))), const<i32>(1))), truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(const<i32>(48), read<i32>(%[[VALUE_i]]))));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_table]]>>(%[[VALUE_t]])))), read<i32>(%[[VALUE_i]])))), const<i32>(2))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fill_via_ptr:[0-9]+]] @fill_via_ptr(%[[VALUE_t_2:[0-9]+]] t: ptr<@type[[TYPE_table]]>, %[[VALUE_i_2:[0-9]+]] i: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_row:[0-9]+]] row: ptr<array<i8, 3>> [storage=automatic] = array_decay<ptr<array<i8, 3>>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_table]]>>(%[[VALUE_t_2]]))));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(read<ptr<array<i8, 3>>>(%[[VALUE_row]]), read<i32>(%[[VALUE_i_2]])), const<i32>(0)))), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(88)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_fill_cube:[0-9]+]] @fill_cube(%[[VALUE_c:[0-9]+]] c: ptr<@type[[TYPE_cube]]>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %[[VALUE3:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_3:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_3]]), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_3]]);
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_3]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE6:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_j]]), const<i32>(3))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 for %[[VALUE9:[0-9]+]]
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         let %[[VALUE_k:[0-9]+]] k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%[[VALUE_k]]), const<i32>(4))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k]]);
// DEFAULT-NEXT:                                         let %[[VALUE11:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%[[VALUE_k]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 4>, 3>>, subtract=false, element=array<array<i32, 4>, 3>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 3>>, length=Some(2)>(field0(deref(read<ptr<@type[[TYPE_cube]]>>(%[[VALUE_c]])))), read<i32>(%[[VALUE_i_3]])))), read<i32>(%[[VALUE_j]])))), read<i32>(%[[VALUE_k]]))), add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_i_3]]), const<i32>(100)), mul<i32, overflow=ub>(read<i32>(%[[VALUE_j]]), const<i32>(10))), read<i32>(%[[VALUE_k]])));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_sum_cube_via_ptr:[0-9]+]] @sum_cube_via_ptr(%[[VALUE_c_2:[0-9]+]] c: ptr<@type[[TYPE_cube]]>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_plane:[0-9]+]] plane: ptr<array<i32, 4>> [storage=automatic] = array_decay<ptr<array<i32, 4>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 4>, 3>>, subtract=false, element=array<array<i32, 4>, 3>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 3>>, length=Some(2)>(field0(deref(read<ptr<@type[[TYPE_cube]]>>(%[[VALUE_c_2]])))), const<i32>(1))));
// DEFAULT-NEXT:         let %[[VALUE_total:[0-9]+]] total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %[[VALUE12:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_j_2]]), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_2]]);
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %[[VALUE15:[0-9]+]]
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %[[VALUE_k_2:[0-9]+]] k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%[[VALUE_k_2]]), const<i32>(4))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_k_2]]);
// DEFAULT-NEXT:                             let %[[VALUE17:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%[[VALUE_k_2]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %[[VALUE18:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:                                 let %[[VALUE19:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE18]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(read<ptr<array<i32, 4>>>(%[[VALUE_plane]]), read<i32>(%[[VALUE_j_2]])), const<i32>(0)))), read<i32>(%[[VALUE_k_2]])))));
// DEFAULT-NEXT:                                 write<i32>(%[[VALUE_total]], read<i32>(%[[VALUE19]]));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%[[VALUE_total]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_t_3:[0-9]+]] t: @type[[TYPE_table]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_table]]>) -> void>(%[[VALUE_fill]], addr_of<ptr<@type[[TYPE_table]]>>(%[[VALUE_t_3]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_table]]>, i32) -> void>(%[[VALUE_fill_via_ptr]], addr_of<ptr<@type[[TYPE_table]]>>(%[[VALUE_t_3]]), const<i32>(2));
// DEFAULT-NEXT:         for %[[VALUE20:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_4:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_4]]), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_4]]);
// DEFAULT-NEXT:                 let %[[VALUE22:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE21]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_4]], read<i32>(%[[VALUE22]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%[[VALUE_str]])), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(4)>(field0(%[[VALUE_t_3]])), read<i32>(%[[VALUE_i_4]])))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %[[VALUE_c_3:[0-9]+]] c: @type[[TYPE_cube]] [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_cube]]>) -> void>(%[[VALUE_fill_cube]], addr_of<ptr<@type[[TYPE_cube]]>>(%[[VALUE_c_3]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>,
// DEFAULT-SAME: length=Some(10)>(%[[VALUE_str_2]])), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>,
// DEFAULT-SAME: length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 4>, 3>>, subtract=false, element=array<array<i32, 4>, 3>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 3>>,
// DEFAULT-SAME: length=Some(2)>(field0(%[[VALUE_c_3]])), const<i32>(0)))), const<i32>(0)))), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32,
// DEFAULT-SAME: overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 4>, 3>>, subtract=false, element=array<array<i32, 4>, 3>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 3>>,
// DEFAULT-SAME: length=Some(2)>(field0(%[[VALUE_c_3]])), const<i32>(1)))), const<i32>(2)))), const<i32>(3)))), call<i32,
// DEFAULT-SAME: signature=fn(ptr<@type[[TYPE_cube]]>) ->
// DEFAULT-SAME: i32>(%[[VALUE_sum_cube_via_ptr]],
// DEFAULT-SAME: addr_of<ptr<@type[[TYPE_cube]]>>(%[[VALUE_c_3]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
