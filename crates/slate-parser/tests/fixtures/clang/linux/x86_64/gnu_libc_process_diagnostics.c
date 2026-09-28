#define _GNU_SOURCE
#include <error.h>
#include <mcheck.h>
#include <stdio.h>
#include <stdlib.h>

static void handle_exit(int status, void *arg) {
  int *captured = (int *)arg;
  *captured     = status;
  printf("on_exit:%d\n", status);
}

int main(void) {
  int captured = -1;
  on_exit(handle_exit, &captured);

  int                mcheck_enabled = mcheck(NULL) == 0;
  void              *block          = malloc(16);
  enum mcheck_status probe          = mprobe(block);
  printf("mcheck:%d %d\n", mcheck_enabled, probe == MCHECK_OK);
  free(block);

  error_one_per_line = 0;
  error(0, 0, "first message");
  error(0, 0, "second message");
  printf("count_after_two:%u\n", error_message_count);

  error_one_per_line = 1;
  error_at_line(0, 0, "sample.c", 42, "deduped message");
  error_at_line(0, 0, "sample.c", 42, "deduped message");
  printf("count_after_dedup:%u\n", error_message_count);

  error_at_line(0, 0, "sample.c", 43, "different line");
  printf("count_after_new_line:%u\n", error_message_count);

  error(5, 0, "fatal message");
  printf("unreachable\n");
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
// DEFAULT-NEXT:     type @type0 mcheck_status = enum : i32 {
// DEFAULT-NEXT:         %0 MCHECK_DISABLED = const<i32>(-1);
// DEFAULT-NEXT:         %1 MCHECK_OK = const<i32>(0);
// DEFAULT-NEXT:         %2 MCHECK_FREE = const<i32>(1);
// DEFAULT-NEXT:         %3 MCHECK_HEAD = const<i32>(2);
// DEFAULT-NEXT:         %4 MCHECK_TAIL = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type1 size_t = u64;
// DEFAULT-NEXT:     extern %10 error_message_count: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %11 error_one_per_line: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %58 .str58: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([111, 110, 95, 101, 120, 105, 116, 58, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %59 .str59: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([109, 99, 104, 101, 99, 107, 58, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %60 .str60: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([102, 105, 114, 115, 116, 32, 109, 101, 115, 115, 97, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %61 .str61: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([115, 101, 99, 111, 110, 100, 32, 109, 101, 115, 115, 97, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %62 .str62: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([99, 111, 117, 110, 116, 95, 97, 102, 116, 101, 114, 95, 116, 119, 111, 58, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %63 .str63: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 97, 109, 112, 108, 101, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %64 .str64: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([100, 101, 100, 117, 112, 101, 100, 32, 109, 101, 115, 115, 97, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %65 .str65: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 97, 109, 112, 108, 101, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %66 .str66: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([100, 101, 100, 117, 112, 101, 100, 32, 109, 101, 115, 115, 97, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %67 .str67: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([99, 111, 117, 110, 116, 95, 97, 102, 116, 101, 114, 95, 100, 101, 100, 117, 112, 58, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %68 .str68: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 97, 109, 112, 108, 101, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %69 .str69: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([100, 105, 102, 102, 101, 114, 101, 110, 116, 32, 108, 105, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %70 .str70: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([99, 111, 117, 110, 116, 95, 97, 102, 116, 101, 114, 95, 110, 101, 119, 95, 108, 105, 110, 101, 58, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %71 .str71: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([102, 97, 116, 97, 108, 32, 109, 101, 115, 115, 97, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %72 .str72: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([117, 110, 114, 101, 97, 99, 104, 97, 98, 108, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @error(%43 __status: i32, %44 __errnum: i32, %45 __format: ptr<const i8>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %9 @error_at_line(%46 __status: i32, %47 __errnum: i32, %48 __fname: ptr<const i8>, %49 __lineno: u32, %50 __format: ptr<const i8>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %19 @mcheck(%51 __abortfunc: ptr<fn(@type0) -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %21 @mprobe(%52 __ptr: ptr<void>) -> @type0 [linkage=external];
// DEFAULT-NEXT:     fn %24 @printf(%53 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %26 @malloc(%54 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %28 @free(%55 __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %33 @on_exit(%56 __func: ptr<fn(i32, ptr<void>) -> void>, %57 __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %34 @handle_exit(%35 status: i32, %36 arg: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %37 captured: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%36));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%37)), read<i32>(%35));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%58)), read<i32>(%35));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %39 captured: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<fn(i32, ptr<void>) -> void>, ptr<void>) -> i32>(%33, function_decay<ptr<fn(i32, ptr<void>) -> void>>(%34), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%39)));
// DEFAULT-NEXT:         let %40 mcheck_enabled: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<fn(@type0) -> void>) -> i32>(%19, null<ptr<fn(@type0) -> void>>), const<i32>(0)));
// DEFAULT-NEXT:         let %41 block: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%26, reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         let %42 probe: @type0 [storage=automatic] = call<@type0, signature=fn(ptr<void>) -> @type0>(%21, read<ptr<void>>(%41));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%59)), read<i32>(%40), from_bool<i32, reason=vararg>(eq<i32>(enum_to_int<i32, reason=promotion>(read<@type0>(%42)), const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%28, read<ptr<void>>(%41));
// DEFAULT-NEXT:         write<i32>(%11, const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, ...) -> void>(%3, const<i32>(0), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%60)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, ...) -> void>(%3, const<i32>(0), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%61)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(20)>(%62)), read<u32>(%10));
// DEFAULT-NEXT:         write<i32>(%11, const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, u32, ptr<const i8>, ...) -> void>(%9, const<i32>(0), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%63)), reinterpret<u32, reason=arg, fits=always>(const<i32>(42)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%64)));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, u32, ptr<const i8>, ...) -> void>(%9, const<i32>(0), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%65)), reinterpret<u32, reason=arg, fits=always>(const<i32>(42)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%66)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%67)), read<u32>(%10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, u32, ptr<const i8>, ...) -> void>(%9, const<i32>(0), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%68)), reinterpret<u32, reason=arg, fits=always>(const<i32>(43)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%69)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%70)), read<u32>(%10));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, ...) -> void>(%3, const<i32>(5), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%71)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%24, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%72)));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
