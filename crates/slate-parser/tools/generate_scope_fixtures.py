#!/usr/bin/env python3
from pathlib import Path


def main():
    expressions = {
        "sizeof": "sizeof(T)",
        "cast": "(T) + 1",
        "generic": "_Generic((T) + 1, int: 1, default: 0)",
        "bound": "sizeof(int[(T) + 1])",
    }
    bindings = {
        "object": "int T = 2;",
        "enumerator": "enum { T = 2 };",
        "typedef": "typedef long T;",
    }
    lines = ["typedef int T;", ""]
    for binding, declaration in bindings.items():
        for expression, value in expressions.items():
            name = f"{binding}_{expression}"
            lines.extend([
                f"int {name}(void) {{",
                "  int result = 0;",
                "  {",
                f"    {declaration}",
                f"    result += {value};",
                "    {",
                "      typedef int T;",
                f"      result += {value};",
                "    }",
                f"    result += {value};",
                "  }",
                f"  return result + {value};",
                "}",
                "",
            ])
    for expression, value in expressions.items():
        lines.extend([
            f"int loop_{expression}(void) {{",
            f"  for (int T = 2, value = {value}; {value}; T += {value})",
            f"    value += {value};",
            f"  return {value};",
            "}",
            "",
            f"int parameter_{expression}(int T, int a[{value}]);",
            f"int nested_{expression}(int (*callback)(int T, int a[{value}]), int b[{value}]);",
            "",
        ])
    lines.append("// SLATE-FILECHECK-DEFINES DEFAULT")
    fixture = Path(__file__).resolve().parent.parent / "tests/fixtures/clang/linux/x86_64/parser_scope_combinations.c"
    source = "\n".join(lines) + "\n"
    if fixture.exists():
        old = fixture.read_text()
        marker = "// SLATE-FILECHECK-BEGIN "
        if marker in old:
            source += "\n" + old[old.index(marker):]
    fixture.write_text(source)


if __name__ == "__main__":
    main()
