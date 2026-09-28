/* This is a modfied version of va-arg-2.c to test passing a va_list as
   a parameter to another function.  */

#include <stdarg.h>

extern void          abort(void);
extern void          exit(int);
extern __SIZE_TYPE__ strlen(const char *);

int to_hex(unsigned int a) {
  static char hex[] = "0123456789abcdef";

  if (a > 15)
    abort();
  return hex[a];
}

void fap(int i, char *format, va_list ap) {
  if (strlen(format) != 16 - i)
    abort();
  while (*format)
    if (*format++ != to_hex(va_arg(ap, int)))
      abort();
}

void f0(char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(0, format, ap);
  va_end(ap);
}

void f1(int a1, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(1, format, ap);
  va_end(ap);
}

void f2(int a1, int a2, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(2, format, ap);
  va_end(ap);
}

void f3(int a1, int a2, int a3, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(3, format, ap);
  va_end(ap);
}

void f4(int a1, int a2, int a3, int a4, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(4, format, ap);
  va_end(ap);
}

void f5(int a1, int a2, int a3, int a4, int a5, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(5, format, ap);
  va_end(ap);
}

void f6(int a1, int a2, int a3, int a4, int a5, int a6, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(6, format, ap);
  va_end(ap);
}

void f7(int a1, int a2, int a3, int a4, int a5, int a6, int a7, char *format,
        ...) {
  va_list ap;

  va_start(ap, format);
  fap(7, format, ap);
  va_end(ap);
}

void f8(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8,
        char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(8, format, ap);
  va_end(ap);
}

void f9(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
        char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(9, format, ap);
  va_end(ap);
}

void f10(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(10, format, ap);
  va_end(ap);
}

void f11(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(11, format, ap);
  va_end(ap);
}

void f12(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(12, format, ap);
  va_end(ap);
}

void f13(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, int a13, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(13, format, ap);
  va_end(ap);
}

void f14(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, int a13, int a14, char *format, ...) {
  va_list ap;

  va_start(ap, format);
  fap(14, format, ap);
  va_end(ap);
}

void f15(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9,
         int a10, int a11, int a12, int a13, int a14, int a15, char *format,
         ...) {
  va_list ap;

  va_start(ap, format);
  fap(15, format, ap);
  va_end(ap);
}

int main(void) {
  char *f = "0123456789abcdef";

  f0(f + 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f1(0, f + 1, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f2(0, 1, f + 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f3(0, 1, 2, f + 3, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f4(0, 1, 2, 3, f + 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f5(0, 1, 2, 3, 4, f + 5, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f6(0, 1, 2, 3, 4, 5, f + 6, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f7(0, 1, 2, 3, 4, 5, 6, f + 7, 7, 8, 9, 10, 11, 12, 13, 14, 15);
  f8(0, 1, 2, 3, 4, 5, 6, 7, f + 8, 8, 9, 10, 11, 12, 13, 14, 15);
  f9(0, 1, 2, 3, 4, 5, 6, 7, 8, f + 9, 9, 10, 11, 12, 13, 14, 15);
  f10(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, f + 10, 10, 11, 12, 13, 14, 15);
  f11(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, f + 11, 11, 12, 13, 14, 15);
  f12(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, f + 12, 12, 13, 14, 15);
  f13(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, f + 13, 13, 14, 15);
  f14(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, f + 14, 14, 15);
  f15(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, f + 15, 15);

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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     global %7 hex: array<i8, 17> [storage=static] [align=16] = code_units<array<i8, 17>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %185 .str185: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%182 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @strlen(%183 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %5 @to_hex(%6 a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(17)>(%7), read<u32>(%6)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fap(%9 i: i32, %10 format: ptr<i8>, %11 ap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%10))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%9)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         while %184 ne<i8>(read<i8>(deref(read<ptr<i8>>(%10))), const<i8>(0))
// DEFAULT-NEXT:             let %186: ptr<i8> [synthetic] = read<ptr<i8>>(%10);
// DEFAULT-NEXT:             let %187: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%186), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%10, read<ptr<i8>>(%187));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%186)))), call<i32, signature=fn(u32) -> i32>(%5, reinterpret<u32, reason=arg, fits=unknown>(va_arg<i32>(%11))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %12 @f0(%13 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %14 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%14);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(0), read<ptr<i8>>(%13), read<va_list>(%14));
// DEFAULT-NEXT:         va_end(%14);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %15 @f1(%16 a1: i32, %17 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %18 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%18);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(1), read<ptr<i8>>(%17), read<va_list>(%18));
// DEFAULT-NEXT:         va_end(%18);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %19 @f2(%20 a1: i32, %21 a2: i32, %22 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %23 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%23);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(2), read<ptr<i8>>(%22), read<va_list>(%23));
// DEFAULT-NEXT:         va_end(%23);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %24 @f3(%25 a1: i32, %26 a2: i32, %27 a3: i32, %28 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %29 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%29);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(3), read<ptr<i8>>(%28), read<va_list>(%29));
// DEFAULT-NEXT:         va_end(%29);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %30 @f4(%31 a1: i32, %32 a2: i32, %33 a3: i32, %34 a4: i32, %35 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %36 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%36);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(4), read<ptr<i8>>(%35), read<va_list>(%36));
// DEFAULT-NEXT:         va_end(%36);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %37 @f5(%38 a1: i32, %39 a2: i32, %40 a3: i32, %41 a4: i32, %42 a5: i32, %43 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %44 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%44);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(5), read<ptr<i8>>(%43), read<va_list>(%44));
// DEFAULT-NEXT:         va_end(%44);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %45 @f6(%46 a1: i32, %47 a2: i32, %48 a3: i32, %49 a4: i32, %50 a5: i32, %51 a6: i32, %52 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %53 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%53);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(6), read<ptr<i8>>(%52), read<va_list>(%53));
// DEFAULT-NEXT:         va_end(%53);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %54 @f7(%55 a1: i32, %56 a2: i32, %57 a3: i32, %58 a4: i32, %59 a5: i32, %60 a6: i32, %61 a7: i32, %62 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %63 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%63);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(7), read<ptr<i8>>(%62), read<va_list>(%63));
// DEFAULT-NEXT:         va_end(%63);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %64 @f8(%65 a1: i32, %66 a2: i32, %67 a3: i32, %68 a4: i32, %69 a5: i32, %70 a6: i32, %71 a7: i32, %72 a8: i32, %73 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %74 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%74);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(8), read<ptr<i8>>(%73), read<va_list>(%74));
// DEFAULT-NEXT:         va_end(%74);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %75 @f9(%76 a1: i32, %77 a2: i32, %78 a3: i32, %79 a4: i32, %80 a5: i32, %81 a6: i32, %82 a7: i32, %83 a8: i32, %84 a9: i32, %85 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %86 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%86);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(9), read<ptr<i8>>(%85), read<va_list>(%86));
// DEFAULT-NEXT:         va_end(%86);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %87 @f10(%88 a1: i32, %89 a2: i32, %90 a3: i32, %91 a4: i32, %92 a5: i32, %93 a6: i32, %94 a7: i32, %95 a8: i32, %96 a9: i32, %97 a10: i32, %98 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %99 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%99);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(10), read<ptr<i8>>(%98), read<va_list>(%99));
// DEFAULT-NEXT:         va_end(%99);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %100 @f11(%101 a1: i32, %102 a2: i32, %103 a3: i32, %104 a4: i32, %105 a5: i32, %106 a6: i32, %107 a7: i32, %108 a8: i32, %109 a9: i32, %110 a10: i32, %111 a11: i32, %112 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %113 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%113);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(11), read<ptr<i8>>(%112), read<va_list>(%113));
// DEFAULT-NEXT:         va_end(%113);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %114 @f12(%115 a1: i32, %116 a2: i32, %117 a3: i32, %118 a4: i32, %119 a5: i32, %120 a6: i32, %121 a7: i32, %122 a8: i32, %123 a9: i32, %124 a10: i32, %125 a11: i32, %126 a12: i32, %127 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %128 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%128);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(12), read<ptr<i8>>(%127), read<va_list>(%128));
// DEFAULT-NEXT:         va_end(%128);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %129 @f13(%130 a1: i32, %131 a2: i32, %132 a3: i32, %133 a4: i32, %134 a5: i32, %135 a6: i32, %136 a7: i32, %137 a8: i32, %138 a9: i32, %139 a10: i32, %140 a11: i32, %141 a12: i32, %142 a13: i32, %143 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %144 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%144);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(13), read<ptr<i8>>(%143), read<va_list>(%144));
// DEFAULT-NEXT:         va_end(%144);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %145 @f14(%146 a1: i32, %147 a2: i32, %148 a3: i32, %149 a4: i32, %150 a5: i32, %151 a6: i32, %152 a7: i32, %153 a8: i32, %154 a9: i32, %155 a10: i32, %156 a11: i32, %157 a12: i32, %158 a13: i32, %159 a14: i32, %160 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %161 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%161);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(14), read<ptr<i8>>(%160), read<va_list>(%161));
// DEFAULT-NEXT:         va_end(%161);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %162 @f15(%163 a1: i32, %164 a2: i32, %165 a3: i32, %166 a4: i32, %167 a5: i32, %168 a6: i32, %169 a7: i32, %170 a8: i32, %171 a9: i32, %172 a10: i32, %173 a11: i32, %174 a12: i32, %175 a13: i32, %176 a14: i32, %177 a15: i32, %178 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %179 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%179);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(15), read<ptr<i8>>(%178), read<va_list>(%179));
// DEFAULT-NEXT:         va_end(%179);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %180 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %181 f: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(17)>(%185);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ...) -> void>(%12, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(0)), const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, ...) -> void>(%15, const<i32>(0), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(1)), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>, ...) -> void>(%19, const<i32>(0), const<i32>(1), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(2)), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, ptr<i8>, ...) -> void>(%24, const<i32>(0), const<i32>(1), const<i32>(2), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(3)), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ptr<i8>, ...) -> void>(%30, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(4)), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%37, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(5)), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%45, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(6)), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%54, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(7)), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%64, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(8)), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%75, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(9)), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%87, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(10)), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%100, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(11)), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%114, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(12)), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%129, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(13)), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%145, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(14)), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%162, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%181), const<i32>(15)), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
