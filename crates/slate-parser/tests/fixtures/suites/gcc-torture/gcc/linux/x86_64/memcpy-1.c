/* { dg-add-options stack_size } */

#include <string.h>

void abort(void);
void exit(int);

#if defined(STACK_SIZE)
#define MEMCPY_SIZE (STACK_SIZE / 3)
#else
#define MEMCPY_SIZE (1 << 17)
#endif

void *copy(void *o, const void *i, unsigned l) { return memcpy(o, i, l); }

int main(void) {
  unsigned      i;
  unsigned char src[MEMCPY_SIZE];
  unsigned char dst[MEMCPY_SIZE];

  for (i = 0; i < MEMCPY_SIZE; i++)
    src[i] = (unsigned char)i, dst[i] = 0;

  (void)memcpy(dst, src, MEMCPY_SIZE / 128);

  for (i = 0; i < MEMCPY_SIZE / 128; i++)
    if (dst[i] != (unsigned char)i)
      abort();

  (void)memset(dst, 1, MEMCPY_SIZE / 128);

  for (i = 0; i < MEMCPY_SIZE / 128; i++)
    if (dst[i] != 1)
      abort();

  (void)memcpy(dst, src, MEMCPY_SIZE);

  for (i = 0; i < MEMCPY_SIZE; i++)
    if (dst[i] != (unsigned char)i)
      abort();

  (void)memset(dst, 0, MEMCPY_SIZE);

  for (i = 0; i < MEMCPY_SIZE; i++)
    if (dst[i] != 0)
      abort();

  (void)copy(dst, src, MEMCPY_SIZE / 128);

  for (i = 0; i < MEMCPY_SIZE / 128; i++)
    if (dst[i] != (unsigned char)i)
      abort();

  (void)memset(dst, 0, MEMCPY_SIZE);

  (void)copy(dst, src, MEMCPY_SIZE);

  for (i = 0; i < MEMCPY_SIZE; i++)
    if (dst[i] != (unsigned char)i)
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
// DEFAULT-NEXT:     fn %[[VALUE_memcpy:[0-9]+]] @memcpy(%[[VALUE___dest:[0-9]+]] __dest: ptr<void> [restrict], %[[VALUE___src:[0-9]+]] __src: ptr<const void> [restrict], %[[VALUE___n:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_memset:[0-9]+]] @memset(%[[VALUE___s:[0-9]+]] __s: ptr<void>, %[[VALUE___c:[0-9]+]] __c: i32, %[[VALUE___n_2:[0-9]+]] __n: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_copy:[0-9]+]] @copy(%[[VALUE_o:[0-9]+]] o: ptr<void>, %[[VALUE_i:[0-9]+]] i: ptr<const void>, %[[VALUE_l:[0-9]+]] l: u32) -> ptr<void> [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], read<ptr<void>>(%[[VALUE_o]]), read<ptr<const void>>(%[[VALUE_i]]), widen<u64, reason=arg>(read<u32>(%[[VALUE_l]])));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_i_2:[0-9]+]] i: u32 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_src:[0-9]+]] src: array<u8, 131072> [storage=automatic] [align=16];
// DEFAULT-NEXT:         let %[[VALUE_dst:[0-9]+]] dst: array<u8, 131072> [storage=automatic] [align=16];
// DEFAULT-NEXT:         for %[[VALUE1:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE2:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE3:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE2]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], read<u32>(%[[VALUE3]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_src]]), read<u32>(%[[VALUE_i_2]]))), truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_i_2]])));
// DEFAULT-NEXT:                 write<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]]), read<u32>(%[[VALUE_i_2]]))), reinterpret<u8, reason=assign, fits=unknown>(truncate<i8, reason=assign, fits=always>(const<i32>(0))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(div<i32, by_zero=ub, min_by_neg_one=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)), const<i32>(128)))));
// DEFAULT-NEXT:         for %[[VALUE4:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)), const<i32>(128))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE5:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE6:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE5]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], read<u32>(%[[VALUE6]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]]), read<u32>(%[[VALUE_i_2]])))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_i_2]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]])), const<i32>(1), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(div<i32, by_zero=ub, min_by_neg_one=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)), const<i32>(128)))));
// DEFAULT-NEXT:         for %[[VALUE7:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)), const<i32>(128))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE8:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE9:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE8]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], read<u32>(%[[VALUE9]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]]), read<u32>(%[[VALUE_i_2]])))))), const<i32>(1))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u64) -> ptr<void>>(%[[VALUE_memcpy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_src]])), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)))));
// DEFAULT-NEXT:         for %[[VALUE10:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE11:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE12:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE11]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], read<u32>(%[[VALUE12]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]]), read<u32>(%[[VALUE_i_2]])))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_i_2]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)))));
// DEFAULT-NEXT:         for %[[VALUE13:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE14:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE15:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE14]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], read<u32>(%[[VALUE15]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]]), read<u32>(%[[VALUE_i_2]])))))), const<i32>(0))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u32) -> ptr<void>>(%[[VALUE_copy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_src]])), reinterpret<u32, reason=arg, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)), const<i32>(128))));
// DEFAULT-NEXT:         for %[[VALUE16:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(div<i32, by_zero=ub, min_by_neg_one=ub>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)), const<i32>(128))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE17:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE18:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE17]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], read<u32>(%[[VALUE18]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]]), read<u32>(%[[VALUE_i_2]])))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_i_2]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(%[[VALUE_memset]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]])), const<i32>(0), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17)))));
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, ptr<const void>, u32) -> ptr<void>>(%[[VALUE_copy]], pointer_cast<ptr<void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]])), pointer_cast<ptr<const void>, reason=arg>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_src]])), reinterpret<u32, reason=arg, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17))));
// DEFAULT-NEXT:         for %[[VALUE19:[0-9]+]]
// DEFAULT-NEXT:             init:
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], reinterpret<u32, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:             condition: lt<u32>(read<u32>(%[[VALUE_i_2]]), reinterpret<u32, reason=usual_arith, fits=unknown>(shl<i32, overflow=ub, amount_out_of_range=ub, negative_left=ub>(const<i32>(1), const<i32>(17))))
// DEFAULT-NEXT:             increment: {
// DEFAULT-NEXT:                 let %[[VALUE20:[0-9]+]]: u32 [synthetic] = read<u32>(%[[VALUE_i_2]]);
// DEFAULT-NEXT:                 let %[[VALUE21:[0-9]+]]: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%[[VALUE20]]), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:                 write<u32>(%[[VALUE_i_2]], read<u32>(%[[VALUE21]]));
// DEFAULT-NEXT:                 yield void;
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:             body:
// DEFAULT-NEXT:                 if ne<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(131072)>(%[[VALUE_dst]]), read<u32>(%[[VALUE_i_2]])))))), reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(truncate<u8, reason=explicit, fits=unknown>(read<u32>(%[[VALUE_i_2]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
