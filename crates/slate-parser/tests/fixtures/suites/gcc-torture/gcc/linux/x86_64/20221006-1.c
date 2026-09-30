#include <stdlib.h>

int main(int argc, char **argv) {
  const int len = argc == 2 ? atoi(argv[1]) : 4;

  int count;
  int data[64];
  int M1[len][len];
  int M2[len][len];

  for (int i = 0; i < len; i++)
    for (int j = 0; j < len; j++)
      M1[i][j] = M2[i][j] = i * len + j;

  M2[1][0] = M2[0][1];

  /* This writes successively 0 and 1 into data[M2[0][1]].  */
  for (int i = 0; i < len - 1; i++)
    for (int j = 0; j < len; j++)
      if (M1[i + 1][j] > M1[i][j])
        data[M2[i][j]] = i;

  if (data[M2[0][1]] != 1)
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
// DEFAULT-NEXT:     fn %[[VALUE_atoi:[0-9]+]] @atoi(%[[VALUE___nptr:[0-9]+]] __nptr: ptr<const i8>) -> i32 [linkage=external] [memory=read];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_len:[0-9]+]] len: i32 [storage=automatic] [const] = conditional<i32>(eq<i32>(read<i32>(%[[VALUE_argc]]), const<i32>(2)), call<i32, signature=fn(ptr<const i8>) -> i32>(%[[VALUE_atoi]], pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(deref(ptr_offset<ptr<ptr<i8>>, subtract=false, element=ptr<i8>, overflow=ub>(read<ptr<ptr<i8>>>(%[[VALUE_argv]]), const<i32>(1)))))), const<i32>(4));
// DEFAULT-NEXT:         let %[[VALUE_count:[0-9]+]] count: i32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_data:[0-9]+]] data: array<i32, 64> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE0:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_len]])));
// DEFAULT-NEXT:         let %[[VALUE1:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_len]])));
// DEFAULT-NEXT:         let %[[VALUE_M1:[0-9]+]] M1: vla<vla<i32, %[[VALUE1]]>, %[[VALUE0]]> [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE2:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_len]])));
// DEFAULT-NEXT:         let %[[VALUE3:[0-9]+]]: u64 [synthetic] = reinterpret<u64, reason=assign, fits=unknown>(widen<i64, reason=assign>(read<i32>(%[[VALUE_len]])));
// DEFAULT-NEXT:         let %[[VALUE_M2:[0-9]+]] M2: vla<vla<i32, %[[VALUE3]]>, %[[VALUE2]]> [storage=automatic];
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_len]]))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE5]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i]], read<i32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %[[VALUE_j:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_j]]), read<i32>(%[[VALUE_len]]))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE8:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j]]);
// DEFAULT-NEXT:                         let %[[VALUE9:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE8]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j]], read<i32>(%[[VALUE9]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         let %[[VALUE10:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(mul<i32, overflow=ub>(read<i32>(%[[VALUE_i]]), read<i32>(%[[VALUE_len]])), read<i32>(%[[VALUE_j]]));
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_M2]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]]))), read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:                         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE1]]>>, subtract=false, element=vla<i32, %[[VALUE1]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE1]]>>, length=None>(%[[VALUE_M1]]), read<i32>(%[[VALUE_i]])))), read<i32>(%[[VALUE_j]]))), read<i32>(%[[VALUE10]]));
// DEFAULT-NEXT:         write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_M2]]), const<i32>(1)))), const<i32>(0))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_M2]]), const<i32>(0)))), const<i32>(1)))));
// DEFAULT-NEXT:         for %[[VALUE11:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 let %[[VALUE_i_2:[0-9]+]] i: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:             condition: lt<i32>(read<i32>(%[[VALUE_i_2]]), sub<i32, overflow=ub>(read<i32>(%[[VALUE_len]]), const<i32>(1)))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE13:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE12]]), const<i32>(1));
// DEFAULT-NEXT:                 write<i32>(%[[VALUE_i_2]], read<i32>(%[[VALUE13]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 for %[[VALUE14:[0-9]+]]
// DEFAULT-NEXT:                     init:
// DEFAULT-NEXT:                         let %[[VALUE_j_2:[0-9]+]] j: i32 [storage=automatic] = const<i32>(0);
// DEFAULT-NEXT:                     condition: lt<i32>(read<i32>(%[[VALUE_j_2]]), read<i32>(%[[VALUE_len]]))
// DEFAULT-NEXT:                     increment: {
// DEFAULT-NEXT:                         let %[[VALUE15:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_j_2]]);
// DEFAULT-NEXT:                         let %[[VALUE16:[0-9]+]]: i32 [synthetic] = add<i32, overflow=ub>(read<i32>(%[[VALUE15]]), const<i32>(1));
// DEFAULT-NEXT:                         write<i32>(%[[VALUE_j_2]], read<i32>(%[[VALUE16]]));
// DEFAULT-NEXT:                         yield void;
// DEFAULT-NEXT:                     }
// DEFAULT-NEXT:                     body:
// DEFAULT-NEXT:                         if gt<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE1]]>>, subtract=false, element=vla<i32, %[[VALUE1]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE1]]>>, length=None>(%[[VALUE_M1]]), add<i32, overflow=ub>(read<i32>(%[[VALUE_i_2]]), const<i32>(1))))), read<i32>(%[[VALUE_j_2]])))), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE1]]>>, subtract=false, element=vla<i32, %[[VALUE1]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE1]]>>, length=None>(%[[VALUE_M1]]), read<i32>(%[[VALUE_i_2]])))), read<i32>(%[[VALUE_j_2]])))))
// DEFAULT-NEXT:                             write<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(64)>(%[[VALUE_data]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_M2]]), read<i32>(%[[VALUE_i_2]])))), read<i32>(%[[VALUE_j_2]])))))), read<i32>(%[[VALUE_i_2]]));
// DEFAULT-NEXT:         if ne<i32>(read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(64)>(%[[VALUE_data]]), read<i32>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=None>(deref(ptr_offset<ptr<vla<i32, %[[VALUE3]]>>, subtract=false, element=vla<i32, %[[VALUE3]]>, overflow=ub>(array_decay<ptr<vla<i32, %[[VALUE3]]>>, length=None>(%[[VALUE_M2]]), const<i32>(0)))), const<i32>(1))))))), const<i32>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
