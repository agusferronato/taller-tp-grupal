#!/usr/bin/env bash
set -euo pipefail

APP_NAME="argentum"

PROJECT_ROOT="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd -P)"
BUILD_DIR="${PROJECT_ROOT}/build"

LOCAL_BIN_DIR="${HOME}/.local/bin"
SHARE_DIR="${HOME}/.local/share/${APP_NAME}"
SHARE_BIN_DIR="${SHARE_DIR}/bin"
CONFIG_DIR="${HOME}/.config/${APP_NAME}"

if [[ "$(id -u)" -eq 0 ]]; then
  SUDO=""
else
  SUDO="sudo"
fi

packages=(
  build-essential
  cmake
  ninja-build
  pkg-config

  qt6-base-dev

  libsdl2-dev
  libsdl2-image-dev
  libsdl2-ttf-dev
  libsdl2-mixer-dev

  libtomlplusplus-dev
  libgtest-dev
)

warn() {
  echo "Warning: $*" >&2
}

copy_dir_if_exists() {
  local src="$1"
  local dst="$2"

  if [[ ! -d "$src" ]]; then
    warn "directory not found: $src"
    return
  fi

  mkdir -p "$dst"
  cp -a "${src}/." "$dst/"
}

copy_file_if_exists() {
  local src="$1"
  local dst="$2"

  if [[ ! -f "$src" ]]; then
    warn "file not found: $src"
    return
  fi

  mkdir -p "$(dirname -- "$dst")"
  cp -a "$src" "$dst"
}

copy_tomls_if_exist() {
  local src_dir="$1"
  local dst_dir="$2"

  if [[ ! -d "$src_dir" ]]; then
    warn "config directory not found: $src_dir"
    return
  fi

  shopt -s nullglob
  local files=("${src_dir}"/*.toml)
  shopt -u nullglob

  if [[ "${#files[@]}" -eq 0 ]]; then
    warn "no .toml files found in: $src_dir"
    return
  fi

  mkdir -p "$dst_dir"
  cp -a "${files[@]}" "$dst_dir/"
}

find_executable() {
  local name="$1"
  local path

  path="$(find "$BUILD_DIR" -type f -name "$name" -perm -111 -print -quit)"
  if [[ -z "$path" ]]; then
    echo "Could not find executable: $name" >&2
    return 1
  fi

  printf '%s\n' "$path"
}

install_wrapper() {
  local executable="$1"
  local wrapper="${LOCAL_BIN_DIR}/${executable}"

  cat > "$wrapper" <<EOF
#!/usr/bin/env bash
set -euo pipefail

ARGENTUM_DATA_DIR="\${ARGENTUM_DATA_DIR:-\$HOME/.local/share/${APP_NAME}}"
ARGENTUM_CONFIG_DIR="\${ARGENTUM_CONFIG_DIR:-\$HOME/.config/${APP_NAME}}"
export ARGENTUM_DATA_DIR ARGENTUM_CONFIG_DIR

cd "\$ARGENTUM_DATA_DIR"
exec "\$ARGENTUM_DATA_DIR/bin/${executable}" "\$@"
EOF

  chmod 0755 "$wrapper"
}

echo "Installing dependencies..."
$SUDO apt-get update
$SUDO apt-get install -y --no-install-recommends "${packages[@]}"

echo "Configuring project..."
cmake -S "$PROJECT_ROOT" -B "$BUILD_DIR" -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DTALLER_CLIENT=ON \
  -DTALLER_SERVER=ON \
  -DTALLER_EDITOR=ON \
  -DTALLER_TESTS=ON

echo "Building project..."
cmake --build "$BUILD_DIR"

echo "Running tests..."
ctest --test-dir "$BUILD_DIR" --output-on-failure

echo "Installing binaries..."
mkdir -p "$LOCAL_BIN_DIR" "$SHARE_BIN_DIR"

for executable in taller_client taller_server taller_editor; do
  built_executable="$(find_executable "$executable")"
  install -m 0755 "$built_executable" "${SHARE_BIN_DIR}/${executable}"
  install_wrapper "$executable"
done

echo "Installing assets..."
mkdir -p "$SHARE_DIR"
copy_dir_if_exists "${PROJECT_ROOT}/client/assets" "${SHARE_DIR}/assets"
copy_dir_if_exists "${PROJECT_ROOT}/editor/assets" "${SHARE_DIR}/assets"
copy_dir_if_exists "${PROJECT_ROOT}/client/fonts" "${SHARE_DIR}/fonts"
copy_dir_if_exists "${PROJECT_ROOT}/editor/assets" "${SHARE_DIR}/editor/assets"
copy_dir_if_exists "${PROJECT_ROOT}/gif" "${SHARE_DIR}/gif"

echo "Installing maps..."
copy_file_if_exists "${PROJECT_ROOT}/map.toml" "${SHARE_DIR}/maps/map.toml"
copy_file_if_exists "${PROJECT_ROOT}/mapa_de_prueba.toml" "${SHARE_DIR}/maps/mapa_de_prueba.toml"
copy_file_if_exists "${PROJECT_ROOT}/mapa_final.toml" "${SHARE_DIR}/maps/mapa_final.toml"
copy_file_if_exists "${PROJECT_ROOT}/editor/map.toml" "${SHARE_DIR}/maps/editor_map.toml"

echo "Installing configs..."
mkdir -p "$CONFIG_DIR"
copy_tomls_if_exist "${PROJECT_ROOT}/common" "$CONFIG_DIR"
copy_tomls_if_exist "${PROJECT_ROOT}/client/assets" "${CONFIG_DIR}/client"
copy_tomls_if_exist "${PROJECT_ROOT}/editor/assets" "${CONFIG_DIR}/editor"

# Compatibilidad con binarios que todavía buscan configs relativas al cwd.
copy_tomls_if_exist "${PROJECT_ROOT}/common" "$SHARE_DIR"
copy_file_if_exists "${PROJECT_ROOT}/map.toml" "${SHARE_DIR}/map.toml"

cat <<EOF

Install complete.

Commands:
  taller_server 8080 maps/map.toml
  taller_client localhost 8080
  taller_editor maps/map.toml

Installed in:
  binaries: ${SHARE_BIN_DIR}
  wrappers: ${LOCAL_BIN_DIR}
  assets:   ${SHARE_DIR}
  configs:  ${CONFIG_DIR}

EOF

if [[ ":${PATH}:" != *":${LOCAL_BIN_DIR}:"* ]]; then
  cat <<EOF
${LOCAL_BIN_DIR} is not in PATH.
Add it with:
  echo 'export PATH="\$HOME/.local/bin:\$PATH"' >> "\$HOME/.bashrc"
  source "\$HOME/.bashrc"

EOF
fi
