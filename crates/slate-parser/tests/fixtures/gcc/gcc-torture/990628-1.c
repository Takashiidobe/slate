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
// DEFAULT-NEXT:     type @type0 size_t = u64;
// DEFAULT-NEXT:     type @type1 = struct {
// DEFAULT-NEXT:         field0 sqlcode: i64;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0]];
// DEFAULT-NEXT:     type @type2 data_record = struct {
// DEFAULT-NEXT:         field0 dummy: i32;
// DEFAULT-NEXT:         field1 a: array<i32, 100>;
// DEFAULT-NEXT:     } [size=404, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     global %5 sqlca: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %7 data_ptr: ptr<@type2> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 data_tmp: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 fetch_count: i32 [storage=static] [linkage=internal];
// DEFAULT-NEXT:     fn %1 @malloc(%16 __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%17 __status: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %9 @num_records() -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return const<i32>(1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @fetch() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type2>>(%8)), const<i32>(85), const<u64>(404));
// DEFAULT-NEXT:         let %19: i32 [synthetic] = read<i32>(%11);
// DEFAULT-NEXT:         let %20: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%19), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%11, read<i32>(%20));
// DEFAULT-NEXT:         write<i64>(field0(%5), widen<i64, reason=assign>(conditional<i32>(gt<i32>(read<i32>(%20), const<i32>(1)), const<i32>(100), const<i32>(0))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @load_data() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 p: ptr<@type2> [storage=automatic];
// DEFAULT-NEXT:         let %14 num: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%9);
// DEFAULT-NEXT:         write<ptr<@type2>>(%7, pointer_cast<ptr<@type2>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%1, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14))), const<u64>(404)))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type2>, reason=assign>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%1, mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14))), const<u64>(404))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(read<ptr<@type2>>(%7)), const<i32>(170), mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%14))), const<u64>(404)));
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:         write<ptr<@type2>>(%13, read<ptr<@type2>>(%7));
// DEFAULT-NEXT:         while %18 eq<i64>(read<i64>(field0(%5)), widen<i64, reason=usual_arith>(const<i32>(0)))
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %21: ptr<@type2> [synthetic] = read<ptr<@type2>>(%13);
// DEFAULT-NEXT:                 let %22: ptr<@type2> [synthetic] = ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%21), const<i32>(1));
// DEFAULT-NEXT:                 write<ptr<@type2>>(%13, read<ptr<@type2>>(%22));
// DEFAULT-NEXT:                 write<@type2>(deref(read<ptr<@type2>>(%21)), copy<@type2, reason=assign>(read<@type2>(%8)));
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%10);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn() -> void>(%12);
// DEFAULT-NEXT:         if logical_and<bool>(eq<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%7), const<i32>(0))))), const<i32>(21845)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             if logical_and<bool>(gt<u64>(const<u64>(4), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(2)))), ne<i32>(read<i32>(field0(deref(ptr_offset<ptr<@type2>, subtract=false, element=@type2, overflow=ub>(read<ptr<@type2>>(%7), const<i32>(0))))), const<i32>(1431655765)))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
