void abort(void);
#if (__SIZEOF_INT__ == 4)
typedef int int32;
#elif (__SIZEOF_LONG__ == 4)
typedef long int32;
#else
#error Add target support for int32
#endif
static int32 ref(void) {
  union {
    char  c[5];
    int32 i;
  } u;

  __builtin_memset(&u, 0, sizeof(u));
  u.c[0] = 1;
  u.c[1] = 2;
  u.c[2] = 3;
  u.c[3] = 4;

  return u.i;
}

int main() {
  int32 b = ref();
  if (b != 0x01020304 && b != 0x04030201)
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
// DEFAULT-NEXT:     type @type0 int32 = i32;
// DEFAULT-NEXT:     type @type1 = union {
// DEFAULT-NEXT:         field0 c: array<i8, 5>;
// DEFAULT-NEXT:         field1 i: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     fn %0 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @ref() -> i32 [linkage=internal] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %4 u: @type1 [storage=automatic];
// DEFAULT-NEXT:         call<ptr<void>, signature=fn(ptr<void>, i32, u64) -> ptr<void>>(__builtin_memset, pointer_cast<ptr<void>, reason=arg>(addr_of<ptr<@type1>>(%4)), const<i32>(0), const<u64>(8));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%4)), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%4)), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(2)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%4)), const<i32>(2))), truncate<i8, reason=assign, fits=always>(const<i32>(3)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(5)>(field0(%4)), const<i32>(3))), truncate<i8, reason=assign, fits=always>(const<i32>(4)));
// DEFAULT-NEXT:         return read<i32>(field1(%4));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %5 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %6 b: i32 [storage=automatic] = call<i32, signature=fn() -> i32>(%2);
// DEFAULT-NEXT:         if logical_and<bool>(ne<i32>(read<i32>(%6), const<i32>(16909060)), ne<i32>(read<i32>(%6), const<i32>(67305985)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%0);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
