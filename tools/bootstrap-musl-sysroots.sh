#!/usr/bin/env bash
set -euo pipefail

musl_source=${MUSL_SOURCE:-${HOME}/musl}
sysroot_root=${SLATE_MUSL_SYSROOT_ROOT:-${HOME}/toolchains/slate-musl}
build_root=${SLATE_MUSL_BUILD_ROOT:-${sysroot_root}/build}
jobs=${SLATE_MUSL_JOBS:-$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1)}

pick_compiler() {
  local candidate
  for candidate in "$@"; do
    if [[ -x ${candidate} ]]; then
      printf '%s\n' "${candidate}"
      return 0
    fi
    if command -v "${candidate}" >/dev/null 2>&1; then
      command -v "${candidate}"
      return 0
    fi
  done
  return 1
}

if [[ ! -x ${musl_source}/configure ]]; then
  echo "musl source is missing configure: ${musl_source}" >&2
  exit 1
fi

declare -a targets=(
  "x86_64-linux-musl|x86_64|clang --target=x86_64-linux-musl -fuse-ld=lld"
  "i686-linux-musl|i386|clang --target=i686-linux-musl -m32 -fuse-ld=lld"
  "arm-linux-musleabihf|arm|"
  "aarch64-linux-musl|aarch64|"
)

for entry in "${targets[@]}"; do
  IFS='|' read -r target name cc <<<"${entry}"
  prefix=${sysroot_root}/${name}
  build=${build_root}/${name}

  if [[ ${name} == arm ]]; then
    if [[ -n ${SLATE_MUSL_ARM_CC:-} ]]; then
      cc=${SLATE_MUSL_ARM_CC}
    elif ! cc=$(pick_compiler \
      arm-none-linux-gnueabihf-gcc \
      "${HOME}/toolchains/arm-gnu-toolchain-15.2.rel1-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc"); then
      echo "ARM32 compiler not found; set SLATE_MUSL_ARM_CC" >&2
      exit 1
    fi
  elif [[ ${name} == aarch64 ]]; then
    if [[ -n ${SLATE_MUSL_AARCH64_CC:-} ]]; then
      cc=${SLATE_MUSL_AARCH64_CC}
    elif ! cc=$(pick_compiler aarch64-linux-gnu-gcc); then
      echo "AArch64 compiler not found; set SLATE_MUSL_AARCH64_CC" >&2
      exit 1
    fi
  fi

  mkdir -p "${build}" "${prefix}"
  signature=$(printf 'target=%s\ncompiler=%s\nmusl_source=%s\nmusl_revision=%s' \
    "${target}" \
    "${cc}" \
    "${musl_source}" \
    "$(git -C "${musl_source}" rev-parse HEAD 2>/dev/null || echo unknown)")
  if [[ -f ${build}/.slate-musl-config ]]; then
    if [[ $(<"${build}/.slate-musl-config") != "${signature}" ]]; then
      echo "${name}: existing build uses a different compiler or musl source" >&2
      echo "move ${build} aside and rerun" >&2
      exit 1
    fi
  else
    printf '%s\n' "${signature}" > "${build}/.slate-musl-config"
  fi
  if [[ ! -f ${build}/config.mak ]]; then
    (
      cd "${build}"
      CC="${cc}" \
      AR=llvm-ar \
      RANLIB=llvm-ranlib \
      "${musl_source}/configure" \
        --srcdir="${musl_source}" \
        --target="${target}" \
        --prefix="${prefix}" \
        --syslibdir="${prefix}/lib" \
        --disable-warnings
    )
  fi

  make -C "${build}" -j"${jobs}"
  make -C "${build}" install

  for required in include/stdint.h lib/libc.a lib/crt1.o; do
    if [[ ! -e ${prefix}/${required} ]]; then
      echo "${name}: missing ${prefix}/${required}" >&2
      exit 1
    fi
  done

  printf '%s\n' "${signature}" > "${prefix}/.slate-musl-target"
done
