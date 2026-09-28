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
// DEFAULT-NEXT:     type @type0 table = struct {
// DEFAULT-NEXT:         field0 rows: array<array<i8, 3>, 4>;
// DEFAULT-NEXT:     } [size=12, align=1, offsets=[0]];
// DEFAULT-NEXT:     type @type1 cube = struct {
// DEFAULT-NEXT:         field0 v: array<array<array<i32, 4>, 3>, 2>;
// DEFAULT-NEXT:     } [size=96, align=4, offsets=[0]];
// DEFAULT-NEXT:     global %34 .str34: array<i8, 4> [storage=static] = code_units<array<i8, 4>>([37, 115, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %35 .str35: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 100, 32, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @printf(%26 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %4 @fill(%5 t: ptr<@type0>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %27
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %6 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%6), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %36: i32 [synthetic] = read<i32>(%6);
// DEFAULT-NEXT:                 let %37: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%36), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%6, read<i32>(%37));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%5)))), read<i32>(%6)))), const<i32>(0))), truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(const<i32>(97), read<i32>(%6))));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%5)))), read<i32>(%6)))), const<i32>(1))), truncate<i8, reason=explicit, fits=unknown>(add<i32, overflow=ub>(const<i32>(48), read<i32>(%6))));
// DEFAULT-NEXT:                     write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%5)))), read<i32>(%6)))), const<i32>(2))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @fill_via_ptr(%8 t: ptr<@type0>, %9 i: i32) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %10 row: ptr<array<i8, 3>> [storage=automatic] = array_decay<ptr<array<i8, 3>>, length=Some(4)>(field0(deref(read<ptr<@type0>>(%8))));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(read<ptr<array<i8, 3>>>(%10), read<i32>(%9)), const<i32>(0)))), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(88)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @fill_cube(%12 c: ptr<@type1>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         for %28
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %13 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%13), const<i32>(2))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %38: i32 [synthetic] = read<i32>(%13);
// DEFAULT-NEXT:                 let %39: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%38), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%13, read<i32>(%39));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %29
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %14 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%14), const<i32>(3))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %40: i32 [synthetic] = read<i32>(%14);
// DEFAULT-NEXT:                             let %41: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%40), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%14, read<i32>(%41));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 for %30
// DEFAULT-NEXT:                                     init:
// DEFAULT-NEXT:                                         let %15 k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                                     condition: lt<i32>(read<i32>(%15), const<i32>(4))
// DEFAULT-NEXT:                                     increment: {
// DEFAULT-NEXT:                                         let %42: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:                                         let %43: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%42), const<i32>(1));
// DEFAULT-NEXT:                                         write<i32>(%15, read<i32>(%43));
// DEFAULT-NEXT:                                         yield void;
// DEFAULT-NEXT:                                     }
// DEFAULT-NEXT:                                     body:
// DEFAULT-NEXT:                                         {
// DEFAULT-NEXT:                                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 4>, 3>>, subtract=false, element=array<array<i32, 4>, 3>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 3>>, length=Some(2)>(field0(deref(read<ptr<@type1>>(%12)))), read<i32>(%13)))), read<i32>(%14)))), read<i32>(%15))), add<i32, overflow=ub>(add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%13), const<i32>(100)), mul<i32, overflow=ub>(read<i32>(%14), const<i32>(10))), read<i32>(%15)));
// DEFAULT-NEXT:                                         }
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @sum_cube_via_ptr(%17 c: ptr<@type1>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %18 plane: ptr<array<i32, 4>> [storage=automatic] = array_decay<ptr<array<i32, 4>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 4>, 3>>, subtract=false, element=array<array<i32, 4>, 3>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 3>>, length=Some(2)>(field0(deref(read<ptr<@type1>>(%17)))), const<i32>(1))));
// DEFAULT-NEXT:         let %19 total: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:         for %31
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %20 j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%20), const<i32>(3))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %44: i32 [synthetic] = read<i32>(%20);
// DEFAULT-NEXT:                 let %45: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%44), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%20, read<i32>(%45));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     for %32
// DEFAULT-NEXT:                         init:
// DEFAULT-NEXT:                             let %21 k: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                         condition: lt<i32>(read<i32>(%21), const<i32>(4))
// DEFAULT-NEXT:                         increment: {
// DEFAULT-NEXT:                             let %46: i32 [synthetic] = read<i32>(%21);
// DEFAULT-NEXT:                             let %47: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%46), const<i32>(1));
// DEFAULT-NEXT:                             write<i32>(%21, read<i32>(%47));
// DEFAULT-NEXT:                             yield void;
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                         body:
// DEFAULT-NEXT:                             {
// DEFAULT-NEXT:                                 let %48: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:                                 let %49: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%48), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(read<ptr<array<i32, 4>>>(%18), read<i32>(%20)), const<i32>(0)))), read<i32>(%21)))));
// DEFAULT-NEXT:                                 write<i32>(%19, read<i32>(%49));
// DEFAULT-NEXT:                             }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return read<i32>(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %23 t: @type0 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%4, addr_of<ptr<@type0>>(%23));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>, i32) -> void>(%7, addr_of<ptr<@type0>>(%23), const<i32>(2));
// DEFAULT-NEXT:         for %33
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %24 i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%24), const<i32>(4))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %50: i32 [synthetic] = read<i32>(%24);
// DEFAULT-NEXT:                 let %51: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%50), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%24, read<i32>(%51));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(4)>(%34)), array_decay<ptr<i8>, length=Some(3)>(deref(ptr_offset<ptr<array<i8, 3>>, subtract=false, element=array<i8, 3>, overflow=ub>(array_decay<ptr<array<i8, 3>>, length=Some(4)>(field0(%23)), read<i32>(%24)))));
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         let %25 c: @type1 [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type1>) -> void>(%11, addr_of<ptr<@type1>>(%25));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%1, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%35)), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 4>, 3>>, subtract=false, element=array<array<i32, 4>, 3>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 3>>, length=Some(2)>(field0(%25)), const<i32>(0)))), const<i32>(0)))), const<i32>(0)))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(4)>(deref(ptr_offset<ptr<array<i32, 4>>, subtract=false, element=array<i32, 4>, overflow=ub>(array_decay<ptr<array<i32, 4>>, length=Some(3)>(deref(ptr_offset<ptr<array<array<i32, 4>, 3>>, subtract=false, element=array<array<i32, 4>, 3>, overflow=ub>(array_decay<ptr<array<array<i32, 4>, 3>>, length=Some(2)>(field0(%25)), const<i32>(1)))), const<i32>(2)))), const<i32>(3)))), call<i32, signature=fn(ptr<@type1>) -> i32>(%16, addr_of<ptr<@type1>>(%25)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
