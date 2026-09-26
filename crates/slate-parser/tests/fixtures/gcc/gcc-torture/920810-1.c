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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 super: ptr<void>;
// DEFAULT-NEXT:         field1 name: i32;
// DEFAULT-NEXT:         field2 size: i32;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8, 12]];
// DEFAULT-NEXT:     type @type2 t = @type1;
// DEFAULT-NEXT:     fn %1 @malloc(%15 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%16 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @memcpy(%17 __dest: ptr<void> [restrict], %18 __src: ptr<const void> [restrict], %19 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %5 @memset(%20 __s: ptr<void>, %21 __c: i32, %22 __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %8 @f(%9 clas: ptr<@type1>, %10 size: i32) -> ptr<@type1> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %11 child: ptr<@type1> [storage=automatic] = pointer_cast<ptr<@type1>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(malloc, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(%10)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(memcpy, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type1>>(%11)), pointer_cast<ptr<const void>, reason=arg>(read<ptr<@type1>>(%9)), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(read<i32>(field2(deref(read<ptr<@type1>>(%9)))))));
// DEFAULT-NEXT:         write<ptr<void>>(field0(deref(read<ptr<@type1>>(%11))), pointer_cast<ptr<void>, reason=assign>(read<ptr<@type1>>(%9)));
// DEFAULT-NEXT:         write<i32>(field1(deref(read<ptr<@type1>>(%11))), const<i32>(0));
// DEFAULT-NEXT:         write<i32>(field2(deref(read<ptr<@type1>>(%11))), read<i32>(%10));
// DEFAULT-NEXT:         return read<ptr<@type1>>(%11);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %13 foo: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %14 bar: ptr<@type1> [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type1>>(%13)), const<i32>(37), const<u64>(16));
// DEFAULT-NEXT:         write<i32>(field2(%13), reinterpret<i32, reason=assign, fits=unknown>(truncate<u32, reason=assign, fits=always>(const<u64>(16))));
// DEFAULT-NEXT:         write<ptr<@type1>>(%14, call<ptr<@type1>, signature=fn(ptr<@type1>, i32) -> ptr<@type1>>(%8, addr_of<ptr<@type1>>(%13), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(16)))));
// DEFAULT-NEXT:         call<ptr<@type1>, signature=fn(ptr<@type1>, i32) -> ptr<@type1>>(%8, addr_of<ptr<@type1>>(%13), reinterpret<i32, reason=arg, fits=unknown>(truncate<u32, reason=arg, fits=always>(const<u64>(16))));
// DEFAULT-NEXT:         if logical_or<bool>(logical_or<bool>(ne<ptr<void>>(read<ptr<void>>(field0(deref(read<ptr<@type1>>(%14)))), pointer_cast<ptr<void>, reason=usual_arith>(addr_of<ptr<@type1>>(%13))), ne<i32>(read<i32>(field1(deref(read<ptr<@type1>>(%14)))), const<i32>(0))), ne<u64>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(field2(deref(read<ptr<@type1>>(%14)))))), const<u64>(16)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
