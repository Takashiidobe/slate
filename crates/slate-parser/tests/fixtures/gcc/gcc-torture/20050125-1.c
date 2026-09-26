/* Verify that the CALL sideeffect isn't optimized away.  */
/* Contributed by Greg Parker  25 Jan 2005  <gparker@apple.com> */

#include <stdio.h>
#include <stdlib.h>

struct parse {
  char *next;
  char *end;
  int   error;
};

int seterr(struct parse *p, int err) {
  p->error = err;
  return 0;
}

void bracket_empty(struct parse *p) {
  if (((p->next < p->end) && (*p->next++) == ']') || seterr(p, 7)) {
  }
}

int main(int    argc __attribute__((unused)),
         char **argv __attribute__((unused))) {
  struct parse p;
  p.next = p.end = (char *)0x12345;

  p.error = 0;
  bracket_empty(&p);
  if (p.error != 7)
    abort();

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
// DEFAULT-NEXT:     type @type0 parse = struct {
// DEFAULT-NEXT:         field0 next: ptr<i8>;
// DEFAULT-NEXT:         field1 end: ptr<i8>;
// DEFAULT-NEXT:         field2 error: i32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @seterr(%3 p: ptr<@type0>, %4 err: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field2(deref(read<ptr<@type0>>(%3))), read<i32>(%4));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @bracket_empty(%6 p: ptr<@type0>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11: bool [synthetic];
// DEFAULT-NEXT:         if lt<ptr<i8>>(read<ptr<i8>>(field0(deref(read<ptr<@type0>>(%6)))), read<ptr<i8>>(field1(deref(read<ptr<@type0>>(%6)))))
// DEFAULT-NEXT:             let %12: ptr<@type0> [synthetic] = read<ptr<@type0>>(%6);
// DEFAULT-NEXT:             let %13: ptr<i8> [synthetic] = read<ptr<i8>>(field0(deref(read<ptr<@type0>>(%12))));
// DEFAULT-NEXT:             let %14: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%13), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(field0(deref(read<ptr<@type0>>(%12))), read<ptr<i8>>(%14));
// DEFAULT-NEXT:             write<bool>(%11, eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%13)))), const<i32>(93)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%11, const<bool>(false));
// DEFAULT-NEXT:         let %15: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%11)
// DEFAULT-NEXT:             write<bool>(%15, const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%15, ne<i32>(call<i32, signature=fn(ptr<@type0>, i32) -> i32>(%2, read<ptr<@type0>>(%6), const<i32>(7)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%15)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @main(%8 argc: i32, %9 argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %10 p: @type0 [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(field1(%10), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(74565)));
// DEFAULT-NEXT:         write<ptr<i8>>(field0(%10), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(74565)));
// DEFAULT-NEXT:         write<i32>(field2(%10), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type0>) -> void>(%5, addr_of<ptr<@type0>>(%10));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field2(%10)), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
