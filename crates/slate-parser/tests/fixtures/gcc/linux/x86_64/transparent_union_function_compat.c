#define _GNU_SOURCE
#include <sys/socket.h>

typedef union {
  int  *i;
  long *l;
} Argument __attribute__((transparent_union));

int take(Argument);
int take(int *);

int take(Argument argument) { return *argument.i; }

int read_long(long *value) { return (int)*value; }

static int call(int (*f)(int, struct sockaddr *, socklen_t *), int fd,
                struct sockaddr *address, socklen_t *length) {
  return f(fd, address, length);
}

int main(int argc, char **argv) {
  (void)argv;
  struct sockaddr address;
  socklen_t length = sizeof address;
  int (*from_member)(Argument) = read_long;
  int (*to_member)(int *) = take;
  int (*merged)(int *) = argc ? take : to_member;
  int value = 5;
  long wide = 7;
  return call(getsockname, -1, &address, &length) + from_member(&wide) +
         to_member(&value) + merged(&value);
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
// DEFAULT-NEXT:     type @type[[TYPE___socklen_t:[0-9]+]] __socklen_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_socklen_t:[0-9]+]] socklen_t = u32;
// DEFAULT-NEXT:     type @type[[TYPE_sa_family_t:[0-9]+]] sa_family_t = u16;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr:[0-9]+]] sockaddr = struct {
// DEFAULT-NEXT:         field0 sa_family: u16;
// DEFAULT-NEXT:         field1 sa_data: array<i8, 14>;
// DEFAULT-NEXT:     } [size=16, align=2, offsets=[0, 2]];
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 __sockaddr__: ptr<@type[[TYPE_sockaddr]]>;
// DEFAULT-NEXT:         field1 __sockaddr_at__: ptr<@type[[TYPE_sockaddr_at:[0-9]+]]>;
// DEFAULT-NEXT:         field2 __sockaddr_ax25__: ptr<@type[[TYPE_sockaddr_ax25:[0-9]+]]>;
// DEFAULT-NEXT:         field3 __sockaddr_dl__: ptr<@type[[TYPE_sockaddr_dl:[0-9]+]]>;
// DEFAULT-NEXT:         field4 __sockaddr_eon__: ptr<@type[[TYPE_sockaddr_eon:[0-9]+]]>;
// DEFAULT-NEXT:         field5 __sockaddr_in__: ptr<@type[[TYPE_sockaddr_in:[0-9]+]]>;
// DEFAULT-NEXT:         field6 __sockaddr_in6__: ptr<@type[[TYPE_sockaddr_in6:[0-9]+]]>;
// DEFAULT-NEXT:         field7 __sockaddr_inarp__: ptr<@type[[TYPE_sockaddr_inarp:[0-9]+]]>;
// DEFAULT-NEXT:         field8 __sockaddr_ipx__: ptr<@type[[TYPE_sockaddr_ipx:[0-9]+]]>;
// DEFAULT-NEXT:         field9 __sockaddr_iso__: ptr<@type[[TYPE_sockaddr_iso:[0-9]+]]>;
// DEFAULT-NEXT:         field10 __sockaddr_ns__: ptr<@type[[TYPE_sockaddr_ns:[0-9]+]]>;
// DEFAULT-NEXT:         field11 __sockaddr_un__: ptr<@type[[TYPE_sockaddr_un:[0-9]+]]>;
// DEFAULT-NEXT:         field12 __sockaddr_x25__: ptr<@type[[TYPE_sockaddr_x25:[0-9]+]]>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_at]] sockaddr_at = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_ax25]] sockaddr_ax25 = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_dl]] sockaddr_dl = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_eon]] sockaddr_eon = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_in]] sockaddr_in = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_in6]] sockaddr_in6 = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_inarp]] sockaddr_inarp = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_ipx]] sockaddr_ipx = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_iso]] sockaddr_iso = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_ns]] sockaddr_ns = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_un]] sockaddr_un = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_sockaddr_x25]] sockaddr_x25 = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE___SOCKADDR_ARG:[0-9]+]] __SOCKADDR_ARG = @type[[TYPE0]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = union {
// DEFAULT-NEXT:         field0 i: ptr<i32>;
// DEFAULT-NEXT:         field1 l: ptr<i64>;
// DEFAULT-NEXT:     } [size=8, align=8, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_Argument:[0-9]+]] Argument = @type[[TYPE1]];
// DEFAULT-NEXT:     fn %[[VALUE_getsockname:[0-9]+]] @getsockname(%[[VALUE___fd:[0-9]+]] __fd: i32, %[[VALUE___addr:[0-9]+]] __addr: @type[[TYPE0]], %[[VALUE___len:[0-9]+]] __len: ptr<u32> [restrict]) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_take:[0-9]+]] @take(%[[VALUE_argument:[0-9]+]] argument: @type[[TYPE1]]) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<i32>(deref(read<ptr<i32>>(field0(%[[VALUE_argument]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_read_long:[0-9]+]] @read_long(%[[VALUE_value:[0-9]+]] value: ptr<i64>) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return truncate<i32, reason=explicit, fits=unknown>(read<i64>(deref(read<ptr<i64>>(%[[VALUE_value]]))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_call:[0-9]+]] @call(%[[VALUE_f:[0-9]+]] f: ptr<fn(i32, ptr<@type[[TYPE_sockaddr]]>, ptr<u32>) -> i32>, %[[VALUE_fd:[0-9]+]] fd: i32, %[[VALUE_address:[0-9]+]] address: ptr<@type[[TYPE_sockaddr]]>, %[[VALUE_length:[0-9]+]] length: ptr<u32>) -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<i32, signature=fn(i32, ptr<@type[[TYPE_sockaddr]]>, ptr<u32>) -> i32>(read<ptr<fn(i32, ptr<@type[[TYPE_sockaddr]]>, ptr<u32>) -> i32>>(%[[VALUE_f]]), read<i32>(%[[VALUE_fd]]), read<ptr<@type[[TYPE_sockaddr]]>>(%[[VALUE_address]]), read<ptr<u32>>(%[[VALUE_length]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         read<ptr<ptr<i8>>>(%[[VALUE_argv]]);
// DEFAULT-NEXT:         let %[[VALUE_address_2:[0-9]+]] address: @type[[TYPE_sockaddr]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_length_2:[0-9]+]] length: u32 [storage=automatic] = truncate<u32, reason=assign, fits=always>(const<u64>(16));
// DEFAULT-NEXT:         let %[[VALUE_from_member:[0-9]+]] from_member: ptr<fn(@type[[TYPE1]]) -> i32> [storage=automatic] = pointer_cast<ptr<fn(@type[[TYPE1]]) -> i32>, reason=assign>(function_decay<ptr<fn(ptr<i64>) -> i32>>(%[[VALUE_read_long]]));
// DEFAULT-NEXT:         let %[[VALUE_to_member:[0-9]+]] to_member: ptr<fn(ptr<i32>) -> i32> [storage=automatic] = pointer_cast<ptr<fn(ptr<i32>) -> i32>, reason=assign>(function_decay<ptr<fn(@type[[TYPE1]]) -> i32>>(%[[VALUE_take]]));
// DEFAULT-NEXT:         let %[[VALUE_merged:[0-9]+]] merged: ptr<fn(ptr<i32>) -> i32> [storage=automatic] = pointer_cast<ptr<fn(ptr<i32>) -> i32>, reason=assign>(conditional<ptr<fn(@type[[TYPE1]]) -> i32>>(ne<i32>(read<i32>(%[[VALUE_argc]]), const<i32>(0)), function_decay<ptr<fn(@type[[TYPE1]]) -> i32>>(%[[VALUE_take]]), pointer_cast<ptr<fn(@type[[TYPE1]]) -> i32>, reason=usual_arith>(read<ptr<fn(ptr<i32>) -> i32>>(%[[VALUE_to_member]]))));
// DEFAULT-NEXT:         let %[[VALUE_value_2:[0-9]+]] value: i32 [storage=automatic] = const<i32>(5);
// DEFAULT-NEXT:         let %[[VALUE_wide:[0-9]+]] wide: i64 [storage=automatic] = widen<i64, reason=assign>(const<i32>(7));
// DEFAULT-NEXT:         return add<i32, overflow=ub>(add<i32, overflow=ub>(add<i32, overflow=ub>(call<i32, signature=fn(ptr<fn(i32, ptr<@type[[TYPE_sockaddr]]>, ptr<u32>) -> i32>, i32, ptr<@type[[TYPE_sockaddr]]>, ptr<u32>) -> i32>(%[[VALUE_call]], pointer_cast<ptr<fn(i32, ptr<@type[[TYPE_sockaddr]]>, ptr<u32>) -> i32>, reason=arg>(function_decay<ptr<fn(i32, @type[[TYPE0]], ptr<u32>) -> i32>>(%[[VALUE_getsockname]])), neg<i32, overflow=ub>(const<i32>(1)), addr_of<ptr<@type[[TYPE_sockaddr]]>>(%[[VALUE_address_2]]), addr_of<ptr<u32>>(%[[VALUE_length_2]])), call<i32, signature=fn(@type[[TYPE1]]) -> i32>(read<ptr<fn(@type[[TYPE1]]) -> i32>>(%[[VALUE_from_member]]), aggregate<@type[[TYPE1]], zero_fill=false>(field1 = addr_of<ptr<i64>>(%[[VALUE_wide]])))), call<i32, signature=fn(ptr<i32>) -> i32>(read<ptr<fn(ptr<i32>) -> i32>>(%[[VALUE_to_member]]), addr_of<ptr<i32>>(%[[VALUE_value_2]]))), call<i32, signature=fn(ptr<i32>) -> i32>(read<ptr<fn(ptr<i32>) -> i32>>(%[[VALUE_merged]]), addr_of<ptr<i32>>(%[[VALUE_value_2]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
