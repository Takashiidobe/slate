# IR Grammar

<!-- toc -->
- [Notation](#notation)
- [Lexical](#lexical)
- [Module](#module)
- [Types](#types)
- [Type definitions](#type-definitions)
- [Globals](#globals)
- [Functions](#functions)
- [ABI signatures](#abi-signatures)
- [Statements](#statements)
- [Places](#places)
- [Values](#values)
  - [Operations](#operations)
  - [Operation policies](#operation-policies)
- [Metadata](#metadata)
- [Compact form](#compact-form)
<!-- /toc -->

EBNF of the IR printed by `slate-parser ir source.c` (also
`parse source.c --dump-ir`). Meaning: [IR Spec](ir-spec.md).

Source of truth: `src/ir/module_print.rs` (module, declarations,
statements), `Value::format` in `src/ir/mod.rs`, `Place::format` in
`src/ir/declarations.rs`, and the `Display` impls in `src/ir/numeric.rs`,
`src/ir/abi.rs`, `src/ir/module.rs`. Update this page with any change to
what they print.

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
  `--dump-ir-expressions`, and `--dump-ir-names`, and the
  `[spelling=..., expansion=...]` suffixes `--show-spans` adds.

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
module     = "module" "{" target { asm } { type_def } { global } { function } "}" ;

target     = "target" string "{"
               "endian" "=" ( "little" | "big" ) ";"
               "pointer" "[size=" int ", align=" int "]" ";"
               "stack_alignment" "=" int ";"
               "long_double" "=" float_type ";"
               { "storage" storage_of "[size=" int ", align=" int "]" ";" }
             "}" ;
storage_of = "bool" | "i8, u8" | "i16, u16" | "i32, u32" | "i64, u64"
           | "i128, u128" | "bf16" | "f16" | "f32" | "f64" | "f80" | "f128"
           | "d32" | "d64" | "d128" ;
```

- `storage` lines cover every scalar format a module can name (`bool`,
  standard integers, floats, in that order), only for formats the target
  supports. Bit-precise integers, pointers, `va_list`, and composites are
  derived at the use site.
- A module-level [`asm`](#statements) is a file-scope `asm("...")`, in
  source order before the types. No qualifiers or goto labels; operands
  only under the GNU personality, so it is almost always the single-`;`
  form.

## Types

```ebnf
type          = "void" | "bool" | "va_list" | numeric
              | "complex<" numeric ">"
              | "imaginary<" float_type ">"
              | "vector<" numeric ", " int ">"
              | [ "sat_" ] "fixed<" int_type ", " int ">"
              | "ptr<" [ "const " ] [ access_prefix ] type [ ", " ptr_space ] ">"
              | "array<" type ", " ( int | "incomplete" ) ">"
              | "vla<" type ", " ( binding | "*" ) ">"
              | fn_type
              | type_ref ;
numeric       = int_type | float_type ;
int_type      = ( "i" | "u" ) digits [ "b" ] ;
float_type    = "bf16" | "f16" | "f32" | "f64" | "f80" | "f128"
              | "d32" | "d64" | "d128" ;
access_prefix = "volatile " | "atomic " | "volatile atomic " ;
ptr_space     = "ptr32_sptr" | "ptr32_uptr" | "ptr64" ;
fn_type       = "fn" [ " " call_conv ] "(" [ fn_params ] ") -> " type ;
call_conv     = "stdcall" | "fastcall" | "vectorcall" | "thiscall" ;
fn_params     = "unprototyped" | type { ", " type } [ ", ..." ] | "..." ;
```

- Types are concrete and target-resolved; the C spelling is metadata.
- `b` marks a bit-precise integer (`_BitInt(N)`), distinct from the standard
  integer of the same width: `i128b` is not `i128`.
- `d32`/`d64`/`d128` are decimal floating types, never converted to binary.
- `vector<T, N>` is a GNU `vector_size` or Clang `ext_vector_type` type with
  `N` lanes of the scalar `T`; both source forms print the same way.
- `fixed<iW, S>` is an N1169 fixed-point type: a two's-complement `iW`/`uW`
  storage word with `S` fractional bits, leaving `W - S - signed` integral
  bits. `sat_fixed<..>` is the `_Sat` form, whose operations saturate where
  the plain form is undefined. `_Fract` and `_Accum` of the same rank differ
  only in width, so the kind is not printed.
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
                [ "const " ] [ access_prefix ] type [ ":" int ] { metadata } ";" ;
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
               [ "[align=" int "]" ] [ "[cleanup=%" int "]" ]
               [ "[register=" string "]" ] [ "=" value ] ;
storage      = "automatic" | "static" | "thread" ;
linkage      = "[linkage=" ( "internal" | "external" ) "]" ;
symbol_attrs = [ "[asm_name=" string "]" ] [ "[visibility=" visibility "]" ]
               [ "[weak]" ] [ "[alias=" string "]" ] [ "[weakref=" string "]" ]
               [ "[ifunc=" string "]" ]
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
              [ "[naked]" ]
              [ "[memory=" ( "none" | "read" ) "]" ]
              { "[deallocator=%" int ", argument=" int "]" }
              [ "[abi=" ( abi_signature | "incomplete" ) "]" ]
              [ "[fallthrough=" fallthrough "]" ] { metadata }
              ( ";" | "{" { statement } "}" ) ;
params      = "unprototyped" | param { ", " param } [ ", ..." ] | "..." ;
param       = binding ( c_identifier | "<unnamed>" ) ":" [ access_prefix ] type
              [ "[restrict]" ] [ "[const]" ] [ array_param ] { metadata } ;
array_param = "[array=" ( "static" [ " " extent ] | extent ) "]" ;
extent      = integer | binding | "*" ;
fallthrough = "ret_zero" | "ret_void" | "ub_if_used" | "ub"
            | "ret(" value ")" ;
```

- One `fn` per function: the body and parameters come from the definition,
  or else the first prototype.
- `[abi=...]` is printed only when some argument or the result is not passed
  as a plain scalar, or the calling convention is not the default C one.
  `[abi=incomplete]` marks a declaration whose parameter or result type is a
  record still incomplete at the end of the translation unit.
- `fallthrough` is present only on definitions and says what reaching the end
  of the body means. `ret(value)` evaluates the value only when control reaches
  that point; its local bindings belong to the function body.
- Inlining preference and definition emission are independent. Inline bodies
  print whether they supply a linkable definition; `inline_only` bodies do not.
  `noreturn` survives compact printing and makes fallthrough unconditionally `ub`.
- `[naked]` is `__attribute__((naked))` from any declaration: no prologue or
  epilogue, fallthrough `ub`, and its asm statements print no `[options=...]`.
- `memory=none` (GNU `const`) reads and writes no memory beyond the arguments;
  `memory=read` (`pure`) may read but not write. Absent means unrestricted.

## ABI signatures

```ebnf
abi_signature = convention [ " " call_conv ] "(" [ abi_pass { ", " abi_pass } ] ") -> "
                abi_pass ;
convention    = "sysv64" | "win64" | "x86_cdecl" | "x86_win32" | "aapcs64" | "win_arm64"
              | "aapcs32" | "aapcs32_hard_float" ;
abi_pass      = "void" | "scalar" | "direct" | "native_c"
              | "coerce<" chunk { ", " chunk } ">"
              | "byval<align=" int ">" | "byref<align=" int ">" | "sret<align=" int ">" ;
chunk         = "i" digits | float_type | "pair<" float_type ">" | "quad<" float_type ">" ;
```

- `native_c`: rustc's `extern "C"` passes a same-layout `repr(C)` type
  exactly as the C compiler does. Other record and complex shapes are
  explicit because rustc would differ.
- `pair<T>` / `quad<T>`: two or four `T` lanes in one SSE eightbyte.
- `direct`: passed in registers as its own type, no coercion or memory
  copy; used by vectors that fit the target's vector registers.

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
           | asm
           | "{" { metadata } body "}" ;
asm        = "asm" [ " volatile" ] [ " inline" ] [ " goto" ] string
               [ " [dialect=" ( "att" | "intel" ) "]" ]
               [ " [options=" asm_options "]" ]
               [ " [alternative=" ( integer | "none" ) "]" ]
               ( { metadata } ";"
               | "{" { metadata }
                   [ "template:" { asm_piece } ";" ]
                   { asm_operand }
                   [ "rejected:" asm_reject { "," asm_reject } ";" ]
                   [ "clobbers:" clobber { "," clobber } ";" ]
                   [ "labels:" binding { "," binding } ";" ]
                 "}" ) ;
asm_operand = ( "in" integer [ "[" c_identifier "]" ] [ string asm_classes ]
                  [ asm_chosen ] [ asm_width ]
                  ( value | asm_place | asm_symbol | asm_memory )
              | ( "out" | "lateout" ) integer [ "[" c_identifier "]" ] string
                  asm_classes [ asm_chosen ] [ asm_width ] asm_place
              | ( "inout" | "inlateout" ) integer [ "[" c_identifier "]" ] string
                  asm_classes [ asm_chosen ] [ asm_width ] asm_place
                  [ " from" value ] ) ";" ;
asm_options = asm_option { "," asm_option } ;
asm_option = "pure" | "nomem" | "readonly" | "nostack" | "preserves_flags"
           | "may_unwind" ;
asm_chosen = "->" asm_class ;
asm_width  = "width" integer ;
asm_reject = integer "(operand" integer ":" ( "unresolved(" string ")"
           | "clobber-only" | "width" | "not-constant" | "matching" | "missing" ) ")" ;
asm_classes = "[" asm_alt { ", " asm_alt } "]" ;
asm_alt    = "{" identifier "}" | integer
           | [ asm_class { " | " asm_class } ] ;
asm_class  = "reg" | "reg_abcd" | "reg_legacy" | "vreg_low8" | "xmm_reg" | "ymm_reg" | "zmm_reg" | "kreg" | "x87_reg"
           | "mmx_reg" | "vreg" | "vreg_low16" | "sreg" | "dreg"
           | "mem" | "imm" | "sym" | "{" identifier "}" | "unresolved(" string ")" ;
asm_place  = "place<" type [ ", volatile" ] ">(" place ")" ;
asm_symbol = "sym<offset=" [ "-" ] integer ">(" binding ")" ;
asm_memory = "mem<" ( "read" | "write" | "readwrite" ) ">" asm_place ;
asm_piece  = string | "%" [ letter ] integer [ asm_view ] | "%l" integer
           | "%%" | "%=" | asm_address | "label(" identifier ")"
           | "entry_label(" binding ", " identifier ")" ;
asm_address = "addr" [ "<" identifier ">" ] "(%" integer [ " + " identifier ]
              [ " + " identifier "*" integer ] [ ( " + " | " - " ) integer ] ")" ;
asm_view   = "(" ( integer | "high8" ) ")" ;
clobber    = "memory" | "cc" | "unwind" | register ;
register   = string [ "as" identifier ] ;
simple     = "let" binding ":" type "[synthetic" [ ", unsequenced" ] "]"
             [ "=" value ]
           | "let" variable
           | "write<" type access [ ", unsequenced" ] [ ordering ] ">("
             place ", " value ")"
           | "fence<scope=" ( "thread" | "signal" ) ", order=" order
             [ sync_scope ] ">"
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
- An `asm` with no operand sections prints as a single `;` statement; that
  is basic asm, whose template is emitted verbatim. The quoted string after
  the qualifiers is always the raw source template; `template:` is the same
  text with `%`-directives resolved. In a resolved piece, `%N` is an index
  into the operand lines, and `%lN` is an index into the `labels:` list —
  neither is necessarily the number the source wrote, because a tied input
  is folded into its output and later operands renumber. An operand piece may carry a one-letter target modifier (`%a1`).
  `(N)` after it is the register slice in bits the reference prints,
  overriding the operand's own width: a width modifier (`%k0(32)`,
  `%h0(high8)`), or on x86 a tied input narrower or wider than its
  output (`%0(32)`).
- `width N` is the operand's storage size in bits; absent when the type
  has no fixed size (a VLA under `"m"`).
- `[alternative=N]` is the constraint alternative chosen for the whole
  asm, printed when there is more than one; `none` means no alternative
  fits Rust. `-> class` is the class an operand took when its chosen
  alternative offered several (`rm`, `g`). `rejected:` lists each
  alternative tried before the chosen one with the first operand that
  ruled it out, numbered as in the source.
- `[dialect=...]` is the assembler syntax the template is written in; it is
  present on x86 and absent on targets without a dialect choice. `template:`
  holds only the selected side of each `{att|intel}` alternation, so it
  never contains one.
- `[options=...]` is the set of Rust `asm!` options derived during lowering,
  in that fixed order, printed on every statement asm except inside a
  naked function, and never on file-scope asm; `nomem` and `readonly` are exclusive.
  It is left out when no option is set, as for MSVC `__asm`.
- An operand line starts with its Rust-facing direction. `=` and `+` and
  `&` are folded into it and no longer appear in the quoted constraint,
  which keeps `,`-separated alternatives, each with `%` or `-` and then a
  hard register `{reg}`, a matching operand number, or constraint letters.
- The bracket after the constraint resolves it, one entry per alternative:
  a hard register or explicit-register letter (`a` is `{ax}`) as its
  canonical name, a matching operand number, or the letters' classes joined
  by `|`. Class names are Rust's, except `reg_legacy` and `vreg_low8`,
  subsets that emission pins to an explicit register; a letter we cannot
  resolve prints as `unresolved("l")`.
- An `in` with an `asm_place` is a memory-capable input naming its object;
  the asm may address it in place, so no load precedes the statement.
- An `in` with an `asm_symbol` selected `sym`: a link-time address, the
  binding's symbol plus a byte offset, which Rust takes as a `sym` operand.
- MSVC `__asm` lowers to the same node with `[dialect=intel]`. Its quoted
  string is the statement rebuilt from the AST (instructions joined by `\n`,
  numbers in decimal), not raw source. Its operands carry no constraint:
  `mem<access>` is a C object the asm addresses, so the operand is the
  object's address; `access` is what the asm may do there. A function is a
  `sym`, and `offset x` is an `addr_of` value. In `template:`,
  `addr<size>(%N + base + index*scale + disp)` is a memory reference into
  operand `N` (the size appears where MASM needs it spelled out), and
  `label(name)` is an asm-local label, lowercased, which emission renumbers
  because Rust `asm!` rejects named labels. `entry_label(%N, name)` defines
  a label inside MSVC asm that C `goto` can enter at that position; its
  binding is the target of the `goto`. Register-only memory such as
  `dword ptr [esp + 12]` stays text.
- `from value` on an `inout`/`inlateout` is a tied input (`"0"`); without
  it the place itself is read (`"+r"`).
- A clobbered or hard-coded register prints its source spelling, plus
  `as <canonical>` when the target's register table recognized it. The
  register's width is not carried: a clobber names the whole register, and
  an operand carries its own `width`.
- `evaluation` is a loop condition or `for` increment with hoisted side
  effects: the statements run each time, then `yield` gives the value. An
  increment yields `void`.
- `[synthetic]` lets are temporaries that lowering introduced (hoisted old
  values, short-circuit results, VLA extents). They have no source variable.
- `unsequenced` on a temporary or a write marks a statement whose position
  commits to an evaluation order C leaves unspecified, so a different C
  compiler may order it differently. See
  [unsequenced effects](ir/control-flow.md#unsequenced-effects-follow-clang).
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
       | "lane(" place ", " value ")"
       | "swizzle<lanes=[" digits { ", " digits } "]>(" place ")"
       | "compound_literal" binding "[storage=" storage "]"
         [ "[align=" int "]" ] "=" value
       | "temporary" binding "=" value ;
access   = [ ", volatile" ] ;
ordering = ", atomic=" order [ sync_scope ] ;
order    = "relaxed" | "consume" | "acquire" | "release" | "acq_rel"
         | "seq_cst" | "dynamic(" value ")" ;
sync_scope = ", sync_scope=" scope ;
scope    = "device" | "workgroup" | "wavefront" | "single" | "cluster"
         | "dynamic(" value ")" ;
```

- A place is a storage location, not a read. Only `read`, `write`, `store`,
  `update`, `compare_exchange`, `addr_of`, the decays and the `va_*` values
  use one.
- `ordering` belongs to the operation, not the place: it is absent on a
  non-atomic access and follows `access` when present (`read<i32, volatile,
  atomic=acquire>`). `dynamic(v)` is a runtime ordering value.
- `sync_scope` is the synchronization scope of an atomic operation, from the
  `__scoped_atomic_*` builtins. The system scope is the default and is never
  printed, so it has no spelling here; every other scope prints after the
  ordering, and `dynamic(v)` is a runtime scope value.
- `bitfieldN<unit=U, bytes=A..B, bits=C..D>` is field `N`, stored in storage
  unit `U` (record bytes `A..B`), occupying bits `C..D` from the least
  significant bit of that unit. Bit-fields are not addressable.
- `lane(p, i)` is element `i` of the vector place `p`. Like a bit-field it is
  readable and writable but not addressable, and `i` is a runtime value, not a
  constant. A vector *value* is indexed by the `lane<T>(..)` value instead.
- `swizzle<lanes=[..]>(p)` is the several-lane form, for an `ext_vector`
  component selection such as `v.xy` or `v.lo`. Its lanes are constants and
  distinct, since a repeated component is not assignable; a repeated or
  out-of-range selection is a `shuffle<..>` value instead.
- `compound_literal %N` is a distinct object with its own storage duration.
  `[align=N]` is printed only when an `_Alignas` in its type name changes
  the natural alignment.
- `temporary %N = v` materializes an aggregate rvalue (`f().x`) so a
  member can be projected. Not an object: places rooted in one are neither
  assignable nor addressable.
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
             [ sync_scope ] ">(" place ", " value ", " value ")"
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
             ">(" ( binding | value ) { ", " value } ")"
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
           | logical_op "<" type ">(" value ", " value ")"
           | "float_class<bool, test=" float_class ">(" value ")"
           | "lane<" type ">(" value ", " value ")"
           | "shuffle<" type ", mask=" shuffle_mask ">(" value [ ", " value ] ")" ;
shuffle_mask = "[" lane { ", " lane } "]" | "dynamic(" value ")" ;
lane       = digits | "undef" ;
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
- `lane<T>(v, i)` extracts element `i` from the vector value `v`, for a
  subscript whose base is not an lvalue. An lvalue base yields a `lane(..)`
  place instead, so `v[i] = x` stays a write to that place.
- `shuffle<T, mask=[..]>(v1, v2)` selects lanes from the concatenation of its
  operands, so an index reaches the second operand once it passes the first
  operand's lane count; `undef` is a lane whose value is not observed
  (`__builtin_shufflevector`'s `-1`). A one-operand shuffle indexes only that
  operand and comes from an `ext_vector` swizzle. `mask=dynamic(v)` is the
  two-operand `__builtin_shufflevector` form, whose mask is a runtime integer
  vector, not constants.
- `float_literal` is a round-trippable decimal (`1.0`, `-0.0`, `inf`); a NaN
  prints as `bits=0x...` to keep its payload. Decimal floats keep their digit
  spelling.
- `aggregate` members are resolved targets in order; nested subobjects are
  nested `aggregate`s. `zero_fill=true` means some member or element was
  omitted and is zero-initialized. A union has exactly one member.
- `call` names a direct callee by `binding` and an indirect callee by a pointer
  value. A function-like builtin is a direct callee: its `fn` is either the
  source declaration that kept builtin status or an implicit declaration named
  by the builtin's spelling. `signature` is the type visible at this call site,
  which can differ from the function's final declaration.
- `overflow_add/sub/mul` stores the converted arithmetic result through its
  place operand and returns whether that conversion overflowed.
- `float_class` tests one real floating operand against an IEEE class and
  results in `bool`; it never raises, so the classification builtins that
  return `int` wrap it in `from_bool<int>`.
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
           | "float_to_int" | "pointer_cast" | "address_space_cast"
           | "ptr_to_int" | "int_to_ptr"
           | "enum_to_int" | "int_to_enum"
           | "real_to_complex" | "complex_to_real" | "complex_to_imag"
           | "complex_convert"
           | "real_to_imaginary" | "imaginary_to_real" | "imaginary_to_complex"
           | "complex_to_imaginary" | "imaginary_convert"
           | "vector_splat" | "vector_bit_cast"
           | "int_to_fixed" | "fixed_to_int" | "float_to_fixed"
           | "fixed_to_float" | "fixed_convert" ;
arith_op   = "add" | "sub" | "mul" | "div" | "rem"
           | "and" | "or" | "xor" | "shl" | "shr"
           | "minnum" | "maxnum" | "minimum" | "maximum"
           | "minimum_num" | "maximum_num" ;
unary_op   = "neg" | "not" ;
compare_op = "eq" | "ne" | "lt" | "le" | "gt" | "ge" ;
logical_op = "logical_and" | "logical_or" ;
float_class = "nan" | "infinite" | "finite" | "normal" | "subnormal"
            | "zero" | "signaling" | "sign_bit" ;
```

### Operation policies

```ebnf
conversion_policy = (* exact: nothing *)
                  | ", fits=" ( "always" | "unknown" )
                  | ", exact=" bool floating
                  | floating
                  | ", out_of_range=ub, exceptions=" exceptions
                  | fixed ;
arith_policy      = [ ", elementwise=true" ] arith_contract ;
arith_contract    = (* exact: nothing *)
                  | overflow
                  | ", by_zero=ub" [ ", min_by_neg_one=ub" ]
                  | overflow ", amount_out_of_range=ub" [ ", negative_left=ub" ]
                  | ", amount_out_of_range=ub, fill=" ( "sign_extend" | "zero_extend" )
                  | floating ", contract=" ( "off" | "on" | "fast" )
                  | ", complex=true" floating ", range=" ( "basic" | "full" )
                  | ", complex=true" overflow [ ", by_zero=ub" ]
                  | fixed [ ", by_zero=ub" ] [ ", amount_out_of_range=ub" ] ;
fixed             = ", overflow=" ( "ub" | "saturate" )
                    ", rounding=toward_zero" ;
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
  arithmetic (`add`/`sub`/`mul`/`div` and the six extrema); complex
  floating; complex integer; fixed-point.
- `contract=` is the fused multiply-add permission of floating arithmetic:
  `off` never fuses, `on` may fuse within one source expression, `fast` may
  fuse across statements. `range=` is the complex multiply/divide algorithm:
  `full` handles infinities and NaNs (C Annex G), `basic` may use the plain
  textbook formulas. Both are permissions; ignoring them is always correct.
- A fixed-point contract is `overflow=saturate` for a `_Sat` type and
  `overflow=ub` otherwise, and always `rounding=toward_zero`, which is the
  fractional bits the operation discards. It appears on `add`/`sub`/`mul`/
  `div`/`shl`/`shr`/`neg` and on every conversion into or out of a
  fixed-point type except one that is exact.

## Metadata

```ebnf
metadata = "[" key "=" string "]" ;          (* only with --show-metadata *)
```

- Metadata is source context keyed by node: C type (`c`, `c_canon`,
  `typedef_chain`, `c_const`, …), storage class (`c_storage`), folded
  layout queries (`size_of`, …). Never needed for semantics.
- Every other `[...]` (storage, linkage, symbol attributes, `restrict`,
  `synthetic`, ABI, fallthrough) is semantics and always printed.

## Compact form

`--compact-ir` prints the same grammar with these parts removed:

- `, reason=...` and the conversion policy on conversions;
- the policy on arithmetic and unary operations (`add<i32>(..)`);
- `, reason=` and `, exceptions=` on comparisons;
- `, element=...` and the overflow policy on `ptr_offset`; everything after
  the type on `ptr_diff`;
- `, signature=...` on calls.

Reading aid only; fixtures and consumers use the default form.
