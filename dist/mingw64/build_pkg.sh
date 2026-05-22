#!/usr/bin/env bash
set -euo pipefail

VERSION="${1:?Usage: build_pkg.sh <version>}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
BUILD_DIR="${SCRIPT_DIR}/build"

rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

echo "Creating source tarball for version ${VERSION}..."
git -C "${REPO_ROOT}" archive \
    --prefix="libdedx-${VERSION}/" \
    --format=tar.gz \
    HEAD \
    -o "${BUILD_DIR}/libdedx-${VERSION}.tar.gz"

echo "Preparing PKGBUILD..."
sed "s/^pkgver=.*/pkgver=${VERSION}/" "${SCRIPT_DIR}/PKGBUILD" > "${BUILD_DIR}/PKGBUILD"

echo "Building UCRT64 package..."
cd "${BUILD_DIR}"
MINGW_ARCH=ucrt64 makepkg-mingw --noconfirm --noprogressbar

echo "Done. Package built in ${BUILD_DIR}"
