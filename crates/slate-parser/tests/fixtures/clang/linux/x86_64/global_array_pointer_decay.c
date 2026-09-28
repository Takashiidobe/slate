#include <stdint.h>
#include <stdio.h>

static uint32_t values[3]                      = {1, 2, 3};
_Alignas(32) static uint32_t aligned_values[3] = {4, 5, 6};

static uint32_t update(uint32_t *items) {
  items[0] += 10;
  return items[0] + items[2];
}

static uint32_t middle(uint32_t (*items)[3]) { return (*items)[1]; }

int main(void) {
  uint32_t first  = update(values);
  uint32_t second = update(aligned_values);
  printf("%u %u %u\n", first, second, middle(&values));
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
// DEFAULT-NEXT:     type @type0 __uint32_t = u32;
// DEFAULT-NEXT:     type @type1 uint32_t = u32;
// DEFAULT-NEXT:     global %4 values: array<u32, 3> [storage=static] = aggregate<array<u32, 3>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(1)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(2)), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(3))) [linkage=internal];
// DEFAULT-NEXT:     global %5 aligned_values: array<u32, 3> [storage=static] [align=32] = aggregate<array<u32, 3>, zero_fill=false>(index0 = reinterpret<u32, reason=assign, fits=always>(const<i32>(4)), index1 = reinterpret<u32, reason=assign, fits=always>(const<i32>(5)), index2 = reinterpret<u32, reason=assign, fits=always>(const<i32>(6))) [linkage=internal];
// DEFAULT-NEXT:     global %14 .str14: array<i8, 10> [storage=static] = code_units<array<i8, 10>>([37, 117, 32, 37, 117, 32, 37, 117, 10, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %3 @printf(%13 __format: ptr<const i8> [restrict], ...) -> i32 [linkage=external];
// DEFAULT-NEXT:     fn %6 @update(%7 items: ptr<u32>) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %15: ptr<u32> [synthetic] = ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%7), const<i32>(0));
// DEFAULT-NEXT:         let %16: u32 [synthetic] = read<u32>(deref(read<ptr<u32>>(%15)));
// DEFAULT-NEXT:         let %17: u32 [synthetic] = add<u32, overflow=wrap>(read<u32>(%16), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(10)));
// DEFAULT-NEXT:         write<u32>(deref(read<ptr<u32>>(%15)), read<u32>(%17));
// DEFAULT-NEXT:         return add<u32, overflow=wrap>(read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%7), const<i32>(0)))), read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(read<ptr<u32>>(%7), const<i32>(2)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @middle(%9 items: ptr<array<u32, 3>>) -> u32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<u32>(deref(ptr_offset<ptr<u32>, subtract=false, element=u32, overflow=ub>(array_decay<ptr<u32>, length=Some(3)>(deref(read<ptr<array<u32, 3>>>(%9))), const<i32>(1))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %10 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %11 first: u32 [storage=automatic] = call<u32, signature=fn(ptr<u32>) -> u32>(%6, array_decay<ptr<u32>, length=Some(3)>(%4));
// DEFAULT-NEXT:         let %12 second: u32 [storage=automatic] = call<u32, signature=fn(ptr<u32>) -> u32>(%6, array_decay<ptr<u32>, length=Some(3)>(%5));
// DEFAULT-NEXT:         call<i32, signature=fn(ptr<const i8>, ...) -> i32>(%3, pointer_cast<ptr<const i8>, reason=arg>(array_decay<ptr<i8>, length=Some(10)>(%14)), read<u32>(%11), read<u32>(%12), call<u32, signature=fn(ptr<array<u32, 3>>) -> u32>(%8, addr_of<ptr<array<u32, 3>>>(%4)));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
