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
// DEFAULT-NEXT:     type @type0 __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type1 va_list = va_list;
// DEFAULT-NEXT:     global %7 hex: array<i8, 17> [storage=static] [align=16] = code_units<array<i8, 17>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %188 .str188: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %2 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @exit(%184 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %4 @strlen(%185 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %5 @to_hex(%6 a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%6), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(17)>(%7), read<u32>(%6)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %8 @fap(%9 i: i32, %10 format: ptr<i8>, %11 ap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %12 apc: va_list [storage=automatic];
// DEFAULT-NEXT:         let %13 formatc: ptr<i8> [storage=automatic];
// DEFAULT-NEXT:         va_copy(%12, %11);
// DEFAULT-NEXT:         write<ptr<i8>>(%13, read<ptr<i8>>(%10));
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%4, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%10))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%9)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         while %186 ne<i8>(read<i8>(deref(read<ptr<i8>>(%10))), const<i8>(0))
// DEFAULT-NEXT:             let %189: ptr<i8> [synthetic] = read<ptr<i8>>(%10);
// DEFAULT-NEXT:             let %190: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%189), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%10, read<ptr<i8>>(%190));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%189)))), call<i32, signature=fn(u32) -> i32>(%5, reinterpret<u32, reason=arg, fits=unknown>(va_arg<i32>(%11))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:         while %187 ne<i8>(read<i8>(deref(read<ptr<i8>>(%13))), const<i8>(0))
// DEFAULT-NEXT:             let %191: ptr<i8> [synthetic] = read<ptr<i8>>(%13);
// DEFAULT-NEXT:             let %192: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%191), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%13, read<ptr<i8>>(%192));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%191)))), call<i32, signature=fn(u32) -> i32>(%5, reinterpret<u32, reason=arg, fits=unknown>(va_arg<i32>(%12))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%2);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f0(%15 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %16 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%16);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(0), read<ptr<i8>>(%15), read<va_list>(%16));
// DEFAULT-NEXT:         va_end(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %17 @f1(%18 a1: i32, %19 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %20 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%20);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(1), read<ptr<i8>>(%19), read<va_list>(%20));
// DEFAULT-NEXT:         va_end(%20);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %21 @f2(%22 a1: i32, %23 a2: i32, %24 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %25 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%25);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(2), read<ptr<i8>>(%24), read<va_list>(%25));
// DEFAULT-NEXT:         va_end(%25);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @f3(%27 a1: i32, %28 a2: i32, %29 a3: i32, %30 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %31 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%31);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(3), read<ptr<i8>>(%30), read<va_list>(%31));
// DEFAULT-NEXT:         va_end(%31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @f4(%33 a1: i32, %34 a2: i32, %35 a3: i32, %36 a4: i32, %37 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %38 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%38);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(4), read<ptr<i8>>(%37), read<va_list>(%38));
// DEFAULT-NEXT:         va_end(%38);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @f5(%40 a1: i32, %41 a2: i32, %42 a3: i32, %43 a4: i32, %44 a5: i32, %45 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %46 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%46);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(5), read<ptr<i8>>(%45), read<va_list>(%46));
// DEFAULT-NEXT:         va_end(%46);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %47 @f6(%48 a1: i32, %49 a2: i32, %50 a3: i32, %51 a4: i32, %52 a5: i32, %53 a6: i32, %54 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %55 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%55);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(6), read<ptr<i8>>(%54), read<va_list>(%55));
// DEFAULT-NEXT:         va_end(%55);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %56 @f7(%57 a1: i32, %58 a2: i32, %59 a3: i32, %60 a4: i32, %61 a5: i32, %62 a6: i32, %63 a7: i32, %64 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %65 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%65);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(7), read<ptr<i8>>(%64), read<va_list>(%65));
// DEFAULT-NEXT:         va_end(%65);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %66 @f8(%67 a1: i32, %68 a2: i32, %69 a3: i32, %70 a4: i32, %71 a5: i32, %72 a6: i32, %73 a7: i32, %74 a8: i32, %75 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %76 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%76);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(8), read<ptr<i8>>(%75), read<va_list>(%76));
// DEFAULT-NEXT:         va_end(%76);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %77 @f9(%78 a1: i32, %79 a2: i32, %80 a3: i32, %81 a4: i32, %82 a5: i32, %83 a6: i32, %84 a7: i32, %85 a8: i32, %86 a9: i32, %87 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %88 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%88);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(9), read<ptr<i8>>(%87), read<va_list>(%88));
// DEFAULT-NEXT:         va_end(%88);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %89 @f10(%90 a1: i32, %91 a2: i32, %92 a3: i32, %93 a4: i32, %94 a5: i32, %95 a6: i32, %96 a7: i32, %97 a8: i32, %98 a9: i32, %99 a10: i32, %100 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %101 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%101);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(10), read<ptr<i8>>(%100), read<va_list>(%101));
// DEFAULT-NEXT:         va_end(%101);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %102 @f11(%103 a1: i32, %104 a2: i32, %105 a3: i32, %106 a4: i32, %107 a5: i32, %108 a6: i32, %109 a7: i32, %110 a8: i32, %111 a9: i32, %112 a10: i32, %113 a11: i32, %114 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %115 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%115);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(11), read<ptr<i8>>(%114), read<va_list>(%115));
// DEFAULT-NEXT:         va_end(%115);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %116 @f12(%117 a1: i32, %118 a2: i32, %119 a3: i32, %120 a4: i32, %121 a5: i32, %122 a6: i32, %123 a7: i32, %124 a8: i32, %125 a9: i32, %126 a10: i32, %127 a11: i32, %128 a12: i32, %129 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %130 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%130);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(12), read<ptr<i8>>(%129), read<va_list>(%130));
// DEFAULT-NEXT:         va_end(%130);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %131 @f13(%132 a1: i32, %133 a2: i32, %134 a3: i32, %135 a4: i32, %136 a5: i32, %137 a6: i32, %138 a7: i32, %139 a8: i32, %140 a9: i32, %141 a10: i32, %142 a11: i32, %143 a12: i32, %144 a13: i32, %145 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %146 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%146);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(13), read<ptr<i8>>(%145), read<va_list>(%146));
// DEFAULT-NEXT:         va_end(%146);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %147 @f14(%148 a1: i32, %149 a2: i32, %150 a3: i32, %151 a4: i32, %152 a5: i32, %153 a6: i32, %154 a7: i32, %155 a8: i32, %156 a9: i32, %157 a10: i32, %158 a11: i32, %159 a12: i32, %160 a13: i32, %161 a14: i32, %162 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %163 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%163);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(14), read<ptr<i8>>(%162), read<va_list>(%163));
// DEFAULT-NEXT:         va_end(%163);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %164 @f15(%165 a1: i32, %166 a2: i32, %167 a3: i32, %168 a4: i32, %169 a5: i32, %170 a6: i32, %171 a7: i32, %172 a8: i32, %173 a9: i32, %174 a10: i32, %175 a11: i32, %176 a12: i32, %177 a13: i32, %178 a14: i32, %179 a15: i32, %180 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %181 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%181);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%8, const<i32>(15), read<ptr<i8>>(%180), read<va_list>(%181));
// DEFAULT-NEXT:         va_end(%181);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %182 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %183 f: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(17)>(%188);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ...) -> void>(%14, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(0)), const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, ...) -> void>(%17, const<i32>(0), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(1)), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>, ...) -> void>(%21, const<i32>(0), const<i32>(1), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(2)), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, ptr<i8>, ...) -> void>(%26, const<i32>(0), const<i32>(1), const<i32>(2), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(3)), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ptr<i8>, ...) -> void>(%32, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(4)), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%39, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(5)), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%47, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(6)), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%56, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(7)), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%66, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(8)), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%77, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(9)), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%89, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(10)), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%102, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(11)), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%116, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(12)), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%131, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(13)), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%147, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(14)), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%164, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%183), const<i32>(15)), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%3, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
