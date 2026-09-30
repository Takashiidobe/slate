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
// DEFAULT-NEXT:     type @type[[TYPE_mcheck_status:[0-9]+]] mcheck_status = enum : i32 {
// DEFAULT-NEXT:         %[[VALUE_MCHECK_DISABLED:[0-9]+]] MCHECK_DISABLED = const<i32>(-1);
// DEFAULT-NEXT:         %[[VALUE_MCHECK_OK:[0-9]+]] MCHECK_OK = const<i32>(0);
// DEFAULT-NEXT:         %[[VALUE_MCHECK_FREE:[0-9]+]] MCHECK_FREE = const<i32>(1);
// DEFAULT-NEXT:         %[[VALUE_MCHECK_HEAD:[0-9]+]] MCHECK_HEAD = const<i32>(2);
// DEFAULT-NEXT:         %[[VALUE_MCHECK_TAIL:[0-9]+]] MCHECK_TAIL = const<i32>(3);
// DEFAULT-NEXT:     } [size=4, align=4];
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     extern %[[VALUE_error_message_count:[0-9]+]] error_message_count: u32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     extern %[[VALUE_error_one_per_line:[0-9]+]] error_one_per_line: i32 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 12> [storage=static] = code_units<array<i8, 12>>([111, 110, 95, 101, 120, 105, 116, 58, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_2:[0-9]+]] .str[[VALUE_str_2]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([109, 99, 104, 101, 99, 107, 58, 37, 100, 32, 37, 100, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_3:[0-9]+]] .str[[VALUE_str_3]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([102, 105, 114, 115, 116, 32, 109, 101, 115, 115, 97, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_4:[0-9]+]] .str[[VALUE_str_4]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([115, 101, 99, 111, 110, 100, 32, 109, 101, 115, 115, 97, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_5:[0-9]+]] .str[[VALUE_str_5]]: array<i8, 20> [storage=static] = code_units<array<i8, 20>>([99, 111, 117, 110, 116, 95, 97, 102, 116, 101, 114, 95, 116, 119, 111, 58, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_6:[0-9]+]] .str[[VALUE_str_6]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 97, 109, 112, 108, 101, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_7:[0-9]+]] .str[[VALUE_str_7]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([100, 101, 100, 117, 112, 101, 100, 32, 109, 101, 115, 115, 97, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_8:[0-9]+]] .str[[VALUE_str_8]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 97, 109, 112, 108, 101, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_9:[0-9]+]] .str[[VALUE_str_9]]: array<i8, 16> [storage=static] = code_units<array<i8, 16>>([100, 101, 100, 117, 112, 101, 100, 32, 109, 101, 115, 115, 97, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_10:[0-9]+]] .str[[VALUE_str_10]]: array<i8, 22> [storage=static] = code_units<array<i8, 22>>([99, 111, 117, 110, 116, 95, 97, 102, 116, 101, 114, 95, 100, 101, 100, 117, 112, 58, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_11:[0-9]+]] .str[[VALUE_str_11]]: array<i8, 9> [storage=static] = code_units<array<i8, 9>>([115, 97, 109, 112, 108, 101, 46, 99, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_12:[0-9]+]] .str[[VALUE_str_12]]: array<i8, 15> [storage=static] = code_units<array<i8, 15>>([100, 105, 102, 102, 101, 114, 101, 110, 116, 32, 108, 105, 110, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_13:[0-9]+]] .str[[VALUE_str_13]]: array<i8, 25> [storage=static] = code_units<array<i8, 25>>([99, 111, 117, 110, 116, 95, 97, 102, 116, 101, 114, 95, 110, 101, 119, 95, 108, 105, 110, 101, 58, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_14:[0-9]+]] .str[[VALUE_str_14]]: array<i8, 14> [storage=static] = code_units<array<i8, 14>>([102, 97, 116, 97, 108, 32, 109, 101, 115, 115, 97, 103, 101, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_str_15:[0-9]+]] .str[[VALUE_str_15]]: array<i8, 13> [storage=static] = code_units<array<i8, 13>>([117, 110, 114, 101, 97, 99, 104, 97, 98, 108, 101, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_MCHECK_HEAD]] @error(%[[VALUE___status:[0-9]+]] __status: i32, %[[VALUE___errnum:[0-9]+]] __errnum: i32, %[[VALUE___format:[0-9]+]] __format: ptr<const i8>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_error_at_line:[0-9]+]] @error_at_line(%[[VALUE___status_2:[0-9]+]] __status: i32, %[[VALUE___errnum_2:[0-9]+]] __errnum: i32, %[[VALUE___fname:[0-9]+]] __fname: ptr<const i8>, %[[VALUE___lineno:[0-9]+]] __lineno: u32, %[[VALUE___format_2:[0-9]+]] __format: ptr<const i8>, ...) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mcheck:[0-9]+]] @mcheck(%[[VALUE___abortfunc:[0-9]+]] __abortfunc: ptr<fn(@type[[TYPE_mcheck_status]]) -> void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_mprobe:[0-9]+]] @mprobe(%[[VALUE___ptr:[0-9]+]] __ptr: ptr<void>) -> @type[[TYPE_mcheck_status]] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_printf:[0-9]+]] @printf(%[[VALUE___format_3:[0-9]+]] __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE___ptr_2:[0-9]+]] __ptr: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_on_exit:[0-9]+]] @on_exit(%[[VALUE___func:[0-9]+]] __func: ptr<fn(i32, ptr<void>) -> void>, %[[VALUE___arg:[0-9]+]] __arg: ptr<void>) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_handle_exit:[0-9]+]] @handle_exit(%[[VALUE_status:[0-9]+]] status: i32, %[[VALUE_arg:[0-9]+]] arg: ptr<void>) -> void [linkage=internal] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_captured:[0-9]+]] captured: ptr<i32> [storage=automatic] = pointer_cast<ptr<i32>, reason=explicit>(read<ptr<void>>(%[[VALUE_arg]]));
// DEFAULT-NEXT:         write<i32>(deref(read<ptr<i32>>(%[[VALUE_captured]])), read<i32>(%[[VALUE_status]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(12)>(%[[VALUE_str]])), read<i32>(%[[VALUE_status]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_captured_2:[0-9]+]] captured: i32 [storage=automatic] = neg<i32, overflow=ub>(const<i32>(1));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<fn(i32, ptr<void>) -> void>, ptr<void>) -> i32>(%[[VALUE_on_exit]], function_decay<ptr<fn(i32, ptr<void>) -> void>>(%[[VALUE_handle_exit]]), pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<i32>>(%[[VALUE_captured_2]])));
// DEFAULT-NEXT:         let %[[VALUE_mcheck_enabled:[0-9]+]] mcheck_enabled: i32 [storage=automatic] = from_bool<i32, reason=assign>(eq<i32>(call<i32, signature=fn(ptr<fn(@type[[TYPE_mcheck_status]]) -> void>) -> i32>(%[[VALUE_mcheck]], null<ptr<fn(@type[[TYPE_mcheck_status]]) -> void>>), const<i32>(0)));
// DEFAULT-NEXT:         let %[[VALUE_block:[0-9]+]] block: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(16))));
// DEFAULT-NEXT:         let %[[VALUE_probe:[0-9]+]] probe: @type[[TYPE_mcheck_status]] [storage=automatic] = call<@type[[TYPE_mcheck_status]], signature=fn(ptr<void>) -> @type[[TYPE_mcheck_status]]>(%[[VALUE_mprobe]], read<ptr<void>>(%[[VALUE_block]]));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_2]])), read<i32>(%[[VALUE_mcheck_enabled]]), from_bool<i32, reason=vararg>(eq<i32>(enum_to_int<i32, reason=promotion>(read<@type[[TYPE_mcheck_status]]>(%[[VALUE_probe]])), const<i32>(0))));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_block]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_error_one_per_line]], const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, ...) -> void>(%[[VALUE_MCHECK_HEAD]], const<i32>(0), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_3]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, ...) -> void>(%[[VALUE_MCHECK_HEAD]], const<i32>(0), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_4]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(20)>(%[[VALUE_str_5]])), read<u32>(%[[VALUE_error_message_count]]));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_error_one_per_line]], const<i32>(1));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, u32, ptr<const i8>, ...) -> void>(%[[VALUE_error_at_line]], const<i32>(0), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_6]])), reinterpret<u32, reason=arg, fits=always>(const<i32>(42)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_7]])));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, u32, ptr<const i8>, ...) -> void>(%[[VALUE_error_at_line]], const<i32>(0), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_8]])), reinterpret<u32, reason=arg, fits=always>(const<i32>(42)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(16)>(%[[VALUE_str_9]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(22)>(%[[VALUE_str_10]])), read<u32>(%[[VALUE_error_message_count]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, u32, ptr<const i8>, ...) -> void>(%[[VALUE_error_at_line]], const<i32>(0), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(9)>(%[[VALUE_str_11]])), reinterpret<u32, reason=arg, fits=always>(const<i32>(43)), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(15)>(%[[VALUE_str_12]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(25)>(%[[VALUE_str_13]])), read<u32>(%[[VALUE_error_message_count]]));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<const i8>, ...) -> void>(%[[VALUE_MCHECK_HEAD]], const<i32>(5), const<i32>(0), pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(14)>(%[[VALUE_str_14]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%[[VALUE_printf]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(13)>(%[[VALUE_str_15]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
