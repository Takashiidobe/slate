# Slate lowering barriers: /home/takashi/c-corpus/chibicc/compile_commands.json

Regenerate with `python3 tools/corpus_barriers.py --output wiki/concepts/chibicc-lowering-barriers.md`.

- slate `fd807d6ca008`
- slate-parser `9494cb503fdb-dirty`
- functions lowered: 21 / 297

| TU | functions ok | module barriers | declaration barriers | first barrier |
| --- | ---: | ---: | ---: | --- |
| codegen.c | 6/33 | 11 | 6 | `extern global` |
| hashmap.c | 1/12 | 0 | 0 | `type struct` |
| main.c | 2/28 | 27 | 15 | `extern global` |
| parse.c | 4/113 | 26 | 21 | `extern global` |
| preprocess.c | 1/53 | 10 | 18 | `extern global` |
| strings.c | 0/2 | 0 | 3 | `type struct` |
| tokenize.c | 2/34 | 19 | 13 | `extern global` |
| type.c | 0/15 | 13 | 2 | `initialized global` |
| unicode.c | 5/7 | 4 | 0 | `initialized global` |

### Defined functions by first barrier

| count | barrier |
| ---: | --- |
| 215 | `type struct` |
| 8 | `place Field` |
| 8 | `value null` |
| 8 | `type enum` |
| 6 | `variadic definition` |
| 6 | `value logical_and` |
| 5 | `statement Switch` |
| 5 | `value not` |
| 3 | `value aggregate` |
| 3 | `value logical_or` |
| 3 | `value ptr_diff` |
| 2 | `value function_decay` |
| 1 | `value neg` |
| 1 | `type fn` |
| 1 | `type va_list` |
| 1 | `value conditional` |

### Module-level barriers

| count | barrier |
| ---: | --- |
| 49 | `zero-initialized global` |
| 31 | `initialized global` |
| 30 | `extern global` |

### Declaration barriers

| count | barrier |
| ---: | --- |
| 75 | `type struct` |
| 2 | `type fn` |
| 1 | `type f80` |

### TUs by first reported barrier

| count | barrier |
| ---: | --- |
| 5 | `extern global` |
| 2 | `type struct` |
| 2 | `initialized global` |

