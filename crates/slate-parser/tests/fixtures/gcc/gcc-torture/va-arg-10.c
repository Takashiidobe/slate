/* This is a modfied version of va-arg-9.c to test va_copy.  */

#include <stdarg.h>

void abort(void);
void exit(int);

#ifndef va_copy
#define va_copy __va_copy
#endif

extern __SIZE_TYPE__ strlen(const char *);

int to_hex(unsigned int a) {
  static char hex[] = "0123456789abcdef";

  if (a > 15)
    abort();
  return hex[a];
}

void fap(int i, char *format, va_list ap) {
  va_list apc;
  char   *formatc;

  va_copy(apc, ap);
  formatc = format;

  if (strlen(format) != 16 - i)
    abort();
  while (*format)
    if (*format++ != to_hex(va_arg(ap, int)))
      abort();
  while (*formatc)
    if (*formatc++ != to_hex(va_arg(apc, int)))
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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     global %6 hex: array<i8, 17> [storage=static] [align=16] = code_units<array<i8, 17>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %187 .str187: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external];
// DEFAULT-NEXT:     fn %2 @exit(%183 <unnamed>: i32) -> void [linkage=external];
// DEFAULT-NEXT:     fn %3 @strlen(%184 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @to_hex(%5 a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(17)>(%6), read<u32>(%5)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @fap(%8 i: i32, %9 format: ptr<i8>, %10 ap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %11 apc: va_list [storage=automatic];
// DEFAULT-NEXT:         let %12 formatc: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_copy(%11, %10);
// DEFAULT-NEXT:         write<ptr<i8>>(%12, read<ptr<i8>>(%9));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(strlen, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%9))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         while %185 ne<i8>(read<i8>(deref(read<ptr<i8>>(%9))), const<i8>(0))
// DEFAULT-NEXT:             let %188: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:             let %189: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%188), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%9, read<ptr<i8>>(%189));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%188)))), call<i32, signature=fn(u32) -> i32>(%4, reinterpret<u32, reason=arg, fits=unknown>(va_arg<i32>(%10))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:         while %186 ne<i8>(read<i8>(deref(read<ptr<i8>>(%12))), const<i8>(0))
// DEFAULT-NEXT:             let %190: ptr<i8> [synthetic] = read<ptr<i8>>(%12);
// DEFAULT-NEXT:             let %191: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%190), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%12, read<ptr<i8>>(%191));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%190)))), call<i32, signature=fn(u32) -> i32>(%4, reinterpret<u32, reason=arg, fits=unknown>(va_arg<i32>(%11))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(abort);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %13 @f0(%14 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %15 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%15);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(0), read<ptr<i8>>(%14), read<va_list>(%15));
// DEFAULT-NEXT:         va_end(%15);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %16 @f1(%17 a1: i32, %18 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %19 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%19);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(1), read<ptr<i8>>(%18), read<va_list>(%19));
// DEFAULT-NEXT:         va_end(%19);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %20 @f2(%21 a1: i32, %22 a2: i32, %23 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %24 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%24);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(2), read<ptr<i8>>(%23), read<va_list>(%24));
// DEFAULT-NEXT:         va_end(%24);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %25 @f3(%26 a1: i32, %27 a2: i32, %28 a3: i32, %29 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %30 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%30);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(3), read<ptr<i8>>(%29), read<va_list>(%30));
// DEFAULT-NEXT:         va_end(%30);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %31 @f4(%32 a1: i32, %33 a2: i32, %34 a3: i32, %35 a4: i32, %36 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %37 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%37);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(4), read<ptr<i8>>(%36), read<va_list>(%37));
// DEFAULT-NEXT:         va_end(%37);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %38 @f5(%39 a1: i32, %40 a2: i32, %41 a3: i32, %42 a4: i32, %43 a5: i32, %44 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %45 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%45);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(5), read<ptr<i8>>(%44), read<va_list>(%45));
// DEFAULT-NEXT:         va_end(%45);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @f6(%47 a1: i32, %48 a2: i32, %49 a3: i32, %50 a4: i32, %51 a5: i32, %52 a6: i32, %53 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %54 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%54);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(6), read<ptr<i8>>(%53), read<va_list>(%54));
// DEFAULT-NEXT:         va_end(%54);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %55 @f7(%56 a1: i32, %57 a2: i32, %58 a3: i32, %59 a4: i32, %60 a5: i32, %61 a6: i32, %62 a7: i32, %63 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %64 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%64);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(7), read<ptr<i8>>(%63), read<va_list>(%64));
// DEFAULT-NEXT:         va_end(%64);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %65 @f8(%66 a1: i32, %67 a2: i32, %68 a3: i32, %69 a4: i32, %70 a5: i32, %71 a6: i32, %72 a7: i32, %73 a8: i32, %74 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %75 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%75);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(8), read<ptr<i8>>(%74), read<va_list>(%75));
// DEFAULT-NEXT:         va_end(%75);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %76 @f9(%77 a1: i32, %78 a2: i32, %79 a3: i32, %80 a4: i32, %81 a5: i32, %82 a6: i32, %83 a7: i32, %84 a8: i32, %85 a9: i32, %86 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %87 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%87);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(9), read<ptr<i8>>(%86), read<va_list>(%87));
// DEFAULT-NEXT:         va_end(%87);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %88 @f10(%89 a1: i32, %90 a2: i32, %91 a3: i32, %92 a4: i32, %93 a5: i32, %94 a6: i32, %95 a7: i32, %96 a8: i32, %97 a9: i32, %98 a10: i32, %99 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %100 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%100);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(10), read<ptr<i8>>(%99), read<va_list>(%100));
// DEFAULT-NEXT:         va_end(%100);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %101 @f11(%102 a1: i32, %103 a2: i32, %104 a3: i32, %105 a4: i32, %106 a5: i32, %107 a6: i32, %108 a7: i32, %109 a8: i32, %110 a9: i32, %111 a10: i32, %112 a11: i32, %113 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %114 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%114);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(11), read<ptr<i8>>(%113), read<va_list>(%114));
// DEFAULT-NEXT:         va_end(%114);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %115 @f12(%116 a1: i32, %117 a2: i32, %118 a3: i32, %119 a4: i32, %120 a5: i32, %121 a6: i32, %122 a7: i32, %123 a8: i32, %124 a9: i32, %125 a10: i32, %126 a11: i32, %127 a12: i32, %128 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %129 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%129);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(12), read<ptr<i8>>(%128), read<va_list>(%129));
// DEFAULT-NEXT:         va_end(%129);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %130 @f13(%131 a1: i32, %132 a2: i32, %133 a3: i32, %134 a4: i32, %135 a5: i32, %136 a6: i32, %137 a7: i32, %138 a8: i32, %139 a9: i32, %140 a10: i32, %141 a11: i32, %142 a12: i32, %143 a13: i32, %144 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %145 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%145);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(13), read<ptr<i8>>(%144), read<va_list>(%145));
// DEFAULT-NEXT:         va_end(%145);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %146 @f14(%147 a1: i32, %148 a2: i32, %149 a3: i32, %150 a4: i32, %151 a5: i32, %152 a6: i32, %153 a7: i32, %154 a8: i32, %155 a9: i32, %156 a10: i32, %157 a11: i32, %158 a12: i32, %159 a13: i32, %160 a14: i32, %161 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %162 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%162);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(14), read<ptr<i8>>(%161), read<va_list>(%162));
// DEFAULT-NEXT:         va_end(%162);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %163 @f15(%164 a1: i32, %165 a2: i32, %166 a3: i32, %167 a4: i32, %168 a5: i32, %169 a6: i32, %170 a7: i32, %171 a8: i32, %172 a9: i32, %173 a10: i32, %174 a11: i32, %175 a12: i32, %176 a13: i32, %177 a14: i32, %178 a15: i32, %179 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %180 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%180);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(15), read<ptr<i8>>(%179), read<va_list>(%180));
// DEFAULT-NEXT:         va_end(%180);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %181 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %182 f: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(17)>(%187);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ...) -> void>(%13, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(0)), const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, ...) -> void>(%16, const<i32>(0), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(1)), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>, ...) -> void>(%20, const<i32>(0), const<i32>(1), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(2)), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, ptr<i8>, ...) -> void>(%25, const<i32>(0), const<i32>(1), const<i32>(2), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(3)), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ptr<i8>, ...) -> void>(%31, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(4)), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%38, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(5)), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%46, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(6)), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%55, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(7)), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%65, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(8)), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%76, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(9)), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%88, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(10)), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%101, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(11)), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%115, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(12)), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%130, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(13)), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%146, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(14)), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%163, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%182), const<i32>(15)), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(exit, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
