#!/bin/bash
# Paste verbatim into the "Setup script" field of both a Claude Code cloud
# environment and a Codex cloud environment for this repo. Both run this as
# root on Ubuntu 24.04 with network access, then cache the result, so identical
# script content works on either platform unchanged.
#
# Also set these as environment variables in the same environment's config
# (both platforms have a separate env-var field, not just this script):
#   SLATE_CLANG=/opt/slate-toolchain/bin/clang
#   SLATE_CIR_OPT=/opt/slate-toolchain/bin/cir-opt
#   SLATE_ALIVE_TV=/opt/slate-toolchain/bin/alive-tv
set -euo pipefail

apt-get update -qq
DEBIAN_FRONTEND=noninteractive apt-get install -y -qq ca-certificates gh >/dev/null

mkdir -p /opt/slate-toolchain
gh release download toolchain \
  --repo Takashiidobe/slate \
  --pattern 'slate-toolchain-linux-x86_64.tar.gz*' \
  --dir /tmp --clobber
printf '%s  slate-toolchain-linux-x86_64.tar.gz\n' "$(cat /tmp/slate-toolchain-linux-x86_64.tar.gz.sha256)" \
  | (cd /tmp && sha256sum -c -)
tar xzf /tmp/slate-toolchain-linux-x86_64.tar.gz -C /opt/slate-toolchain

gh release download --repo gastownhall/beads --pattern '*_linux_amd64.tar.gz' --dir /tmp --clobber
tar xzf /tmp/beads_*_linux_amd64.tar.gz -C /usr/local/bin bd

/opt/slate-toolchain/bin/clang --version
/opt/slate-toolchain/bin/cir-opt --help >/dev/null
/opt/slate-toolchain/bin/alive-tv --version
bd --version
