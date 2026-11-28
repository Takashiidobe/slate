# Project Manifest

Use a manifest to name C builds and record the preprocessor configurations Slate
should translate. Pass it to `slate translate-project --manifest FILE`. A
version 1 manifest looks like this:

```json
{
  "version": 1,
  "targets": {
    "linux-aarch64": {
      "triple": "aarch64-unknown-linux-gnu",
      "flavor": "clang",
      "compile_commands": "build-clang/compile_commands.json",
      "build_target": "zlibstatic"
    }
  },
  "features": {
    "fastest": ["FASTEST"]
  }
}
```

| Field              | Meaning                                                                                                                |
| ------------------ | ---------------------------------------------------------------------------------------------------------------------- |
| `version`          | Manifest format version, currently `1`.                                                                                |
| `targets`          | Map from names used by `--manifest-target` to C build configurations.                                                  |
| `triple`           | Rust target triple whose ABI Slate uses for that translation.                                                          |
| `flavor`           | Optional compiler flavor override.                                                                                     |
| `compile_commands` | Compilation database. Resolve a relative path from the project root passed to `translate-project`.                     |
| `build_target`     | Optional selection of a C build target from that database.                                                             |
| `sysroot`          | Optional target sysroot override.                                                                                      |
| `features`         | Map from Cargo feature names to lists of C preprocessor definitions. A definition can be a macro name or `NAME=VALUE`. |
| `invalid_features` | Feature combinations the C project disallows.                                                                          |

Run the editing commands from the project root:

```sh
slate project target add slate.json linux-aarch64 build-clang/compile_commands.json \
  --triple aarch64-unknown-linux-gnu --build-target zlibstatic
slate project feature add slate.json FASTEST
```

Use `--name` on `project feature add` to choose the Cargo feature name. Use
`project feature conflict FILE FEATURE FEATURE` to record features that cannot
be enabled together. Keep feature macros out of the base compile commands so the
manifest can toggle them. Explicit targets in compile commands must agree with
the manifest triple.

For a single configuration, pass one `--manifest-target` and select enabled
features with `--features`. Use `--merge` to generate one crate for the manifest
configurations. See
[Translate zlib for two platforms](../tutorials/cross-platform.md) and
[Merge](../project/merge.md).
