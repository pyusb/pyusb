# Development process

## Branches and tags

The main branch is `master`, and it tracks the changes that will be included in
the next release.  This branch is kept functional (barring the occasional bug),
and its history is never rewritten.  Pull requests and patches should generally
be developed for it (i.e. using it as base).

Releases are tagged as `v<release>` (e.g. `v1.6.0`).  Updates to past minor
releases are managed in branches following the naming scheme
`<major>.<minor>.x-branch` (e.g. `1.5.x-branch`).

Other branches and tags are generally for internal use, and may be deleted or
rewritten at any time.

## Release cycle and pre-release freeze periods

Besides unscheduled patch releases, a new minor release is expected once every
6 months.

In the four weeks before a scheduled release, non-trivial changes, like complex
new features or large refactorings, stop being merged into the main branch.
This period is referred to as the _pre-release freeze._

Occasionally, scheduled releases may be anticipated (if the activity is low and
the freeze periods can be retroactively respected), downgraded (if it only
contains bug fixes and documentation improvements) or skipped (if there are no
changes).
