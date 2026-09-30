extern void abort(void);
extern void exit(int);

union iso_directory_record {
  char carr[4];
  struct {
    unsigned char name_len[1];
    char          name[0];
  } u;
} entry;

void set(union iso_directory_record *);

int main(void) {
  union iso_directory_record *de;

  de = &entry;
  set(de);

  if (de->u.name_len[0] == 1 && de->u.name[0] == 0)
    exit(0);
  else
    abort();
}

void set(union iso_directory_record *p) {
  p->carr[0] = 1;
  p->carr[1] = 0;
  return;
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
// DEFAULT-NEXT:     type @type[[TYPE_iso_directory_record:[0-9]+]] iso_directory_record = union {
// DEFAULT-NEXT:         field0 carr: array<i8, 4>;
// DEFAULT-NEXT:         field1 u: @type[[TYPE0:[0-9]+]];
// DEFAULT-NEXT:     } [size=4, align=1, offsets=[0, 0]];
// DEFAULT-NEXT:     type @type[[TYPE0]] = struct {
// DEFAULT-NEXT:         field0 name_len: array<u8, 1>;
// DEFAULT-NEXT:         field1 name: array<i8, 0>;
// DEFAULT-NEXT:     } [size=1, align=1, offsets=[0, 1]];
// DEFAULT-NEXT:     global %[[VALUE_entry:[0-9]+]] entry: @type[[TYPE_iso_directory_record]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_abort:[0-9]+]] @abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_exit:[0-9]+]] @exit(%[[VALUE0:[0-9]+]] <unnamed>: i32) -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_set:[0-9]+]] @set(%[[VALUE_p:[0-9]+]] p: ptr<@type[[TYPE_iso_directory_record]]>) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_iso_directory_record]]>>(%[[VALUE_p]])))), const<i32>(0))), truncate<i8, reason=assign, fits=always>(const<i32>(1)));
// DEFAULT-NEXT:         write<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(4)>(field0(deref(read<ptr<@type[[TYPE_iso_directory_record]]>>(%[[VALUE_p]])))), const<i32>(1))), truncate<i8, reason=assign, fits=always>(const<i32>(0)));
// DEFAULT-NEXT:         return;
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_de:[0-9]+]] de: ptr<@type[[TYPE_iso_directory_record]]> [storage=automatic];
// DEFAULT-NEXT:         write<ptr<@type[[TYPE_iso_directory_record]]>>(%[[VALUE_de]], addr_of<ptr<@type[[TYPE_iso_directory_record]]>>(%[[VALUE_entry]]));
// DEFAULT-NEXT:         call<void, signature=fn(ptr<@type[[TYPE_iso_directory_record]]>) -> void>(%[[VALUE_set]], read<ptr<@type[[TYPE_iso_directory_record]]>>(%[[VALUE_de]]));
// DEFAULT-NEXT:         if logical_and<bool>(eq<i32>(reinterpret<i32, reason=promotion, fits=unknown>(widen<u32, reason=promotion>(read<u8>(deref(ptr_offset<ptr<u8>, subtract=false, element=u8, overflow=ub>(array_decay<ptr<u8>, length=Some(1)>(field0(field1(deref(read<ptr<@type[[TYPE_iso_directory_record]]>>(%[[VALUE_de]]))))), const<i32>(0)))))), const<i32>(1)), eq<i32>(widen<i32, reason=promotion>(read<i8>(deref(ptr_offset<ptr<i8>, subtract=false, element=i8, overflow=ub>(array_decay<ptr<i8>, length=Some(0)>(field1(field1(deref(read<ptr<@type[[TYPE_iso_directory_record]]>>(%[[VALUE_de]]))))), const<i32>(0))))), const<i32>(0)))
// DEFAULT-NEXT:             call<void, signature=fn(i32) -> void>(%[[VALUE_exit]], const<i32>(0));
// DEFAULT-NEXT:         else
// DEFAULT-NEXT:             call<void, signature=fn() -> void>(%[[VALUE_abort]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
