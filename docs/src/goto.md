# Control flow

Slate consumes structured statements and control-flow facts from slate-parser
IR. Rust lowering lives in `frontend/lowerer/{statements,switch}.rs`.

Use `lowering-barriers` on a fixture to determine whether its control-flow
construct is supported. Parser support does not imply Rust lowering support.
The [IR control-flow specification](https://github.com/takashiidobe/slate/blob/main/wiki/concepts/ir/control-flow.md)
defines the boundary; [testing](testing.md) defines the runtime gate.
