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
// DEFAULT-NEXT:     type @type[[TYPE_parse:[0-9]+]] parse = struct {
// DEFAULT-NEXT:         field0 next: ptr<i8>;
// DEFAULT-NEXT:         field1 end: ptr<i8>;
// DEFAULT-NEXT:         field2 error: i32;
// DEFAULT-NEXT:     } [size=24, align=8, offsets=[0, 8, 16]];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_seterr:[0-9]+]] @seterr(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_parse]]>, %[[VALUE_err:[0-9]+]] err: i32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<i32>(field2(deref(read<ptr<@type[[TYPE_parse]]>>(%[[VALUE_p]]))), read<i32>(%[[VALUE_err]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bracket_empty:[0-9]+]] @bracket_empty(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_parse]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if lt<ptr<i8>>(read<ptr<i8>>(field0(deref(read<ptr<@type[[TYPE_parse]]>>(%[[VALUE_p_2]])))), read<ptr<i8>>(field1(deref(read<ptr<@type[[TYPE_parse]]>>(%[[VALUE_p_2]])))))
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: ptr<@type[[TYPE_parse]]> [synthetic] = read<ptr<@type[[TYPE_parse]]>>(%[[VALUE_p_2]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: ptr<i8> [synthetic] = read<ptr<i8>>(field0(deref(read<ptr<@type[[TYPE_parse]]>>(%[[VALUE1]]))));
// DEFAULT-NEXT:             let %[[VALUE3:[0-9]+]]: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE2]]), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(field0(deref(read<ptr<@type[[TYPE_parse]]>>(%[[VALUE1]]))), read<ptr<i8>>(%[[VALUE3]]));
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%[[VALUE2]])))), const<i32>(93)));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE0]], const<bool>(false));
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:         if read<bool>(%[[VALUE0]])
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], const<bool>(true));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             write<bool>(%[[VALUE4]], ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_parse]]>, i32) -> i32>(%[[VALUE_seterr]], read<ptr<@type[[TYPE_parse]]>>(%[[VALUE_p_2]]), const<i32>(7)), const<i32>(0)));
// DEFAULT-NEXT:         if read<bool>(%[[VALUE4]])
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: @type[[TYPE_parse]] [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(field1(%[[VALUE_p_3]]), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(74565)));
// DEFAULT-NEXT:         write<ptr<i8>>(field0(%[[VALUE_p_3]]), int_to_ptr<ptr<i8>, reason=explicit>(const<i32>(74565)));
// DEFAULT-NEXT:         write<i32>(field2(%[[VALUE_p_3]]), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_parse]]>) -> void>(%[[VALUE_bracket_empty]], addr_of<ptr<@type[[TYPE_parse]]>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(field2(%[[VALUE_p_3]])), const<i32>(7))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
