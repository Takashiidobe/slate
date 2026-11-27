# Printed IR Grammar

This EBNF describes the default textual IR printed by
`slate emit-slate-ir input.c` and `slate-parser ir input.c`. See the
[IR Reference](ir.md) for node semantics. The typed module passed between
Slate's stages is an in-memory Rust representation.

## Notation

`a = ... ;` defines a production. A `|` separates alternatives, brackets mark
optional items, braces mark repetition, and parentheses group alternatives.
Quoted text is literal and `(* ... *)` is a comment. Module items and nested
bodies use four spaces per indentation level. A `body` in the grammar continues
one level deeper than its owner.

`--show-metadata`, `--show-comments`, and `--show-macros` expose additional
source context. `--compact-ir` omits some operation policies and call
signatures.

## Lexical

{{#include ../../../wiki/concepts/ir-grammar.md:lexical}}

## Module

{{#include ../../../wiki/concepts/ir-grammar.md:module}}

## Types

{{#include ../../../wiki/concepts/ir-grammar.md:types}}

## Type definitions

{{#include ../../../wiki/concepts/ir-grammar.md:type-definitions}}

## Globals

{{#include ../../../wiki/concepts/ir-grammar.md:globals}}

## Functions

{{#include ../../../wiki/concepts/ir-grammar.md:functions}}

## ABI signatures

{{#include ../../../wiki/concepts/ir-grammar.md:abi-signatures}}

## Statements

{{#include ../../../wiki/concepts/ir-grammar.md:statements}}

## Places

{{#include ../../../wiki/concepts/ir-grammar.md:places}}

## Values

{{#include ../../../wiki/concepts/ir-grammar.md:values}}

### Operations

{{#include ../../../wiki/concepts/ir-grammar.md:operations}}

### Operation policies

{{#include ../../../wiki/concepts/ir-grammar.md:operation-policies}}

## Metadata

{{#include ../../../wiki/concepts/ir-grammar.md:metadata}}
