# Target-qualified FileCheck is independent of fixture flavor

_created 2026-08-24_

`FixtureFlavor` describes fixture discovery and the platform-specific C oracle
setup. It is not a general code-generation target abstraction.

Target-specific source checks use conjunctive FileCheck prefixes such as
`REWRITES-MACOS` or `LOWERING-BIONIC-X86_64`. The runner enables only the
prefix for both the active nextest profile and translation target.

The C compiler may require a versioned target triple while Slate accepts a
canonical triple, so each platform flavor supplies those values separately.

Tests that need an extra translation target without a platform fixture tree use
`TargetCheckFixture`. For example, the x87 long-double case translates the
existing linked fixture for `x86_64-unknown-linux-gnu`; x87 is not a fixture
flavor.
