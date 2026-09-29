/* Test the code generation for the new attribute counted_by.
   And also the offsetof operator on such array.  */
/* { dg-do run } */
/* { dg-options "-O2 -fdump-tree-original" } */

#include <stdlib.h>

typedef __UINTPTR_TYPE__ uintptr_t;

struct annotated {
  int b;
  char *c __attribute__ ((counted_by (b)));
} *p_annotated;

struct flex {
  int b;
  char *c; 
}; 

struct nested_annotated {
  struct {
    union {
      int b;
      float f;	
    };
    int n;
  };
  char *c __attribute__ ((counted_by (b)));
} *p_nested_annotated;

struct nested_flex {
  struct {
    union {
      int b;
      float f;	
    };
    int n;
  };
  char *c;
};

void __attribute__((__noinline__)) setup (int normal_count, int attr_count)
{
  p_annotated
    = (struct annotated *)malloc (sizeof (struct annotated));
 
  p_annotated->c = (char *) malloc (sizeof (char) * attr_count); 
  p_annotated->b = attr_count;

  p_nested_annotated
    = (struct nested_annotated *)malloc (sizeof (struct nested_annotated));
  p_nested_annotated->c = (char *) malloc (attr_count *  sizeof (char));
  p_nested_annotated->b = attr_count;

  return;
}

void __attribute__((__noinline__)) test (char a, char b)
{
  if (__builtin_offsetof (struct annotated, c)
      != __builtin_offsetof (struct flex, c))
    abort ();
  if (__builtin_offsetof (struct nested_annotated, c) 
      != __builtin_offsetof (struct nested_flex, c)) 
    abort ();

  if (__alignof (*p_annotated->c) != __alignof (char))
    abort ();
  if (__alignof (*p_nested_annotated->c) != __alignof (char))
    abort ();

  p_annotated->c[2] = a;
  p_nested_annotated->c[3] = b;
}

int main(int argc, char *argv[])
{
  setup (10,10);   
  test ('A', 'B');
  if (p_annotated->c[2] != 'A') abort ();
  if (p_nested_annotated->c[3] != 'B') abort ();
  return 0;
}

/* { dg-final { scan-tree-dump-times "ACCESS_WITH_SIZE" 4 "original" } } */

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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_uintptr_t:[0-9]+]] uintptr_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_annotated:[0-9]+]] annotated = struct {
// DEFAULT-NEXT:         field0 b: i32;
// DEFAULT-NEXT:         field1 c: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_flex:[0-9]+]] flex = struct {
// DEFAULT-NEXT:         field0 b: i32;
// DEFAULT-NEXT:         field1 c: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_nested_annotated:[0-9]+]] nested_annotated = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:         field1 c: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE1:[0-9]+]];
// DEFAULT-NEXT:         field1 n: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE1]] = union {
// DEFAULT-NEXT:         field0 b: i32;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE_nested_flex:[0-9]+]] nested_flex = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE2:[0-9]+]];
// DEFAULT-NEXT:         field1 c: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE2]] = struct {
// DEFAULT-NEXT:         field0 <anonymous>: @type[[TYPE3:[0-9]+]];
// DEFAULT-NEXT:         field1 n: i32;
// DEFAULT-NEXT:     } [size=8, align=4, offsets=[0, 4]];
// DEFAULT-NEXT:     type @type[[TYPE3]] = union {
// DEFAULT-NEXT:         field0 b: i32;
// DEFAULT-NEXT:         field1 f: f32;
// DEFAULT-NEXT:     } [size=4, align=4, offsets=[0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_p_annotated:[0-9]+]] p_annotated: ptr<@type[[TYPE_annotated]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_p_nested_annotated:[0-9]+]] p_nested_annotated: ptr<@type[[TYPE_nested_annotated]]> [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_malloc:[0-9]+]] @malloc(%[[VALUE___size:[0-9]+]] __size: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_setup:[0-9]+]] @setup(%[[VALUE_normal_count:[0-9]+]] normal_count: i32, %[[VALUE_attr_count:[0-9]+]] attr_count: i32) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_annotated]]>>(%[[VALUE_p_annotated]], pointer_cast<ptr<@type[[TYPE_annotated]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(16))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type[[TYPE_annotated]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(16)));
// DEFAULT-NEXT:         write<ptr<i8>>(field1(deref(read<ptr<@type[[TYPE_annotated]]>>(%[[VALUE_p_annotated]]))), pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_attr_count]])))))));
// DEFAULT-NEXT:         pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(const<u64>(1), reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_attr_count]]))))));
// DEFAULT-NEXT:         write<i32>(field0(deref(read<ptr<@type[[TYPE_annotated]]>>(%[[VALUE_p_annotated]]))), read<i32>(%[[VALUE_attr_count]]));
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_nested_annotated]]>>(%[[VALUE_p_nested_annotated]], pointer_cast<ptr<@type[[TYPE_nested_annotated]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(16))));
// DEFAULT-NEXT:         pointer_cast<ptr<@type[[TYPE_nested_annotated]]>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], const<u64>(16)));
// DEFAULT-NEXT:         write<ptr<i8>>(field1(deref(read<ptr<@type[[TYPE_nested_annotated]]>>(%[[VALUE_p_nested_annotated]]))), pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_attr_count]]))), const<u64>(1)))));
// DEFAULT-NEXT:         pointer_cast<ptr<i8>, reason=explicit>(call<ptr<void>, signature=fn(u64) -> ptr<void>>(%[[VALUE_malloc]], mul<u64, overflow=wrap>(reinterpret<u64, reason=usual_arith, fits=unknown>(widen<i64, reason=usual_arith>(read<i32>(%[[VALUE_attr_count]]))), const<u64>(1))));
// DEFAULT-NEXT:         write<i32>(field0(field0(field0(deref(read<ptr<@type[[TYPE_nested_annotated]]>>(%[[VALUE_p_nested_annotated]]))))), read<i32>(%[[VALUE_attr_count]]));
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test:[0-9]+]] @test(%[[VALUE_a:[0-9]+]] a: i8, %[[VALUE_b:[0-9]+]] b: i8) -> void [linkage=external] [inline=never] [definition=emitted] [fallthrough=ret_void] {
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(8), const<u64>(8))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<u64>(const<u64>(1), const<u64>(1))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field1(deref(read<ptr<@type[[TYPE_annotated]]>>(%[[VALUE_p_annotated]])))), const<i32>(2))), read<i8>(%[[VALUE_a]]));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field1(deref(read<ptr<@type[[TYPE_nested_annotated]]>>(%[[VALUE_p_nested_annotated]])))), const<i32>(3))), read<i8>(%[[VALUE_b]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main(%[[VALUE_argc:[0-9]+]] argc: i32, %[[VALUE_argv:[0-9]+]] argv: ptr<ptr<i8>>) -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         call<void, signature=fn(i32, i32) -> void>(%[[VALUE_setup]], const<i32>(10), const<i32>(10));
// DEFAULT-NEXT:         call<void, signature=fn(i8, i8) -> void>(%[[VALUE_test]], truncate<i8, reason=arg, fits=always>(const<i32>(65)), truncate<i8, reason=arg, fits=always>(const<i32>(66)));
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field1(deref(read<ptr<@type[[TYPE_annotated]]>>(%[[VALUE_p_annotated]])))), const<i32>(2))))), const<i32>(65))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field1(deref(read<ptr<@type[[TYPE_nested_annotated]]>>(%[[VALUE_p_nested_annotated]])))), const<i32>(3))))), const<i32>(66))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
