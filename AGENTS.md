# AGENTS.md - FidelKey Development Tracking

This file tracks all development activities for the FidelKey project.

## Project Overview
FidelKey is a lightweight, free, ad-free Amharic keyboard for Windows 10/11 using TSF for system-wide input. Architecture: Native C++ TSF service + Tauri 2/React/TypeScript settings app.

## Development Rules
- **DO NOT** install C++ workloads, Windows SDKs, compilers, or large dependencies locally
- **DO NOT** run local builds/tests requiring these tools
- All builds, compilation, tests go through GitHub Actions on Windows runner
- Write code, document requirements, fix based on user feedback
- User will commit/push and test builds themselves
- **DO NOT** commit/push on behalf of user
- **DO NOT** make significant local environment changes without permission

## Status
- **Start Date:** 2026-10-24
- **Status:** Milestone 1 - Mapping specification (IN PROGRESS - validation phase)
- **Phase:** Engine validation and testing
- **Build State:** Code written, untested locally (awaiting CI)

## Activity Log

### 2026-10-24
- [x] Read BUILD.md and updated requirements
- [x] Explored repo - only BUILD.md existed
- [x] Initialized git repository
- [x] Created AGENTS.md for tracking
- [x] Created project structure (engine, tsf-service, settings-app, installer, .github/workflows, docs)
- [x] Researched GFF Amharic layout patterns
- [x] Created mapping specification document
- [x] Designed conversion engine API
- [x] Implemented trie-based prefix matching
- [x] Implemented mapping table with GFF-style mappings
- [x] Implemented PhoneticEngine with improved composition logic
- [x] Created expanded tests (selam, basic mappings, backspace, mixed)
- [x] Set up CMake build for engine
- [x] Created GitHub Actions workflow for Windows builds

## Milestones
1. **Milestone 1 — Mapping specification** (IN PROGRESS - validation phase)
2. **Milestone 2 — Conversion engine** (started, needs testing)
3. **Milestone 3 — Windows integration (TSF)**
4. **Milestone 4 — Settings interface (Tauri)**
5. **Milestone 5 — Installer**
6. **Milestone 6 — Testing and release**

## Current Focus
- Validating mapping accuracy against GFF reference
- Expanding automated tests
- Ensuring deterministic behavior for overlapping sequences
- Verifying GitHub Actions workflow configuration

## Known Issues / Uncertainties
- Engine logic needs testing - especially composition/commit behavior
- Some mappings (families ሀ/ሐ/ኀ distinction) need verification
- Need to confirm "selam" produces correct output in practice
- Backspace behavior needs validation
- Mixed English/Amharic handling needs testing

## Next Actions
1. Push to GitHub and run CI to see actual test results
2. Based on CI results, fix any mapping or logic issues
3. Expand tests further if needed
4. Only proceed to TSF service once engine tests pass
