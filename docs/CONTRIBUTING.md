# Contributing to organise

Thank you for considering contributing! This document outlines the process for reporting issues, suggesting features, and submitting pull requests.

## Code of Conduct

Be respectful and constructive in all interactions.

## Reporting Issues

- Check existing issues first.
- Provide a clear description, steps to reproduce, and your environment (OS, compiler version, etc.).
- If possible, include a minimal example that triggers the bug.

## Feature Requests

- Open an issue describing the desired feature and its use case.
- Discussion is welcome before implementation.

## Development Setup

1. Fork the repository and clone it locally.
2. Build the project:
   ```bash
   cmake -B build
   cmake --build build
3. Run tests (if any) – we are building a test suite; for now, manual verification is recommended.
4. Make your changes in a dedicated branch.

# Coding Style

* Use **C++17** features.
* Follow the existing style: 4-space indentation, no tabs.
* Use `#pragma once` for header guards.
* Prefer `std::filesystem` for path operations.
* Use `nlohmann::json` for *JSON* handling.

# Commit Messages

* Write clear, concise commit messages.
* Reference issue numbers if applicable.

# Pull Request Process

1. Ensure your changes compile and the tool works (run manual smoke tests).

2. Update the README.md and man/org.1 if you change user‑facing behavior.

3. Add a description of your changes in the pull request.

4. Wait for a review – we'll get back to you promptly.

# Testing

We welcome automated tests. If you add a feature, please consider adding a test under the `tests/` directory.

# License

By contributing, you agree that your contributions will be licensed under the MIT License (same as the project).
