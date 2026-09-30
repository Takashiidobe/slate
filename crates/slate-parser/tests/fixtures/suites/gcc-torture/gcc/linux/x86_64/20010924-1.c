/* Verify that flexible arrays can be initialized from STRING_CST
   constructors. */

void abort(void);

/* Baselines.  */
struct {
  char  a1c;
  char *a1p;
} a1 = {'4', "62"};

struct {
  char a2c;
  char a2p[2];
} a2 = {'v', "cq"};

/* The tests.  */
struct {
  char a3c;
  char a3p[];
} a3 = {'o', "wx"};

struct {
  char a4c;
  char a4p[];
} a4 = {'9', {'e', 'b'}};

int main(void) {
  if (a1.a1c != '4')
    abort();
  if (a1.a1p[0] != '6')
    abort();
  if (a1.a1p[1] != '2')
    abort();
  if (a1.a1p[2] != '\0')
    abort();

  if (a2.a2c != 'v')
    abort();
  if (a2.a2p[0] != 'c')
    abort();
  if (a2.a2p[1] != 'q')
    abort();

  if (a3.a3c != 'o')
    abort();
  if (a3.a3p[0] != 'w')
    abort();
  if (a3.a3p[1] != 'x')
    abort();

  if (a4.a4c != '9')
    abort();
  if (a4.a4p[0] != 'e')
    abort();
  if (a4.a4p[1] != 'b')
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
// DEFAULT-NEXT:     type @type[[TYPE0:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a1c: i8;
// DEFAULT-NEXT:         field1 a1p: ptr<i8>;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE1:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a2c: i8;
// DEFAULT-NEXT:         field1 a2p: array<i8, 2>;
// DEFAULT-NEXT:     } [size=3, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE2:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a3c: i8;
// DEFAULT-NEXT:         field1 a3p: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     type @type[[TYPE3:[0-9]+]] = struct {
// DEFAULT-NEXT:         field0 a4c: i8;
// DEFAULT-NEXT:         field1 a4p: array<i8, incomplete>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     global %[[VALUE_str:[0-9]+]] .str[[VALUE_str]]: array<i8, 3> [storage=static] = code_units<array<i8, 3>>([54, 50, 0]) [linkage=internal];
// DEFAULT-NEXT:     global %[[VALUE_a1:[0-9]+]] a1: @type[[TYPE0]] [storage=static] = aggregate<@type[[TYPE0]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(52)), field1 = array_decay<ptr<i8>, length=Some(3)>(%[[VALUE_str]])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a2:[0-9]+]] a2: @type[[TYPE1]] [storage=static] = aggregate<@type[[TYPE1]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(118)), field1 = code_units<array<i8, 2>>([99, 113])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a3:[0-9]+]] a3: @type[[TYPE2]] [storage=static] = aggregate<@type[[TYPE2]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(111)), field1 = code_units<array<i8, 3>>([119, 120, 0])) [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_a4:[0-9]+]] a4: @type[[TYPE3]] [storage=static] = aggregate<@type[[TYPE3]], zero_fill=false>(field0 = truncate<i8, reason=assign, fits=always>(const<i32>(57)), field1 = aggregate<array<i8, 2>, zero_fill=false>(index0 = truncate<i8, reason=assign, fits=always>(const<i32>(101)), index1 = truncate<i8, reason=assign, fits=always>(const<i32>(98)))) [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_a1]]))), const<i32>(52))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field1(%[[VALUE_a1]])), const<i32>(0))))), const<i32>(54))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field1(%[[VALUE_a1]])), const<i32>(1))))), const<i32>(50))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(read<ptr<i8>>(field1(%[[VALUE_a1]])), const<i32>(2))))), const<i32>(0))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_a2]]))), const<i32>(118))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field1(%[[VALUE_a2]])), const<i32>(0))))), const<i32>(99))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(2)>(field1(%[[VALUE_a2]])), const<i32>(1))))), const<i32>(113))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_a3]]))), const<i32>(111))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(field1(%[[VALUE_a3]])), const<i32>(0))))), const<i32>(119))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(field1(%[[VALUE_a3]])), const<i32>(1))))), const<i32>(120))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(field0(%[[VALUE_a4]]))), const<i32>(57))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(field1(%[[VALUE_a4]])), const<i32>(0))))), const<i32>(101))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         if ne<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=None>(field1(%[[VALUE_a4]])), const<i32>(1))))), const<i32>(98))
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
