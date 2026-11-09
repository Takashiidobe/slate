/* PR109970
 * { dg-do compile }
 * { dg-options "-Wstringop-overflow" }
 * */

void bar(int x, char buf[x]);
void foo(int x; char buf[x], int x);

int main()
{
	char buf[10];
	bar(11, buf);	/* { dg-warning "accessing 11 bytes in a region of size 10" } */
	foo(buf, 11);	/* { dg-warning "accessing 11 bytes in a region of size 10" } */
}

// SLATE-FILECHECK-STD DEFAULT gnu23
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
// DEFAULT-NEXT:     fn %[[VALUE_bar:[0-9]+]] @bar(%[[VALUE_x:[0-9]+]] x: i32, %[[VALUE_buf:[0-9]+]] buf: ptr<i8> [array=*]) -> void [linkage=external] [c="void(int, char[*])"];
// DEFAULT-NEXT:     fn %[[VALUE_foo:[0-9]+]] @foo(%[[VALUE_buf_2:[0-9]+]] buf: ptr<i8> [array=*], %[[VALUE_x_2:[0-9]+]] x: i32) -> void [linkage=external] [c="void(char[*], int)"];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_buf_3:[0-9]+]] buf: array<i8, 10> [storage=automatic];
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>) -> void>(%[[VALUE_bar]], const<i32>(11), array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_buf_3]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, i32) -> void>(%[[VALUE_foo]], array_decay<ptr<i8>, length=Some(10)>(%[[VALUE_buf_3]]), const<i32>(11));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
