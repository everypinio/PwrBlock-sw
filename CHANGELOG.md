# Changelog

All notable changes to this project are documented in this file.

This project uses semantic versioning with tags like `v0.1.0`. See
[`docs/release_process.md`](./docs/release_process.md) for the release process.

## Unreleased

### Added

- GitHub Actions firmware build workflow.
- Automated GitHub Releases for `v*` tags.
- Docker-based CI build environment.
- Release artifacts for `release-tmc` and `release-cdc` presets.
- SHA-256 checksum file for release artifacts.
- GitHub artifact attestations for release artifacts.
- Automated release notes grouped by pull request labels.
- Root `VERSION` file used as the default firmware version.

### Changed

- Firmware `*IDN?` version now comes from the build-time `FIRMWARE_VERSION`
  definition instead of a hardcoded SCPI value.
- Release builds created from Git tags use the tag value without the leading
  `v` as the firmware version.

## 0.1.0

Initial firmware release baseline.
