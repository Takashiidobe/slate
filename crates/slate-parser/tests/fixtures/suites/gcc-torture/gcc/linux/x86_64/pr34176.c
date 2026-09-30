
typedef __SIZE_TYPE__ size_t;
typedef unsigned int  index_ty;
typedef index_ty     *index_list_ty;

struct mult_index {
  index_ty     index;
  unsigned int count;
};

struct mult_index_list {
  struct mult_index *item;
  size_t             nitems;
  size_t             nitems_max;

  struct mult_index *item2;
  size_t             nitems2_max;
};

int __attribute__((noinline)) hash_find_entry(size_t *result) {
  *result = 2;
  return 0;
}

extern void                                  abort(void);
struct mult_index *__attribute__((noinline)) foo(size_t n) {
  static int count = 0;
  if (count++ > 0)
    abort();
  return 0;
}

int main(void) {
  size_t nitems = 0;

  for (;;) {
    size_t list;

    hash_find_entry(&list);
    {
      size_t             len2 = list;
      struct mult_index *destptr;
      struct mult_index *dest;
      size_t             new_max = nitems + len2;

      if (new_max != len2)
        break;
      dest = foo(new_max);

      destptr = dest;
      while (len2--)
        destptr++;

      nitems = destptr - dest;
    }
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_index_ty:[0-9]+]] index_ty = u32;
// DEFAULT-NEXT:     type @type[[TYPE_index_list_ty:[0-9]+]] index_list_ty = ptr<u32>;
// DEFAULT-NEXT:     type @type[[TYPE_mult_index:[0-9]+]] mult_index = struct {
// DEFAULT-NEXT:         field0 index: u32;
// DEFAULT-NEXT:         field1 count: u32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE_mult_index_list:[0-9]+]] mult_index_list = struct {
// DEFAULT-NEXT:         field0 item: ptr<@type[[TYPE_mult_index]]>;
// DEFAULT-NEXT:         field1 nitems: u64;
// DEFAULT-NEXT:         field2 nitems_max: u64;
// DEFAULT-NEXT:         field3 item2: ptr<@type[[TYPE_mult_index]]>;
// DEFAULT-NEXT:         field4 nitems2_max: u64;
// DEFAULT-NEXT:     } [size=40, align=8, offsets=[0, 8, 16, 24, 32]];
// DEFAULT-NEXT:     global %[[VALUE_count:[0-9]+]] count: i32 [storage=static] = const<i32>(0) [linkage=internal];
// DEFAULT-NEXT:     fn %[[VALUE_hash_find_entry:[0-9]+]] @hash_find_entry(%[[VALUE_result:[0-9]+]] result: ptr<u64>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         write<u64>(deref(read<ptr<u64>>(%[[VALUE_result]])), reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(2))));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_n:[0-9]+]] n: u64) -> ptr<@type[[TYPE_mult_index]]> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_count]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE0]]), const<i32>(1));
// DEFAULT-NEXT:         write<i32>(%[[VALUE_count]], read<i32>(%[[VALUE1]]));
// DEFAULT-NEXT:         if gt<i32>(read<i32>(%[[VALUE0]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return null<ptr<@type[[TYPE_mult_index]]>>;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_nitems:[0-9]+]] nitems: u64 [storage=automatic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(const<i32>(0)));
// DEFAULT-NEXT:         for %[[VALUE2:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:             condition: omitted
// DEFAULT-NEXT:             increment: omitted
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 {
// DEFAULT-NEXT:                     let %[[VALUE_list:[0-9]+]] list: u64 [storage=automatic];
// DEFAULT-NEXT:                     call<i32, signature=fn(ptr<u64>) -> i32>(%[[VALUE_hash_find_entry]], addr_of<ptr<u64>>(%[[VALUE_list]]));
// DEFAULT-NEXT:                     {
// DEFAULT-NEXT:                         let %[[VALUE_len2:[0-9]+]] len2: u64 [storage=automatic] = read<u64>(%[[VALUE_list]]);
// DEFAULT-NEXT:                         let %[[VALUE_destptr:[0-9]+]] destptr: ptr<@type[[TYPE_mult_index]]> [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_dest:[0-9]+]] dest: ptr<@type[[TYPE_mult_index]]> [storage=automatic];
// DEFAULT-NEXT:                         let %[[VALUE_new_max:[0-9]+]] new_max: u64 [storage=automatic] = add<u64, overflow=wrap>(read<u64>(%[[VALUE_nitems]]), read<u64>(%[[VALUE_len2]]));
// DEFAULT-NEXT:                         if ne<u64>(read<u64>(%[[VALUE_new_max]]), read<u64>(%[[VALUE_len2]]))
// DEFAULT-NEXT:                             break %[[VALUE2]];
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_mult_index]]>>(%[[VALUE_dest]], call<ptr<@type[[TYPE_mult_index]]>, signature=fn(u64) -> ptr<@type[[TYPE_mult_index]]>>(%[[VALUE_foo]], read<u64>(%[[VALUE_new_max]])));
// DEFAULT-NEXT:                         write<ptr<@type[[TYPE_mult_index]]>>(%[[VALUE_destptr]], read<ptr<@type[[TYPE_mult_index]]>>(%[[VALUE_dest]]));
// DEFAULT-NEXT:                         while %[[VALUE3:[0-9]+]] {
// DEFAULT-NEXT:                             let %[[VALUE4:[0-9]+]]: u64 [synthetic] = read<u64>(%[[VALUE_len2]]);
// DEFAULT-NEXT:                             let %[[VALUE5:[0-9]+]]: u64 [synthetic] = sub<u64, overflow=wrap>(read<u64>(%[[VALUE4]]), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1))));
// DEFAULT-NEXT:                             write<u64>(%[[VALUE_len2]], read<u64>(%[[VALUE5]]));
// DEFAULT-NEXT:                             yield ne<u64>(read<u64>(%[[VALUE4]]), const<u64>(0));
// DEFAULT-NEXT:                         }
// DEFAULT-NEXT:                             let %[[VALUE6:[0-9]+]]: ptr<@type[[TYPE_mult_index]]> [synthetic] = read<ptr<@type[[TYPE_mult_index]]>>(%[[VALUE_destptr]]);
// DEFAULT-NEXT:                             let %[[VALUE7:[0-9]+]]: ptr<@type[[TYPE_mult_index]]> [synthetic] = ptr_offset<ptr<@type[[TYPE_mult_index]]>, subtract=false, element=@type[[TYPE_mult_index]], overflow=ub>(read<ptr<@type[[TYPE_mult_index]]>>(%[[VALUE6]]), const<i32>(1));
// DEFAULT-NEXT:                             write<ptr<@type[[TYPE_mult_index]]>>(%[[VALUE_destptr]], read<ptr<@type[[TYPE_mult_index]]>>(%[[VALUE7]]));
// DEFAULT-NEXT:                         write<u64>(%[[VALUE_nitems]], reinterpret<u64, reason=assign, fits=unknown>(ptr_diff<i64, element=@type[[TYPE_mult_index]], same_array=required, overflow=ub>(read<ptr<@type[[TYPE_mult_index]]>>(%[[VALUE_destptr]]), read<ptr<@type[[TYPE_mult_index]]>>(%[[VALUE_dest]]))));
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                 }
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
