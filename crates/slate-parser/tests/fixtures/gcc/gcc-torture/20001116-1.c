// SLATE-FILECHECK-DEFINES DEFAULT

int x[60];
char *y = ((char*)&(x[2*8 + 2]) - 8);
int z = (&"Foobar"[1] - &"Foobar"[0]);

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
// DEFAULT-NEXT:     global %0 x: array<i32, 60> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %1 y: ptr<i8> [storage=static] = ptr_offset<ptr<i8>, subtract=true, element=i8, overflow=ub>(pointer_cast<ptr<i8>, reason=explicit>(addr_of<ptr<i32>>(deref(ptr_offset<ptr<i32>, subtract=false, element=i32, overflow=ub>(array_decay<ptr<i32>, length=Some(60)>(%0), add<i32, overflow=ub>(mul<i32, overflow=ub>(const<i32>(2), const<i32>(8)), const<i32>(2)))))), const<i32>(8)) [linkage=external];
// DEFAULT-NEXT:     global %3 .str3: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([70, 111, 111, 98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %4 .str4: array<i8, 7> [storage=static] = code_units<array<i8, 7>>([70, 111, 111, 98, 97, 114, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %2 z: i32 [storage=static] = truncate<i32, reason=assign, fits=unknown>(ptr_diff<i64, element=i8, same_array=required, overflow=ub>(addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%3), const<i32>(1)))), addr_of<ptr<i8>>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(7)>(%4), const<i32>(0)))))) [linkage=external];
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
