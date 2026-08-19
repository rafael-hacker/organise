# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] – 2026-08-18

### Added
- Initial release.
- File organization by extension or regex pattern.
- Dry‑run (`-n/--dry-run`), recursive scan (`-r/--recursive`), verbose output (`-v/--verbose`).
- Conflict strategies: interactive prompt, auto‑rename (`-y`), skip, or overwrite.
- Watch mode (`-w/--watch`) – recursively monitors directories for new files using inotify.
- Undo (`-u/--undo`) – reverts the last batch of moves via history log.
- Activity logging to `~/.local/state/organise/activity.log`.
- Cross‑device move support (copy+delete fallback).
- Default configuration auto‑creation at `~/.config/organise/config.json`.
- Shell completions for Bash, Zsh, and Fish.
- Man page (`org.1`).
- AUR packages: `organise` (stable) and `organise-git`.

### Fixed
- Argument parsing for flags (short and long options) – all flags now work correctly.
- Recursive watch – now watches subdirectories and newly created ones.
- Undo handling for cross‑device moves.

### Security
- No known issues.
