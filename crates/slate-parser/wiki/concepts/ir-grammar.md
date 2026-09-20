# IR Grammar

The grammar of the IR text form printed by `slate-parser ir source.c` (also
`parse source.c --dump-ir`). It is the reference for what each IR construct
can be and which choices it has. [IR Spec](ir-spec.md) explains why the IR has
this shape.

The printer is the source of truth: `src/ir/module_print.rs` (module,
declarations, statements), `Value::format` in `src/ir/mod.rs` (values),
`Place::format` in `src/ir/declarations.rs` (places), and the `Display` impls
in `src/ir/numeric.rs`, `src/ir/abi.rs` and `src/ir/module.rs`. **Any change to
what those print must update this file in the same change.**

## Notation

- `a = ... ;` defines `a`; `|` separates choices; `[ x ]` is optional;
  `{ x }` is zero or more; `( ... )` groups; `(* ... *)` is a comment.
- Quoted terminals are printed exactly, including the spaces inside them.
  Adjacent symbols are separated by one space unless a terminal carries its
  own punctuation (`"<"`, `">("`, `", "`).
- Layout: module items are indented 4 spaces, statements 8. A nested `body`
  is the statements indented one level (4 spaces) deeper than their owner;
  only blocks and evaluations use braces.
- This is the default form. `--show-metadata` adds [metadata](#metadata);
  `--compact-ir` removes parts as listed in [Compact form](#compact-form).
- Out of scope: the diagnostic dumps `--dump-ir-types`,
  `--dump-ir-expressions` (which can add `--show-spans`), and
  `--dump-ir-names`.

## Lexical

```ebnf
binding    = "%" digits ;                (* BindingId: locals, globals, functions,
                                            parameters, loops, switches, labels *)
type_ref   = "@type" digits ;            (* TypeId of a type definition *)
name       = c_identifier | ".str" digits ;
int        = [ "-" ] digits ;
bool       = "true" | "false" ;
string     = '"' { char | escape } '"' ; (* Rust {:?} quoting *)
int_list   = "[" [ int { ", " int } ] "]" ;
opt_int    = "Some(" int ")" | "None" ;
```

`%N` is identity; the name printed after it is only a spelling. A `binding`
used before its declaration (a function called before its definition, a loop
ID in `break`) refers to the same entity.

## Module

```ebnf
module     = "module" "{" target { type_def } { global } { function } "}" ;

target     = "target" string "{"
               "endian" "=" ( "little" | "big" ) ";"
               "pointer" "[size=" int ", align=" int "]" ";"
               "stack_alignment" "=" int ";"
               "long_double" "=" float_type ";"
               { "storage" storage_of "[size=" int ", align=" int "]" ";" }
             "}" ;
storage_of = "bool" | "i8, u8" | "i16, u16" | "i32, u32" | "i64, u64"
           | "i128, u128" | "f16" | "f32" | "f64" | "f80" | "f128" ;
```

A `storage` line is printed only for types the target supports.

## Types

```ebnf
type          = "void" | "bool" | "va_list" | numeric
              | "complex<" numeric ">"
              | "imaginary<" float_type ">"
              | "vector<" numeric ", " int ">"
              | "ptr<" [ "const " ] [ access_prefix ] type ">"
              | "array<" type ", " ( int | "incomplete" ) ">"
              | "vla<" type ", " ( binding | "*" ) ">"
              | fn_type
              | type_ref ;
numeric       = int_type | float_type ;
int_type      = ( "i" | "u" ) digits [ "b" ] ;
float_type    = "f16" | "f32" | "f64" | "f80" | "f128" | "d32" | "d64" | "d128" ;
access_prefix = "volatile " | "atomic " | "volatile atomic " ;
fn_type       = "fn(" [ fn_params ] ") -> " type ;
fn_params     = "unprototyped" | type { ", " type } [ ", ..." ] | "..." ;
```

- Types are concrete and target-resolved; the C spelling is metadata.
- `b` marks a bit-precise integer (`_BitInt(N)`), distinct from the standard
  integer of the same width: `i128b` is not `i128`.
- `d32`/`d64`/`d128` are decimal floating types, never converted to binary.
- `vector<T, N>` is a GNU `vector_size` or Clang `ext_vector_type` type with
  `N` lanes of the scalar `T`; both source forms print the same way.
- `ptr<const T>` records pointee constness; `volatile`/`atomic` on a pointee
  are what make accesses through the pointer volatile or atomic.
- `vla<T, %n>` has a runtime extent held in synthetic binding `%n`, captured
  once at the declaration. `vla<T, *>` is a variable extent with no captured
  value: `[*]`, or any non-constant bound in prototype scope.
- Pointer, array and function types are structural and print inline. Records,
  enums and named aliases are `@typeN` references to a `type_def`.

## Type definitions

```ebnf
type_def      = "type" type_ref [ c_identifier ] "=" type_body { metadata } ";" ;
type_body     = type                                          (* alias *)
              | ( "struct" | "union" ) ( fields | "incomplete" ) [ record_layout ]
              | "enum" [ ":" type ] ( enumerators | "incomplete" )
                [ "[size=" int ", align=" int "]" ] ;
fields        = "{" { field } "}" ;
field         = "field" digits ( c_identifier | "<anonymous>" ) ":"
                [ access_prefix ] type [ ":" int ] { metadata } ";" ;
record_layout = "[size=" int ", align=" int ", offsets=" int_list
                [ ", bit_offsets=" "[" opt_int { ", " opt_int } "]" ]
                [ ", bit_units=" "[" [ unit { ", " unit } ] "]" ]
                [ ", field_units=" "[" opt_int { ", " opt_int } "]" ] "]" ;
unit          = "(" int ", " int ")" ;                        (* byte offset, size *)
enumerators   = "{" { binding c_identifier "=" value { metadata } ";" } "}" ;
```

- `fieldN` is the field's index in declaration order; places refer to fields
  by that index. `: W` is a bit-field width.
- `offsets` has one byte offset per field. `bit_offsets` and `field_units` are
  per field (`None` for ordinary fields); `bit_units` are the byte extents of
  bit-field storage units.
- The enum's `: type` is its resolved underlying integer type.

## Globals

```ebnf
global       = ( "global" | "extern" ) variable linkage symbol_attrs
               [ "[common]" ] { metadata } ";" ;
variable     = binding name ":" [ access_prefix ] type "[storage=" storage "]"
               [ "[restrict]" ] [ "[const]" ] [ "[constexpr]" ]
               [ "[align=" int "]" ] [ "=" value ] ;
storage      = "automatic" | "static" | "thread" ;
linkage      = "[linkage=" ( "internal" | "external" ) "]" ;
symbol_attrs = [ "[asm_name=" string "]" ] [ "[visibility=" visibility "]" ]
               [ "[weak]" ] [ "[alias=" string "]" ] [ "[weakref=" string "]" ]
               [ "[section=" string "]" ] [ "[used]" ] [ "[retain]" ]
               [ "[tls_model=" tls_model "]" ] [ "[dllimport]" | "[dllexport]" ]
               [ "[selectany]" ] ;
visibility   = "default" | "hidden" | "protected" | "internal" ;
tls_model    = "global-dynamic" | "local-dynamic" | "initial-exec" | "local-exec" ;
```

- `global` is a definition, `extern` a declaration only. All declarations of
  one object with linkage are merged into a single `global`.
- A global's storage is `static` or `thread`, never `automatic`. Block-scope
  statics and string literals (`.strN`, `code_units`) are internal globals.
- No initializer means zero initialization for a `global`.

## Functions

```ebnf
function    = "fn" binding "@" c_identifier "(" [ params ] ")" "->" type
              linkage symbol_attrs [ "[inline=" ( "hint" | "always" | "never" ) "]" ]
              [ "[definition=" ( "emitted" | "inline_only" ) "]" ] [ "[noreturn]" ]
              [ "[abi=" abi_signature "]" ]
              [ "[fallthrough=" fallthrough "]" ] { metadata }
              ( ";" | "{" { statement } "}" ) ;
params      = "unprototyped" | param { ", " param } [ ", ..." ] | "..." ;
param       = binding ( c_identifier | "<unnamed>" ) ":" [ access_prefix ] type
              [ "[restrict]" ] [ "[const]" ] [ array_param ] { metadata } ;
array_param = "[array=" ( "static" [ " " extent ] | extent ) "]" ;
extent      = integer | binding | "*" ;
fallthrough = "ret_zero" | "ret_void" | "ub_if_used" | "ub" ;
```

- One `fn` per function: the body and parameters come from the definition,
  or else the first prototype.
- `[abi=...]` is printed only when some argument or the result is not passed
  as a plain scalar.
- `fallthrough` is present only on definitions and says what reaching the end
  of the body means.
- Inlining preference and definition emission are independent. Inline bodies
  print whether they supply a linkable definition; `inline_only` bodies do not.
  `noreturn` survives compact printing and makes fallthrough unconditionally `ub`.

## ABI signatures

```ebnf
abi_signature = convention "(" [ abi_pass { ", " abi_pass } ] ") -> " abi_pass ;
convention    = "sysv64" | "win64" | "x86_cdecl" | "aapcs64" | "win_arm64"
              | "aapcs32" | "aapcs32_hard_float" ;
abi_pass      = "void" | "scalar" | "direct" | "native_c"
              | "coerce<" chunk { ", " chunk } ">"
              | "byval<align=" int ">" | "byref<align=" int ">" | "sret<align=" int ">" ;
chunk         = "i" digits | float_type | "pair<" float_type ">" ;
```

`native_c` leaves the record to the target's ordinary C ABI; it is not a
verified coercion. `direct` is the opposite: the value is passed in registers
as its own type, with no coercion and no memory copy. It is what vectors that
fit the target's vector registers use, where `scalar` would misdescribe them.

## Statements

```ebnf
statement  = simple { metadata } ";"
           | "if" value { metadata } body [ "else" body ]
           | "while" binding evaluation { metadata } body
           | "do" binding { metadata } body "while" evaluation ";"
           | "for" binding { metadata }
               "init:" body
               "condition:" ( evaluation | "omitted" )
               "increment:" ( evaluation | "omitted" )
               "body:" body
           | "switch" binding value { metadata } body
           | "case" binding value [ "..." value ] ":" { metadata } body
           | "default" binding ":" { metadata } body
           | "label" binding c_identifier ":" { metadata } body
           | "{" { metadata } body "}" ;
simple     = "let" binding ":" type "[synthetic]" [ "=" value ]
           | "let" variable
           | "write<" type access [ ordering ] ">(" place ", " value ")"
           | "fence<scope=" ( "thread" | "signal" ) ", order=" order ">"
           | value
           | "return" [ value ]
           | "break" binding | "continue" binding
           | "goto" binding | "goto *" value
           | (* empty: null statement *) ;
body       = { statement } ;                   (* indented one level deeper *)
evaluation = value | "{" { statement } "yield" value ";" "}" ;
```

- Loops, switches and labels are identified by their `binding`. `break` and
  `continue` name the loop or switch they leave, and `case`/`default` name
  their switch even when nested deeper (Duff's device). There are no implicit
  targets.
- Statement order and the absence of `break` express fallthrough.
- `evaluation` is a loop condition or `for` increment with hoisted side
  effects: the statements run each time, then `yield` gives the value. An
  increment yields `void`.
- `[synthetic]` lets are temporaries that lowering introduced (hoisted old
  values, short-circuit results, VLA extents). They have no source variable.
- A local `let` always has `storage=automatic`.

## Places

```ebnf
place  = binding
       | "deref(" value ")"
       | "index(" value ", " value ")"
       | "field" digits "(" place ")"
       | "bitfield" digits "<unit=" int ", bytes=" int ".." int
         ", bits=" int ".." int ">(" place ")"
       | ( "real(" | "imag(" ) place ")"
       | "compound_literal" binding "[storage=" storage "]" "=" value ;
access   = [ ", volatile" ] ;
ordering = ", atomic=" order ;
order    = "relaxed" | "consume" | "acquire" | "release" | "acq_rel"
         | "seq_cst" | "dynamic(" value ")" ;
```

- A place is a storage location, not a read. Only `read`, `write`, `store`,
  `update`, `compare_exchange`, `addr_of`, the decays and the `va_*` values
  use one.
- `ordering` belongs to the operation, not the place: it is absent on a
  non-atomic access and follows `access` when present (`read<i32, volatile,
  atomic=acquire>`). `dynamic(v)` is a runtime ordering value.
- `bitfieldN<unit=U, bytes=A..B, bits=C..D>` is field `N`, stored in storage
  unit `U` (record bytes `A..B`), occupying bits `C..D` from the least
  significant bit of that unit. Bit-fields are not addressable.
- `compound_literal %N` is a distinct object with its own storage duration.
- Lowering does not produce `index(..)` yet: all indexing is
  `deref(ptr_offset(..))`, including arrays through `array_decay`.
- Values printed inside a place never carry metadata.

## Values

```ebnf
value      = core { metadata } ;
core       = "const<" type ">(" constant ")"
           | "null<" type ">"
           | "void"
           | "code_units<" type ">(" int_list ")"
           | "aggregate<" type ", zero_fill=" bool ">(" [ member { ", " member } ] ")"
           | "copy<" type ", reason=" reason ">(" value ")"
           | "read<" type access [ ordering ] ">(" place ")"
           | "store<" type access [ ordering ] ">(" place ", " value ")"
           | "update<" type ", result=" ( "old" | "new" ) access [ ordering ]
             ">(" place ", " value ")"
           | "compare_exchange<" type access ", form=" exchange_form ", weak="
             ( bool | "dynamic(" value ")" ) ", success=" order ", failure=" order
             ">(" place ", " value ", " value ")"
           | "old<" type ">"
           | "addr_of<" type ">(" place ")"
           | "array_decay<" type ", length=" opt_int ">(" place ")"
           | "function_decay<" type ">(" place ")"
           | "label_addr<" type ">(" binding ")"
           | "ptr_offset<" type ", subtract=" bool ", element=" type overflow
             ">(" value ", " value ")"
           | "ptr_diff<" type ", element=" type ", same_array=required, overflow=ub"
             ">(" value ", " value ")"
           | "conditional<" type ">(" value ", " value ", " value ")"
           | "sequence<" type ">(" value ", " value ")"
           | "call<" type ", signature=" fn_type [ ", abi=" abi_signature ]
             ">(" ( binding | builtin_name | value ) { ", " value } ")"
           | "va_arg<" type ">(" place ")"
           | "va_start(" place ")" | "va_end(" place ")"
           | "va_copy(" place ", " place ")"
           | conversion "<" type ", reason=" reason conversion_policy ">(" value ")"
           | arith_op "<" type arith_policy ">(" value ", " value ")"
           | "overflow_" ( "add" | "sub" | "mul" ) "<bool>("
             value ", " value ", " place ")"
           | unary_op "<" type arith_policy ">(" value ")"
           | compare_op "<" type [ ", result=" type ] [ ", reason=" reason ]
             [ ", exceptions=" exceptions ] ">(" value ", " value ")"
           | logical_op "<" type ">(" value ", " value ")" ;
constant   = int | bool | decimal_digits | float_literal | "bits=0x" hex_digits ;
member     = ( "field" digits | "index" digits | "index" digits "..=" digits )
             " = " value ;
exchange_form = "write_back" | "success" | "old" ;
reason     = "return" | "assign" | "arg" | "vararg" | "promotion"
           | "usual_arith" | "explicit" ;
```

- The type after `<` is the result type, except in `read`/`write` and
  `compare_exchange` (result `bool`, or the place type for `form=old`), where it is the place type, and in `compare_op`, where it is the operand type. A
  scalar comparison results in `bool`; a vector comparison prints its
  `result=vector<iN, lanes>` lane-mask type instead.
- `float_literal` is a round-trippable decimal (`1.0`, `-0.0`, `inf`); a NaN
  prints as `bits=0x...` to keep its payload. Decimal floats keep their digit
  spelling.
- `aggregate` members are resolved targets in order; nested subobjects are
  nested `aggregate`s. `zero_fill=true` means some member or element was
  omitted and is zero-initialized. A union has exactly one member.
- `call` names a direct callee by `binding`, an implicit compiler builtin by
  `builtin_name`, and an indirect callee by a pointer value. `signature` is the
  type visible at this call site, which can differ from the function's final
  declaration. A builtin name starts with `__builtin_` and has no binding.
- `overflow_add/sub/mul` stores the converted arithmetic result through its
  place operand and returns whether that conversion overflowed.
- Side-effect hoisting turns `store` and non-atomic `update` into `write`
  statements, so the module dump contains only atomic `update`s (one
  read-modify-write). Atomic `update` and `compare_exchange` are each kept
  whole in a synthetic temporary. A fence value becomes a `fence` statement.
- `compare_exchange(place, expected, desired)`: with `form=write_back`,
  `expected` is a pointer that receives the current value on failure and the
  result is whether the exchange happened. With `form=success` and `form=old`
  (the `__sync_*_compare_and_swap` builtins), `expected` is a value, nothing
  is written back, and the result is whether the exchange happened or the
  old value respectively. `old<T>` is the place's old value inside an `update`
  computation.
- `array_decay`'s `length` is the source array's length (`None` for a VLA or
  an incomplete array).

### Operations

```ebnf
conversion = "widen" | "truncate" | "reinterpret" | "bit_cast" | "from_bool"
           | "int_to_float" | "float_widen" | "float_narrow" | "float_convert"
           | "float_to_int" | "pointer_cast" | "ptr_to_int" | "int_to_ptr"
           | "enum_to_int" | "int_to_enum"
           | "real_to_complex" | "complex_to_real" | "complex_to_imag"
           | "complex_convert"
           | "real_to_imaginary" | "imaginary_to_real" | "imaginary_to_complex"
           | "complex_to_imaginary" | "imaginary_convert"
           | "vector_splat" | "vector_bit_cast" ;
arith_op   = "add" | "sub" | "mul" | "div" | "rem"
           | "and" | "or" | "xor" | "shl" | "shr" ;
unary_op   = "neg" | "not" ;
compare_op = "eq" | "ne" | "lt" | "le" | "gt" | "ge" ;
logical_op = "logical_and" | "logical_or" ;
```

### Operation policies

```ebnf
conversion_policy = (* exact: nothing *)
                  | ", fits=" ( "always" | "unknown" )
                  | ", exact=" bool floating
                  | floating
                  | ", out_of_range=ub, exceptions=" exceptions ;
arith_policy      = [ ", elementwise=true" ] arith_contract ;
arith_contract    = (* exact: nothing *)
                  | overflow
                  | ", by_zero=ub" [ ", min_by_neg_one=ub" ]
                  | overflow ", amount_out_of_range=ub" [ ", negative_left=ub" ]
                  | ", amount_out_of_range=ub, fill=" ( "sign_extend" | "zero_extend" )
                  | floating
                  | ", complex=true" floating
                  | ", complex=true" overflow [ ", by_zero=ub" ] ;
overflow          = ", overflow=" ( "ub" | "wrap" | "trap" ) ;
floating          = ", rounding=" ( "nearest_even" | "environment" )
                    ", exceptions=" exceptions ;
exceptions        = "ignore" | "observable" ;
```

- A policy is the operation's behavior _if_ the case occurs, not a claim that
  it does. `overflow=ub` is signed C arithmetic; `wrap` is unsigned or
  `-fwrapv`; `trap` is `-ftrapv`.
- `fits` appears on `truncate`/`reinterpret`; `exact=` on `int_to_float`;
  `out_of_range=ub` on `float_to_int`; `floating` on the other float
  conversions that can round.
- The arithmetic choices are, in order: `and`/`or`/`xor`/`not` and floating
  `neg`; integer `add`/`sub`/`mul`/`neg`; `div`/`rem` (signed adds
  `min_by_neg_one`); `shl` (signed adds `negative_left`); `shr`; floating
  arithmetic; complex floating; complex integer.

## Metadata

```ebnf
metadata = "[" key "=" string "]" ;          (* only with --show-metadata *)
```

Metadata is source context keyed by node: the C type (`c`, `c_canon`,
`typedef_chain`, `c_const`, ...), storage class (`c_storage`), folded layout
queries (`size_of`, ...). It is never needed to reproduce the semantics.
Everything in `[...]` outside this production (storage, linkage, symbol
attributes, `restrict`, `synthetic`, ABI, fallthrough) is required semantics
and is always printed.

## Compact form

`--compact-ir` prints the same grammar with these parts removed:

- `, reason=...` and the conversion policy on conversions;
- the policy on arithmetic and unary operations (`add<i32>(..)`);
- `, reason=` and `, exceptions=` on comparisons;
- `, element=...` and the overflow policy on `ptr_offset`; everything after
  the type on `ptr_diff`;
- `, signature=...` on calls.

Compact mode is a reading aid. FileCheck fixtures and consumers use the
default form.
