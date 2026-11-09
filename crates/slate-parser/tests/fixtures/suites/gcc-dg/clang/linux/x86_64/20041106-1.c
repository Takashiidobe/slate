/* { dg-do run { target *-*-linux* *-*-gnu* *-*-solaris* *-*-uclinux* } } */
/* { dg-options -O2 } */

#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

struct S { long x __attribute__((packed)); }; /* { dg-warning "attribute ignored" "" { target default_packed } } */
volatile long sink;

void foo (struct S *s)
{
  sink = s->x;
}

int main ()
{
  size_t ps = getpagesize ();
  char *ptr, *page;
  struct S *s;

  ptr = malloc (3*ps);
  page = (char *)(((__UINTPTR_TYPE__)ptr + (ps - 1)) & -ps);
  munmap (page + ps, ps);

  s = (struct S *)(page + ps - sizeof(struct S));
  foo(s);

  return 0;
}

// SLATE-FILECHECK-STD DEFAULT c89
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
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 x: i64;
// DEFAULT-NEXT:     } [size=8, align=1, offsets=[0]];
// DEFAULT-NEXT:     global %[[VALUE_sink:[0-9]+]] sink: volatile i64 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_munmap:[0-9]+]] @munmap(%[[VALUE___addr:[0-9]+]] __addr: ptr<void>, %[[VALUE___len:[0-9]+]] __len: u64 [c="size_t"]) -> i32 [linkage=external] [c="int(void *, size_t)"];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64 [c="size_t"]) -> ptr<void> [linkage=external] [c="void *(size_t)"];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_s:[0-9]+]] s: ptr<@type[[TYPE_S]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i64, volatile>(%[[VALUE_sink]], read<i64>(field0(deref(read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s]])))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_getpagesize:[0-9]+]] @getpagesize(unprototyped) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(unprototyped) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_ps:[0-9]+]] ps: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(call<i32, signature=fn(unprototyped) -> i32>(%[[VALUE_getpagesize]]))) [c="size_t"] [c="size_t"];
// DEFAULT-NEXT:         let %[[VALUE_ptr:[0-9]+]] ptr: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_page:[0-9]+]] page: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: ptr<@type[[TYPE_S]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_ptr]], pointer_cast<ptr<i8>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], reinterpret<u64, reason=arg>(mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(3))) [c="size_t"], read<u64>(%[[VALUE_ps]]) [c="size_t"]) [c="size_t"]) [c="__size_t"])));
// DEFAULT-NEXT:         write<ptr<i8>>(%[[VALUE_page]], int_to_ptr<ptr<i8>, reason=explicit>(and<u64>(add<u64, overflow=wrap>(ptr_to_int<u64, reason=explicit>(read<ptr<i8>>(%[[VALUE_ptr]])), reinterpret<u64, reason=usual_arith>(sub<u64, overflow=wrap>(read<u64>(%[[VALUE_ps]]) [c="size_t"], reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))) [c="size_t"]) [c="size_t"])), reinterpret<u64, reason=usual_arith>(neg<u64, overflow=wrap>(read<u64>(%[[VALUE_ps]]) [c="size_t"]) [c="size_t"]))));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<void>, u64) -> i32>(%[[VALUE_munmap]], pointer_cast<ptr<void>, reason=arg>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_page]]), read<u64>(%[[VALUE_ps]]) [c="size_t"])), read<u64>(%[[VALUE_ps]]) [c="size_t"]);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_S]]>>(%[[VALUE_s_2]], pointer_cast<ptr<@type[[TYPE_S]]>, reason=explicit>(ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%[[VALUE_page]]), read<u64>(%[[VALUE_ps]]) [c="size_t"]), size_of<u64, @type[[TYPE_S]]>(8) [c="__size_t"])));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_S]]>) -> void>(%[[VALUE_foo]], read<ptr<@type[[TYPE_S]]>>(%[[VALUE_s_2]]));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
