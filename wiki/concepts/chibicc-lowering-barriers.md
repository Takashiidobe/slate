# Slate lowering barriers: /home/takashi/c-corpus/chibicc/compile_commands.json

<!-- toc -->
- [Defined functions by first
  barrier](#defined-functions-by-first-barrier)
- [Module-level barriers](#module-level-barriers)
- [Declaration barriers](#declaration-barriers)
- [TUs by first reported barrier](#tus-by-first-reported-barrier)
<!-- /toc -->

Regenerate with `python3 tools/corpus_barriers.py --output wiki/concepts/chibicc-lowering-barriers.md`.

- slate `def0421abc26-dirty`
- slate-parser `def0421abc26`
- functions lowered: 85 / 297

| TU | functions ok | module barriers | declaration barriers | first barrier |
| --- | ---: | ---: | ---: | --- |
| codegen.c | 11/33 | 2 | 5 | `field 15 of record _IO_FILE` |
| hashmap.c | 12/12 | 0 | 0 | `clean` |
| main.c | 22/28 | 3 | 9 | `field 15 of record _IO_FILE` |
| parse.c | 6/113 | 21 | 16 | `type f80` |
| preprocess.c | 11/53 | 1 | 11 | `type f80` |
| strings.c | 1/2 | 0 | 3 | `field 15 of record _IO_FILE` |
| tokenize.c | 15/34 | 11 | 11 | `field 15 of record _IO_FILE` |
| type.c | 0/15 | 13 | 2 | `type f80` |
| unicode.c | 7/7 | 4 | 0 | `initialized global` |

### Defined functions by first barrier

| count | barrier |
| ---: | --- |
| 208 | `type f80` |
| 4 | `field 15 of record _IO_FILE` |

### Module-level barriers

| count | barrier |
| ---: | --- |
| 44 | `type f80` |
| 6 | `initialized global` |
| 5 | `field 15 of record _IO_FILE` |

### Declaration barriers

| count | barrier |
| ---: | --- |
| 38 | `type f80` |
| 19 | `field 15 of record _IO_FILE` |

### TUs by first reported barrier

| count | barrier |
| ---: | --- |
| 4 | `field 15 of record _IO_FILE` |
| 3 | `type f80` |
| 1 | `clean` |
| 1 | `initialized global` |

