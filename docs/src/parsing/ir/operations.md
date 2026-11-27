# Operations and Conversions

An operation records the type it computes in and any policy that changes its
result. Conversions are separate operations, so an IR consumer can distinguish
an input promotion from a conversion of the final result.

## Read the arithmetic contract

**C input**

```c
int add(int a, int b) { return a + b; }
```

**Default signed overflow behavior**

```text
return add<i32, overflow=ub>(read<i32>(%1), read<i32>(%2));
```

**With `-fwrapv`**

```text
return add<i32, overflow=wrap>(read<i32>(%1), read<i32>(%2));
```

Both additions operate on `i32`. The policy changes from C undefined behavior on
overflow to wrapping. The Rust lowerer can select an explicit wrapping operation
for the second case, so its behavior survives a change in the Rust build
profile.

## Keep conversions at the right width

In [Reading a Module](module.md), the `unsigned char` operand widens and then
takes the promoted signed type before `add<i32>`. Putting the conversion after
an eight-bit add could lose the carry. Conversion reasons also preserve the
distinction between source casts and implicit C rules.
[Const Fold](../../transforms/const-fold.md) uses that distinction when folding
implicit conversions.
