#!/usr/bin/env bash
set -euo pipefail

if [[ "$(id -u)" -eq 0 ]]; then
  SUDO=""
else
  SUDO="sudo"
fi

if [[ -r /etc/os-release ]]; then
  # shellcheck disable=SC1091
  . /etc/os-release
  if [[ "${ID:-}" != "ubuntu" || "${VERSION_ID:-}" != "24.04" ]]; then
    echo "Warning: this script is intended for Ubuntu 24.04." >&2
  fi
fi

packages=(
  # C++ build toolchain and CMake FetchContent downloads.
  build-essential
  ca-certificates
  cmake
  git
  ninja-build
  pkg-config
  unzip

  # Qt widgets used by client/editor: find_package(Qt6 COMPONENTS Core Gui Widgets).
  qt6-base-dev

  # SDL2 native Linux backends used while building SDL from FetchContent.
  libasound2-dev
  libdbus-1-dev
  libegl1-mesa-dev
  libgl1-mesa-dev
  libibus-1.0-dev
  libpulse-dev
  libudev-dev
  libwayland-dev
  libx11-dev
  libxcursor-dev
  libxext-dev
  libxfixes-dev
  libxi-dev
  libxinerama-dev
  libxkbcommon-dev
  libxrandr-dev
  libxss-dev
  libxxf86vm-dev
  wayland-protocols

  # SDL2_image codecs.
  libjpeg-dev
  libpng-dev
  libtiff-dev
  libwebp-dev

  # SDL2_ttf font stack.
  libfreetype-dev
  libharfbuzz-dev

  # SDL development headers used directly by project includes.
  libsdl2-dev
  libsdl2-image-dev
  libsdl2-ttf-dev
  libsdl2-mixer-dev

  # SDL2_mixer codecs/backends used by the current CMake comment and mixer build.
  fluidsynth
  libfluidsynth-dev
  libopus-dev
  libopusfile-dev
  libwavpack-dev
  libxmp-dev
  wavpack
)

echo "Installing build dependencies for taller_client and taller_server..."
$SUDO apt-get update
$SUDO apt-get install -y --no-install-recommends "${packages[@]}"

cat <<'EOF'

Dependencies installed.

Suggested build:
  cmake -S . -B build -G Ninja
  cmake --build build --target taller_server taller_client

Notes:
  - SDL2, SDL2_image, SDL2_mixer, SDL2_ttf, SDL2pp, toml++ and googletest are
    fetched by the current CMake configuration.
  - This script installs only the Ubuntu packages needed to configure and build
    those targets locally.
EOF
