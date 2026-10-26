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
- **Status:** Milestone 1 - Mapping specification (VALIDATED)
- **Phase:** Milestone 1 complete, Milestone 2 in progress
- **Build State:** Engine build + tests PASSED in CI (GitHub Actions)

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
- [x] Implemented PhoneticEngine with composition logic
- [x] Created expanded tests (selam, basic mappings, backspace, mixed, punctuation, overlapping)
- [x] Set up CMake build for engine
- [x] Created GitHub Actions workflow for Windows builds
- [x] Pushed to GitHub, CI build-engine job SUCCEEDED (30s)
- [x] Engine tests passed in CI
- [x] Added TSF service skeleton
- [x] Added Tauri settings app skeleton

## Milestones
1. **Milestone 1 — Mapping specification** (VALIDATED - CI passed)
2. **Milestone 2 — Conversion engine** (IN PROGRESS)
3. **Milestone 3 — Windows integration (TSF)** (SKELETON)
4. **Milestone 4 — Settings interface (Tauri)** (SKELETON)
5. **Milestone 5 — Installer**
6. **Milestone 6 — Testing and release**

## Current Focus
- Engine tests passed in CI - mapping validation successful
- Next: expand engine test coverage
- Then: implement full TSF service (Milestone 3)
- Then: implement full Tauri settings app (Milestone 4)

## Known Issues / Uncertainties
- Need to verify specific family distinctions (ה/ሐ/ኀ, ሰ/ሠ, ጸ/ፀ) with actual testing
- Labiovelar forms need validation
- TSF service needs full implementation
- Settings app needs full implementation

## Next Actions
1. Expand engine tests further based on CI results
2. Implement full TSF text service
3. Implement full Tauri settings app
4. Create installer
