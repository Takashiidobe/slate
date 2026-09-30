#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  void *super;
  int   name;
  int   size;
} t;
t *f(t *clas, int size) {
  t *child = (t *)malloc(size);
  memcpy(child, clas, clas->size);
  child->super = clas;
  child->name  = 0;
  child->size  = size;
  return child;
}
int main(void) {
  t foo, *bar;
  memset(&foo, 37, sizeof(t));
  foo.size = sizeof(t);
  bar      = f(&foo, sizeof(t));
  if (bar->super != &foo || bar->name != 0 || bar->size != sizeof(t))
    abort();
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 super: ptr<void>;
// DEFAULT-NEXT:         field1 name: i32;
// DEFAULT-NEXT:         field2 size: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type[[TYPE_t:[0-9]+]] t = @type[[TYPE0]];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE___s:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n_2:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f:[0-9]+]] @f(%[[VALUE_clas:[0-9]+]] clas: ptr<@type[[TYPE0]]>, %[[VALUE_size:[0-9]+]] size: i32) -> ptr<@type[[TYPE0]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_child:[0-9]+]] child: ptr<@type[[TYPE0]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE0]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%[[VALUE_size]])))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_child]])), pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_clas]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_clas]])))))));
// DEFAULT-NEXT:         write<ptr<void>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_child]]))), pointer_cast<ptr<void>, reason=assign>(read<ptr<@type[[TYPE0]]>>(%[[VALUE_clas]])));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_child]]))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_child]]))), read<i32>(%[[VALUE_size]]));
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE0]]>>(%[[VALUE_child]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_foo:[0-9]+]] foo: @type[[TYPE0]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_bar:[0-9]+]] bar: ptr<@type[[TYPE0]]> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_foo]])), const<i32>(37), const<u64>(16));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_foo]]), reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(16))));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE0]]>>(%[[VALUE_bar]], call<ptr<@type[[TYPE0]]>, signature=fn(ptr<@type[[TYPE0]]>, i32) -> ptr<@type[[TYPE0]]>>(%[[VALUE_f]], addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_foo]]), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(16)))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<void>>(read<ptr<void>>(field0(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bar]])))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<@type[[TYPE0]]>>(%[[VALUE_foo]]))), ne<i32>(read<i32>(field1(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bar]])))), const<i32>(0))), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field2(deref(read<ptr<@type[[TYPE0]]>>(%[[VALUE_bar]])))))), const<u64>(16)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
