#!/usr/bin/env zsh

set -euo pipefail

# environment
current_dir="$(pwd)"
script_dir="$(cd "$(dirname "$0")" && pwd)"
thirdparty_dir="${current_dir}/_thirdparty"
relative_thirdparty_dir=".${thirdparty_dir#"$current_dir"}"

# vcpkg
vcpkg_dir="${thirdparty_dir}/vcpkg"
vcpkg_ports="${script_dir}/ports"
vcpkg_repo="https://github.com/microsoft/vcpkg.git"
if [ -n "${VCPKG_DIR:-}" ]; then
    echo "Using VCPKG_DIR from environment: $VCPKG_DIR"
    vcpkg_dir="$VCPKG_DIR"
fi

os="$(uname -s)"
arch="$(uname -m)"
vcpkg_triplet=""

if [ "$os" = "Darwin" ]; then
  case "$arch" in
    arm64)
      vcpkg_triplet="arm64-osx"
      ;;
    x86_64)
      vcpkg_triplet="x64-osx"
      ;;
  esac
else
  echo "Error: triplet detection only implemented for macOS." >&2
  exit 1
fi

# Print environment
echo Environment:
echo -----
echo current dir: "$current_dir"
echo install dir: "$thirdparty_dir"
echo OS: "$os"
echo Arch: "$arch"
echo vcpkg dir: "$vcpkg_dir"
echo vcpkg triplet: $vcpkg_triplet
echo -----
echo

# Do work
mkdir -p "$thirdparty_dir"

# Install vcpkg if necessary
if [ ! -d "${vcpkg_dir}/.git" ]; then
    echo "vcpkg not found. Cloning from $vcpkg_repo into $vcpkg_dir ..."
    git clone "$vcpkg_repo" "$vcpkg_dir"
fi
if [ ! -f "$vcpkg_dir/vcpkg" ]; then
    echo "vcpkg binary not found. Bootstrapping vcpkg..."
    pushd "$vcpkg_dir"
    sh "./bootstrap-vcpkg.sh"
    popd
fi

# Check for vcpkg manifest
if [ ! -f "./vcpkg.json" ]; then
  echo "Error: vcpkg.json manifest not found in ${current_dir}" >&2
  exit 1
fi

# Build thirdparty
install_dir=${thirdparty_dir}/.tmp/${arch}
to_dir="${thirdparty_dir}/${arch}"

vcpkg_args=(install)
if [ -n "$vcpkg_triplet" ]; then
  vcpkg_args+=(--triplet="${vcpkg_triplet}")
fi
vcpkg_args+=(--x-install-root="${install_dir}")
vcpkg_args+=(--overlay-ports="${vcpkg_ports}")

mkdir -p "${vcpkg_ports}"

"$vcpkg_dir/vcpkg" "${vcpkg_args[@]}"

rm -rf "${to_dir}"
mkdir -p "${to_dir}"
mv -- "${install_dir}/${vcpkg_triplet}" "${install_dir}/${arch}"
mv -- "${install_dir}/${arch}" "${thirdparty_dir}"
rm -rf "${install_dir}"

# Finish
echo "Add to your CMake config:"
echo "    list(APPEND CMAKE_PREFIX_PATH \"${relative_thirdparty_dir}/\${CMAKE_SYSTEM_PROCESSOR}\")"
