/* PR tree-optimization/84339 */

struct S {
  int  a;
  char b[1];
};

__attribute__((noipa)) int foo(struct S *p) {
  return __builtin_strlen(&p->b[0]);
}

__attribute__((noipa)) int bar(struct S *p) { return __builtin_strlen(p->b); }

int main() {
  struct S *p = __builtin_malloc(sizeof(struct S) + 16);
  if (p) {
    p->a = 1;
    __builtin_strcpy(p->b, "abcdefg");
    if (foo(p) != 7 || bar(p) != 7)
      __builtin_abort();
    __builtin_free(p);
  }
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i32;
// DEFAULT-NEXT:         field1 b: array<i8, 1>;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 8> [storage=static] = code_units<array<i8, 8>>([97, 98, 99, 100, 101, 102, 103, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strlen:[0-9]+]] @__builtin_strlen(%[[VALUE0:[0-9]+]] <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE___builtin_strlen]], pointer_cast<ptr<const i8>, reason=arg>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p]])))), const<i32>(0))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_S]]>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return reinterpret<i32, reason=return, fits=unknown>(truncate<u32, reason=return, fits=unknown>(call<u64, signature=fn(ptr<const i8>) -> u64>(%[[VALUE___builtin_strlen]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_2]]))))))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_malloc:[0-9]+]] @__builtin_malloc(%[[VALUE1:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_strcpy:[0-9]+]] @__builtin_strcpy(%[[VALUE2:[0-9]+]] <unnamed>: ptr<i8>, %[[VALUE3:[0-9]+]] <unnamed>: ptr<const i8>) -> ptr<i8> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_free:[0-9]+]] @__builtin_free(%[[VALUE4:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_p_3:[0-9]+]] p: ptr<@type[[TYPE_S]]> [storage=automatic] = pointer_cast<ptr<@type[[TYPE_S]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE___builtin_malloc]], add<u64, overflow=wrap>(const<u64>(8), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(16))))));
// DEFAULT-NEXT:         if ne<ptr<@type[[TYPE_S]]>>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_3]]), null<ptr<@type[[TYPE_S]]>>)
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 write<i32>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_3]]))), const<i32>(1));
// DEFAULT-NEXT:                 call<ptr<i8>, signature=fn(ptr<i8>, ptr<const i8>) -> ptr<i8>>(%[[VALUE___builtin_strcpy]], array_decay<ptr<i8>, length=Some(1)>(field1(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_3]])))), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(8)>(%[[VALUE_str]])));
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: bool [synthetic];
// DEFAULT-NEXT:                 if ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_S]]>) -> i32>(%[[VALUE_foo]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_3]])), const<i32>(7))
// DEFAULT-NEXT:                     write<bool>(%[[VALUE5]], const<bool>(true));
// DEFAULT-NEXT:                 else
// DEFAULT-NEXT:                     write<bool>(%[[VALUE5]], ne<i32>(call<i32, signature=fn(ptr<@type[[TYPE_S]]>) -> i32>(%[[VALUE_bar]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_3]])), const<i32>(7)));
// DEFAULT-NEXT:                 if read<bool>(%[[VALUE5]])
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_p_3]])));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
