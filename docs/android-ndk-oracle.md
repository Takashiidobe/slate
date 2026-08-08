# Android NDK oracle

Slate uses Android NDK r27d (`27.3.13750724`) as the pinned Bionic ABI oracle.
It is the current LTS release selected for stability across the Android epic.
The initial AArch64 and x86-64 support baseline is Android API 21, the first API
level supporting Android's 64-bit ABIs.

Review the Android NDK license on the official download page, then materialize
the Linux x86-64 package under `target/` with:

```bash
tools/bootstrap-android-ndk.sh --accept-license
```

`ANDROID_NDK_ACCEPT_LICENSE=1` is the non-interactive equivalent. The script
pins the official archive name, byte size, URL, published SHA-1, and a derived
SHA-256 checksum. It stores the archive in `target/android-ndk-cache`, unpacks
the NDK into
`target/android-ndk-oracle/ndk`, validates the revision and API-specific
libraries, and records the target contract in
`target/android-ndk-oracle/oracle.json`. A valid externally installed copy of
the same NDK revision can be used without copying it:

```bash
SLATE_ANDROID_NDK=/path/to/android-ndk-r27d \
  tools/bootstrap-android-ndk.sh
```

Automatic acquisition supports Linux x86-64. Other hosts must provide
`SLATE_ANDROID_NDK`. All downloaded and derived data remains ignored under
`target/`.

## Probes

Run one probe by architecture, API level, and mode:

```bash
tools/probe-android-ndk.sh --arch aarch64 --api 21 --mode layouts
tools/probe-android-ndk.sh --arch x86_64 --api 21 --mode symbols
```

Available modes are `predefined`, `header-macros`, `preprocess`, `ast`,
`layouts`, `assembly`, `availability`, and `symbols`. Results are written under
`target/android-ndk-oracle/probes/<architecture>/api-<level>`. The symbol mode
records both symbols emitted by the representative object and the selected API
level's public `libc.so` dynamic-symbol surface. These are narrow ABI checks;
Slate does not take responsibility for ELF generation or final NDK linking.

Run the complete AArch64 and x86-64 oracle smoke test with:

```bash
ANDROID_NDK_ACCEPT_LICENSE=1 tools/test-android-ndk-oracle.sh
```

To update the oracle, change the release, revision, archive identity, checksum,
and documented API contract together. Remove `target/android-ndk-oracle/ndk`
before rerunning with a different pin; the script rejects mismatched revisions.
