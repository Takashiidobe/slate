// SLATE-FILECHECK-DEFINES DEFAULT

/* Declaration of the frame size doesn't work on ptx.  */
/* { dg-require-effective-target untyped_assembly } */
/* { dg-require-effective-target indirect_calls } */

#define ASIZE 0x1000000000UL
#include "20031023-1.c"

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
// DEFAULT-NEXT:     global %[[VALUE_fn:[0-9]+]] fn: ptr<fn(ptr<const i8>) -> i32> [storage=static] = function_decay<ptr<fn(ptr<const i8>) -> i32>>(%[[VALUE_foo:[0-9]+]]) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_foo]] @foo(%[[VALUE_s:[0-9]+]] s: ptr<const i8>) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if not<bool>(ne<ptr<const i8>>(read<ptr<const i8>>(%[[VALUE_s]]), null<ptr<const i8>>))
// DEFAULT-NEXT:             return const<i32>(1);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_s]]), const<i32>(0))))), const<i32>(97))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: ptr<const i8> [synthetic] = read<ptr<const i8>>(%[[VALUE_s]]);
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: ptr<const i8> [synthetic] = ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE0]]), sub<u64, overflow=wrap>(const<u64>(68719476736), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))));
// DEFAULT-NEXT:         write<ptr<const i8>>(%[[VALUE_s]], read<ptr<const i8>>(%[[VALUE1]]));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<const i8>, subtract=false, element=i8, overflow=ub>(read<ptr<const i8>>(%[[VALUE_s]]), const<i32>(0))))), const<i32>(98))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar() -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_s_2:[0-9]+]] s: array<i8, 68719476736> [storage=automatic] [align=16];
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(68719476736)>(%[[VALUE_s_2]]), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(68719476736)>(%[[VALUE_s_2]]), sub<u64, overflow=wrap>(const<u64>(68719476736), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), truncate<i8, reason=assign, fits=always>(const<i32>(98)));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_foo]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68719476736)>(%[[VALUE_s_2]])));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_foo]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68719476736)>(%[[VALUE_s_2]])));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_baz:[0-9]+]] @baz(%[[VALUE_i:[0-9]+]] i: i64) -> i32 [linkage=external] [inline=never] [definition=emitted] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if ne<i64>(read<i64>(%[[VALUE_i]]), const<i64>(0))
// DEFAULT-NEXT:             return call<i32, signature=fn(ptr<const i8>) -> i32>(read<ptr<fn(ptr<const i8>) -> i32>>(%[[VALUE_fn]]), null<ptr<const i8>>);
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 let %[[VALUE_s_3:[0-9]+]] s: array<i8, 68719476736> [storage=automatic] [align=16];
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(68719476736)>(%[[VALUE_s_3]]), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(97)));
// DEFAULT-NEXT:                 write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(68719476736)>(%[[VALUE_s_3]]), sub<u64, overflow=wrap>(const<u64>(68719476736), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(const<i32>(1)))))), truncate<i8, reason=assign, fits=always>(const<i32>(98)));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_foo]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68719476736)>(%[[VALUE_s_3]])));
// DEFAULT-NEXT:                 call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_foo]], pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(68719476736)>(%[[VALUE_s_3]])));
// DEFAULT-NEXT:                 return call<i32, signature=fn(ptr<const i8>) -> i32>(read<ptr<fn(ptr<const i8>) -> i32>>(%[[VALUE_fn]]), null<ptr<const i8>>);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn() -> i32>(%[[VALUE_bar]]), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_baz]], widen<i64, reason=arg>(const<i32>(0))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(call<i32, signature=fn(i64) -> i32>(%[[VALUE_baz]], widen<i64, reason=arg>(const<i32>(1))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
