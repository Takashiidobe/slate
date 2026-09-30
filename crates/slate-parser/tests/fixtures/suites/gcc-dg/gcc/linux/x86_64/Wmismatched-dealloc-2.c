/* PR middle-end/94527 - Add an attribute that marks a function as freeing
   an object
   Verify that attribute malloc with one or two arguments has the expected
   effect on diagnostics.
   { dg-options "-Wall -ftrack-macro-expansion=0" } */

#define A(...) __attribute__ ((malloc (__VA_ARGS__), noipa))

typedef __SIZE_TYPE__ size_t;
typedef struct A A;
typedef struct B B;

/* A pointer returned by any of the four functions must be deallocated
   either by dealloc() or by realloc_{A,B}().  */
A (__builtin_free) A* alloc_A (int);
A (__builtin_free) B* alloc_B (int);
A (__builtin_free) A* realloc_A (A *p, int n) { return p; }
A (__builtin_free) B* realloc_B (B *p, int n) { return p; }

A (realloc_A) A* alloc_A (int);
A (realloc_B) B* alloc_B (int);
A (realloc_A) A* realloc_A (A*, int);
A (realloc_B) B* realloc_B (B*, int);

void dealloc (void*);
A (dealloc) void* alloc (int);

void sink (void*);
void* source (void);

void test_alloc_A (void)
{
  {
    void *p = alloc_A (1);
    p = realloc_A (p, 2);
    __builtin_free (p);
  }

  {
    void *p = alloc_A (1);
    /* Verify that calling realloc doesn't trigger a warning even though
       alloc_A is not directly associated with it.  */
    p = __builtin_realloc (p, 2);
    sink (p);
  }

  {
    void *p = alloc_A (1);              // { dg-message "returned from 'alloc_A'" }
    dealloc (p);                        // { dg-warning "'dealloc' called on pointer returned from a mismatched allocation function" }
  }

  {
    /* Because alloc_A() and realloc_B() share free() as a deallocator
       they must also be valid as each other's deallocators.  */
    void *p = alloc_A (1);
    p = realloc_B ((B*)p, 2);
    __builtin_free (p);
  }

  {
    void *p = alloc_A (1);
    p = realloc_A (p, 2);
    p = __builtin_realloc (p, 3);
    __builtin_free (p);
  }
}


void test_realloc_A (void *ptr)
{
  {
    void *p = realloc_A (0, 1);
    p = realloc_A (p, 2);
    __builtin_free (p);
  }

  {
    void *p = realloc_A (ptr, 2);
    p = realloc_A (p, 2);
    __builtin_free (p);
  }

  {
    void *p = realloc_A (0, 3);
    p = __builtin_realloc (p, 2);
    sink (p);
  }

  {
    void *p = realloc_A (0, 4);         // { dg-message "returned from 'realloc_A'" }
    dealloc (p);                        // { dg-warning "'dealloc' called on pointer returned from a mismatched allocation function" }
  }

  {
    /* Because realloc_A() and realloc_B() share free() as a deallocator
       they must also be valid as each other's deallocators.  */
    void *p = realloc_A (0, 5);
    p = realloc_B ((B*)p, 2);
    __builtin_free (p);
  }

  {
    void *p = realloc_A (0, 6);
    p = realloc_A ((A*)p, 2);
    p = __builtin_realloc (p, 3);
    __builtin_free (p);
  }
}


void test_realloc (void)
{
  extern void free (void*);
  extern void* realloc (void*, size_t);

  {
    void *p = realloc (source (), 1);
    p = realloc_A (p, 2);
    __builtin_free (p);
  }

  {
    void *p = realloc (source (), 2);
    p = realloc_A (p, 2);
    free (p);
  }

  {
    void *p = realloc (source (), 3);
    free (p);
  }

  {
    void *p = realloc (source (), 4);
    __builtin_free (p);
  }

  {
    void *p = realloc (source (), 5);   // { dg-message "returned from 'realloc'" }
    dealloc (p);                        // { dg-warning "'dealloc' called on pointer returned from a mismatched allocation function" }
  }
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
// DEFAULT-NEXT:     type @type[[TYPE_size_t:[0-9]+]] size_t = u64;
// DEFAULT-NEXT:     type @type[[TYPE_A:[0-9]+]] A = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_A_2:[0-9]+]] A = @type[[TYPE_A]];
// DEFAULT-NEXT:     type @type[[TYPE_B:[0-9]+]] B = struct incomplete;
// DEFAULT-NEXT:     type @type[[TYPE_B_2:[0-9]+]] B = @type[[TYPE_B]];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_free:[0-9]+]] @__builtin_free(%[[VALUE0:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_alloc_A:[0-9]+]] @alloc_A(%[[VALUE1:[0-9]+]] <unnamed>: i32) -> ptr<@type[[TYPE_A]]> [linkage=external] [deallocator=%[[VALUE___builtin_free]], argument=0] [deallocator=%[[VALUE_realloc_A:[0-9]+]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_alloc_B:[0-9]+]] @alloc_B(%[[VALUE2:[0-9]+]] <unnamed>: i32) -> ptr<@type[[TYPE_B]]> [linkage=external] [deallocator=%[[VALUE___builtin_free]], argument=0] [deallocator=%[[VALUE_realloc_B:[0-9]+]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_realloc_A]] @realloc_A(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_A]]>, %[[VALUE_n:[0-9]+]] n: i32) -> ptr<@type[[TYPE_A]]> [linkage=external] [deallocator=%[[VALUE___builtin_free]], argument=0] [deallocator=%[[VALUE_realloc_A]], argument=0] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_A]]>>(%[[VALUE_p]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_realloc_B]] @realloc_B(%[[VALUE_p_2:[0-9]+]] p: ptr<@type[[TYPE_B]]>, %[[VALUE_n_2:[0-9]+]] n: i32) -> ptr<@type[[TYPE_B]]> [linkage=external] [deallocator=%[[VALUE___builtin_free]], argument=0] [deallocator=%[[VALUE_realloc_B]], argument=0] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         return read<ptr<@type[[TYPE_B]]>>(%[[VALUE_p_2]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_dealloc:[0-9]+]] @dealloc(%[[VALUE3:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_alloc:[0-9]+]] @alloc(%[[VALUE4:[0-9]+]] <unnamed>: i32) -> ptr<void> [linkage=external] [deallocator=%[[VALUE_dealloc]], argument=0];
// DEFAULT-NEXT:     fn %[[VALUE_sink:[0-9]+]] @sink(%[[VALUE5:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_source:[0-9]+]] @source() -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE___builtin_realloc:[0-9]+]] @__builtin_realloc(%[[VALUE6:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE7:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_alloc_A:[0-9]+]] @test_alloc_A() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_3:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_alloc_A]], const<i32>(1)));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_3]], pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], pointer_cast<ptr<@type[[TYPE_A]]>, reason=arg>(read<ptr<void>>(%[[VALUE_p_3]])), const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p_3]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_4:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_alloc_A]], const<i32>(1)));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_4]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_p_4]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_4]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_5:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_alloc_A]], const<i32>(1)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_dealloc]], read<ptr<void>>(%[[VALUE_p_5]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_6:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_alloc_A]], const<i32>(1)));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_6]], pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_B]]>, signature=fn(ptr<@type[[TYPE_B]]>, i32) -> ptr<@type[[TYPE_B]]>>(%[[VALUE_realloc_B]], pointer_cast<ptr<@type[[TYPE_B]]>, reason=explicit>(read<ptr<void>>(%[[VALUE_p_6]])), const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p_6]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_7:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_alloc_A]], const<i32>(1)));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_7]], pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], pointer_cast<ptr<@type[[TYPE_A]]>, reason=arg>(read<ptr<void>>(%[[VALUE_p_7]])), const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_7]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_p_7]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p_7]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_test_realloc_A:[0-9]+]] @test_realloc_A(%[[VALUE_ptr:[0-9]+]] ptr: ptr<void>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_8:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], null<ptr<@type[[TYPE_A]]>>, const<i32>(1)));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_8]], pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], pointer_cast<ptr<@type[[TYPE_A]]>, reason=arg>(read<ptr<void>>(%[[VALUE_p_8]])), const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p_8]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_9:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], pointer_cast<ptr<@type[[TYPE_A]]>, reason=arg>(read<ptr<void>>(%[[VALUE_ptr]])), const<i32>(2)));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_9]], pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], pointer_cast<ptr<@type[[TYPE_A]]>, reason=arg>(read<ptr<void>>(%[[VALUE_p_9]])), const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p_9]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_10:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], null<ptr<@type[[TYPE_A]]>>, const<i32>(3)));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_10]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_p_10]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_sink]], read<ptr<void>>(%[[VALUE_p_10]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_11:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], null<ptr<@type[[TYPE_A]]>>, const<i32>(4)));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_dealloc]], read<ptr<void>>(%[[VALUE_p_11]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_12:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], null<ptr<@type[[TYPE_A]]>>, const<i32>(5)));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_12]], pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_B]]>, signature=fn(ptr<@type[[TYPE_B]]>, i32) -> ptr<@type[[TYPE_B]]>>(%[[VALUE_realloc_B]], pointer_cast<ptr<@type[[TYPE_B]]>, reason=explicit>(read<ptr<void>>(%[[VALUE_p_12]])), const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p_12]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_13:[0-9]+]] p: ptr<void> [storage=automatic] = pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], null<ptr<@type[[TYPE_A]]>>, const<i32>(6)));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_13]], pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], pointer_cast<ptr<@type[[TYPE_A]]>, reason=explicit>(read<ptr<void>>(%[[VALUE_p_13]])), const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_13]], call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE___builtin_realloc]], read<ptr<void>>(%[[VALUE_p_13]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3)))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p_13]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_free:[0-9]+]] @free(%[[VALUE8:[0-9]+]] <unnamed>: ptr<void>) -> void [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_realloc:[0-9]+]] @realloc(%[[VALUE9:[0-9]+]] <unnamed>: ptr<void>, %[[VALUE10:[0-9]+]] <unnamed>: u64) -> ptr<void> [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_test_realloc:[0-9]+]] @test_realloc() -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_14:[0-9]+]] p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(1))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_14]], pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], pointer_cast<ptr<@type[[TYPE_A]]>, reason=arg>(read<ptr<void>>(%[[VALUE_p_14]])), const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p_14]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_15:[0-9]+]] p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(2))));
// DEFAULT-NEXT:             write<ptr<void>>(%[[VALUE_p_15]], pointer_cast<ptr<void>, reason=assign>(call<ptr<@type[[TYPE_A]]>, signature=fn(ptr<@type[[TYPE_A]]>, i32) -> ptr<@type[[TYPE_A]]>>(%[[VALUE_realloc_A]], pointer_cast<ptr<@type[[TYPE_A]]>, reason=arg>(read<ptr<void>>(%[[VALUE_p_15]])), const<i32>(2))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_p_15]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_16:[0-9]+]] p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(3))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_free]], read<ptr<void>>(%[[VALUE_p_16]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_17:[0-9]+]] p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(4))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE___builtin_free]], read<ptr<void>>(%[[VALUE_p_17]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:         {
// DEFAULT-NEXT:             let %[[VALUE_p_18:[0-9]+]] p: ptr<void> [storage=automatic] = call<ptr<void>, signature=fn(ptr<void>, u64) -> ptr<void>>(%[[VALUE_realloc]], call<ptr<void>, signature=fn() -> ptr<void>>(%[[VALUE_source]]), reinterpret<u64, reason=arg, fits=unknown>(widen<i64, reason=arg>(const<i32>(5))));
// DEFAULT-NEXT:             call<void, signature=fn(ptr<void>) -> void>(%[[VALUE_dealloc]], read<ptr<void>>(%[[VALUE_p_18]]));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
