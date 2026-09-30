#include <stdlib.h>

struct {
  long sqlcode;
} sqlca;

struct data_record {
  int dummy;
  int a[100];
} *data_ptr, data_tmp;

int num_records() { return 1; }

void fetch() {
  static int fetch_count;

  __builtin_memset(&data_tmp, 0x55, sizeof(data_tmp));
  sqlca.sqlcode = (++fetch_count > 1 ? 100 : 0);
}

void load_data() {
  struct data_record *p;
  int                 num = num_records();

  data_ptr = malloc(num * sizeof(struct data_record));
  __builtin_memset(data_ptr, 0xaa, num * sizeof(struct data_record));

  fetch();
  p = data_ptr;
  while (sqlca.sqlcode == 0) {
    *p++ = data_tmp;
    fetch();
  }
}

int main(void) {
  load_data();
  if (sizeof(int) == 2 && data_ptr[0].dummy != 0x5555)
    abort();
  else if (sizeof(int) > 2 && data_ptr[0].dummy != 0x55555555)
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
// DEFAULT-NEXT:         field0 sqlcode: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type[[TYPE_data_record:[0-9]+]] data_record = struct {
// DEFAULT-NEXT:         field0 dummy: i32;
// DEFAULT-NEXT:         field1 a: array<i32, 100>;
// DEFAULT-NEXT:     } [size=404, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %[[VALUE_sqlca:[0-9]+]] sqlca: @type[[TYPE0]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_data_ptr:[0-9]+]] data_ptr: ptr<@type[[TYPE_data_record]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_data_tmp:[0-9]+]] data_tmp: @type[[TYPE_data_record]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_fetch_count:[0-9]+]] fetch_count: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE___status:[0-9]+]] __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_num_records:[0-9]+]] @num_records() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_memset:[0-9]+]] @__builtin_memset(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE1:[0-9]+]] <unnamed>: i32, %[[VALUE2:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_fetch:[0-9]+]] @fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type[[TYPE_data_record]]>>(%[[VALUE_data_tmp]])), const<i32>(85), const<u64>(404));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_fetch_count]]);
// DEFAULT-NEXT:         let %[[VALUE4:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE3]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_fetch_count]], read<i32>(%[[VALUE4]]));
// DEFAULT-NEXT:         write<i64>(field0(%[[VALUE_sqlca]]), widen<i64, reason=assign>(conditional<i32>(gt<i32>(read<i32>(%[[VALUE4]]), const<i32>(1)), const<i32>(100), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_load_data:[0-9]+]] @load_data() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_data_record]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_num:[0-9]+]] num: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%[[VALUE_num_records]]);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_data_record]]>>(%[[VALUE_data_ptr]], pointer_cast<ptr<@type[[TYPE_data_record]]>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_num]]))), const<u64>(404)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE___builtin_memset]], pointer_cast<ptr<void>, reason=arg>(read<ptr<@type[[TYPE_data_record]]>>(%[[VALUE_data_ptr]])), const<i32>(170), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_num]]))), const<u64>(404)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_fetch]]);
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_data_record]]>>(%[[VALUE_p]], read<ptr<@type[[TYPE_data_record]]>>(%[[VALUE_data_ptr]]));
// DEFAULT-NEXT:         while %[[VALUE5:[0-9]+]] eq<i64>(read<i64>(field0(%[[VALUE_sqlca]])), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: ptr<@type[[TYPE_data_record]]> [synthetic] = read<ptr<@type[[TYPE_data_record]]>>(%[[VALUE_p]]);
// DEFAULT-NEXT:                 let %[[VALUE7:[0-9]+]]: ptr<@type[[TYPE_data_record]]> [synthetic] = ptr_offset<ptr<@type[[TYPE_data_record]]>, subtract=false, element=@type[[TYPE_data_record]], overflow=ub>(read<ptr<@type[[TYPE_data_record]]>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<@type[[TYPE_data_record]]>>(%[[VALUE_p]], read<ptr<@type[[TYPE_data_record]]>>(%[[VALUE7]]));
// DEFAULT-NEXT:                 write<@type[[TYPE_data_record]]>(deref(read<ptr<@type[[TYPE_data_record]]>>(%[[VALUE6]])), copy<@type[[TYPE_data_record]], reason=assign>(read<@type[[TYPE_data_record]]>(%[[VALUE_data_tmp]])));
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_fetch]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%[[VALUE_load_data]]);
// DEFAULT-NEXT:         if logical_and<bool>(eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_data_record]]>, subtract=false, element=@type[[TYPE_data_record]], overflow=ub>(read<ptr<@type[[TYPE_data_record]]>>(%[[VALUE_data_ptr]]), const<i32>(0))))), const<i32>(21845)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(gt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type[[TYPE_data_record]]>, subtract=false, element=@type[[TYPE_data_record]], overflow=ub>(read<ptr<@type[[TYPE_data_record]]>>(%[[VALUE_data_ptr]]), const<i32>(0))))), const<i32>(1431655765)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
