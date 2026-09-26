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
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     global %6 hex: array<i8, 17> [storage=static] [align=16] = code_units<array<i8, 17>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %184 .str184: array<i8, 17> [storage=static] = code_units<array<i8, 17>>([48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 97, 98, 99, 100, 101, 102, 0]) [linkage=internal];
// DEFAULT-NEXT:     fn %1 @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %2 @exit(%181 <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %3 @strlen(%182 <unnamed>: ptr<const i8>) -> u64 [linkage=external];
// DEFAULT-NEXT:     fn %4 @to_hex(%5 a: u32) -> i32 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         if gt<u32>(read<u32>(%5), reinterpret<u32, reason=usual_arith, fits=always>(const<i32>(15)))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         return widen<i32, reason=return>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(17)>(%6), read<u32>(%5)))));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %7 @fap(%8 i: i32, %9 format: ptr<i8>, %10 ap: va_list) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u64>(call<u64, signature=fn(ptr<const i8>) -> u64>(%3, pointer_cast<ptr<const i8>, reason=arg>(read<ptr<i8>>(%9))), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(sub<i32, overflow=ub>(const<i32>(16), read<i32>(%8)))))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:         while %183 ne<i8>(read<i8>(deref(read<ptr<i8>>(%9))), const<i8>(0))
// DEFAULT-NEXT:             let %185: ptr<i8> [synthetic] = read<ptr<i8>>(%9);
// DEFAULT-NEXT:             let %186: ptr<i8> [synthetic] = ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%185), const<i32>(1));
// DEFAULT-NEXT:             write<ptr<i8>>(%9, read<ptr<i8>>(%186));
// DEFAULT-NEXT:             if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(read<ptr<i8>>(%185)))), call<i32, signature=fn(u32) -> i32>(%4, reinterpret<u32, reason=arg, fits=unknown>(va_arg<i32>(%10))))
// DEFAULT-NEXT:                 call<void, signature=fn() -> void>(%1);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %11 @f0(%12 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %13 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%13);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(0), read<ptr<i8>>(%12), read<va_list>(%13));
// DEFAULT-NEXT:         va_end(%13);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %14 @f1(%15 a1: i32, %16 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %17 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%17);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(1), read<ptr<i8>>(%16), read<va_list>(%17));
// DEFAULT-NEXT:         va_end(%17);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f2(%19 a1: i32, %20 a2: i32, %21 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %22 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%22);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(2), read<ptr<i8>>(%21), read<va_list>(%22));
// DEFAULT-NEXT:         va_end(%22);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %23 @f3(%24 a1: i32, %25 a2: i32, %26 a3: i32, %27 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %28 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%28);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(3), read<ptr<i8>>(%27), read<va_list>(%28));
// DEFAULT-NEXT:         va_end(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @f4(%30 a1: i32, %31 a2: i32, %32 a3: i32, %33 a4: i32, %34 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %35 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%35);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(4), read<ptr<i8>>(%34), read<va_list>(%35));
// DEFAULT-NEXT:         va_end(%35);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %36 @f5(%37 a1: i32, %38 a2: i32, %39 a3: i32, %40 a4: i32, %41 a5: i32, %42 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %43 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%43);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(5), read<ptr<i8>>(%42), read<va_list>(%43));
// DEFAULT-NEXT:         va_end(%43);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %44 @f6(%45 a1: i32, %46 a2: i32, %47 a3: i32, %48 a4: i32, %49 a5: i32, %50 a6: i32, %51 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %52 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%52);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(6), read<ptr<i8>>(%51), read<va_list>(%52));
// DEFAULT-NEXT:         va_end(%52);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @f7(%54 a1: i32, %55 a2: i32, %56 a3: i32, %57 a4: i32, %58 a5: i32, %59 a6: i32, %60 a7: i32, %61 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %62 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%62);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(7), read<ptr<i8>>(%61), read<va_list>(%62));
// DEFAULT-NEXT:         va_end(%62);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @f8(%64 a1: i32, %65 a2: i32, %66 a3: i32, %67 a4: i32, %68 a5: i32, %69 a6: i32, %70 a7: i32, %71 a8: i32, %72 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %73 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%73);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(8), read<ptr<i8>>(%72), read<va_list>(%73));
// DEFAULT-NEXT:         va_end(%73);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %74 @f9(%75 a1: i32, %76 a2: i32, %77 a3: i32, %78 a4: i32, %79 a5: i32, %80 a6: i32, %81 a7: i32, %82 a8: i32, %83 a9: i32, %84 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %85 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%85);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(9), read<ptr<i8>>(%84), read<va_list>(%85));
// DEFAULT-NEXT:         va_end(%85);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %86 @f10(%87 a1: i32, %88 a2: i32, %89 a3: i32, %90 a4: i32, %91 a5: i32, %92 a6: i32, %93 a7: i32, %94 a8: i32, %95 a9: i32, %96 a10: i32, %97 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %98 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%98);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(10), read<ptr<i8>>(%97), read<va_list>(%98));
// DEFAULT-NEXT:         va_end(%98);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %99 @f11(%100 a1: i32, %101 a2: i32, %102 a3: i32, %103 a4: i32, %104 a5: i32, %105 a6: i32, %106 a7: i32, %107 a8: i32, %108 a9: i32, %109 a10: i32, %110 a11: i32, %111 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %112 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%112);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(11), read<ptr<i8>>(%111), read<va_list>(%112));
// DEFAULT-NEXT:         va_end(%112);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %113 @f12(%114 a1: i32, %115 a2: i32, %116 a3: i32, %117 a4: i32, %118 a5: i32, %119 a6: i32, %120 a7: i32, %121 a8: i32, %122 a9: i32, %123 a10: i32, %124 a11: i32, %125 a12: i32, %126 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %127 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%127);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(12), read<ptr<i8>>(%126), read<va_list>(%127));
// DEFAULT-NEXT:         va_end(%127);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %128 @f13(%129 a1: i32, %130 a2: i32, %131 a3: i32, %132 a4: i32, %133 a5: i32, %134 a6: i32, %135 a7: i32, %136 a8: i32, %137 a9: i32, %138 a10: i32, %139 a11: i32, %140 a12: i32, %141 a13: i32, %142 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %143 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%143);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(13), read<ptr<i8>>(%142), read<va_list>(%143));
// DEFAULT-NEXT:         va_end(%143);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %144 @f14(%145 a1: i32, %146 a2: i32, %147 a3: i32, %148 a4: i32, %149 a5: i32, %150 a6: i32, %151 a7: i32, %152 a8: i32, %153 a9: i32, %154 a10: i32, %155 a11: i32, %156 a12: i32, %157 a13: i32, %158 a14: i32, %159 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %160 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%160);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(14), read<ptr<i8>>(%159), read<va_list>(%160));
// DEFAULT-NEXT:         va_end(%160);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %161 @f15(%162 a1: i32, %163 a2: i32, %164 a3: i32, %165 a4: i32, %166 a5: i32, %167 a6: i32, %168 a7: i32, %169 a8: i32, %170 a9: i32, %171 a10: i32, %172 a11: i32, %173 a12: i32, %174 a13: i32, %175 a14: i32, %176 a15: i32, %177 format: ptr<i8>, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %178 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%178);
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, va_list) -> void>(%7, const<i32>(15), read<ptr<i8>>(%177), read<va_list>(%178));
// DEFAULT-NEXT:         va_end(%178);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %179 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %180 f: ptr<i8> [storage=automatic] = array_decay<ptr<i8>, length=Some(17)>(%184);
// DEFAULT-NEXT:         call<void, signature=fn(ptr<i8>, ...) -> void>(%11, ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(0)), const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ptr<i8>, ...) -> void>(%14, const<i32>(0), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(1)), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, ptr<i8>, ...) -> void>(%18, const<i32>(0), const<i32>(1), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(2)), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, ptr<i8>, ...) -> void>(%23, const<i32>(0), const<i32>(1), const<i32>(2), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(3)), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, ptr<i8>, ...) -> void>(%29, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(4)), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%36, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(5)), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%44, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(6)), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%53, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(7)), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%63, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(8)), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%74, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(9)), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%86, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(10)), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%99, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(11)), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%113, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(12)), const<i32>(12), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%128, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(13)), const<i32>(13), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%144, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(14)), const<i32>(14), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, ptr<i8>, ...) -> void>(%161, const<i32>(0), const<i32>(1), const<i32>(2), const<i32>(3), const<i32>(4), const<i32>(5), const<i32>(6), const<i32>(7), const<i32>(8), const<i32>(9), const<i32>(10), const<i32>(11), const<i32>(12), const<i32>(13), const<i32>(14), ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(%180), const<i32>(15)), const<i32>(15));
// DEFAULT-NEXT:         call<void, signature=fn(i32) -> void>(%2, const<i32>(0));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
