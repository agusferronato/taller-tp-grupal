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
  # C++ build toolchain and helpers used by this installer.
  build-essential
  ca-certificates
  cmake
  git
  ninja-build
  pkg-config
  unzip

  # Qt widgets used by client/editor: find_package(Qt6 COMPONENTS Core Gui Widgets).
  qt6-base-dev

  # Native Linux backends used by SDL2 and SDL2pp.
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

  # SDL development packages resolved by CMake with find_package().
  libsdl2-dev
  libsdl2-image-dev
  libsdl2-ttf-dev
  libsdl2-mixer-dev

  # C++ libraries resolved with find_package() from CMake.
  libtomlplusplus-dev
  libgtest-dev

  # SDL2_mixer codec/runtime support used by the client audio stack.
  fluidsynth
  libfluidsynth-dev
  libopus-dev
  libopusfile-dev
  libwavpack-dev
  libxmp-dev
  wavpack
)

SDL2PP_REPO="https://github.com/libSDL2pp/libSDL2pp.git"
SDL2PP_REV="cc198c9a5657048bee67ece82de620b2d5661084"
DEPS_DIR="${TMPDIR:-/tmp}/argentum-deps"
SDL2PP_SRC="${DEPS_DIR}/libSDL2pp"
SDL2PP_BUILD="${SDL2PP_SRC}/build"

echo "Installing build dependencies for taller_client, taller_editor and taller_server..."
$SUDO apt-get update
$SUDO apt-get install -y --no-install-recommends "${packages[@]}"

mkdir -p "$DEPS_DIR"

if [[ -d "${SDL2PP_SRC}/.git" ]]; then
  echo "Updating libSDL2pp source..."
  git -C "$SDL2PP_SRC" fetch --tags --prune origin
else
  echo "Cloning libSDL2pp source..."
  git clone "$SDL2PP_REPO" "$SDL2PP_SRC"
fi

git -C "$SDL2PP_SRC" checkout "$SDL2PP_REV"

cmake -S "$SDL2PP_SRC" -B "$SDL2PP_BUILD" -G Ninja \
  -DSDL2PP_WITH_IMAGE=ON \
  -DSDL2PP_WITH_TTF=ON \
  -DSDL2PP_WITH_MIXER=ON \
  -DSDL2PP_STATIC=OFF \
  -DCMAKE_BUILD_TYPE=Release
cmake --build "$SDL2PP_BUILD"
$SUDO cmake --install "$SDL2PP_BUILD"

cat <<'EOF'

Dependencies installed.

EOF
