#! /bin/bash

set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
pushd "$SCRIPT_DIR" >/dev/null

mkdir -p build
pushd build >/dev/null

echo "Downloading configure.pyz..."
rm -f configure.pyz
wget https://github.com/nsubiron/configure-pyz/releases/download/v0.2.1/configure.pyz
chmod u+x configure.pyz
./configure.pyz --version

# ==============================================================================
# -- Get crypto ----------------------------------------------------------------
# ==============================================================================

mkdir -p crypto
pushd crypto >/dev/null

# Get crypto source
if [[ ! -d "source" ]]; then
  echo "Retrieving crypto..."
  git clone --depth=1 https://github.com/nsubiron/crypto.git source
else
  echo "Folder crypto/source already exists, skipping git clone..."
fi

mkdir -p build
pushd build >/dev/null

cmake -DCMAKE_CXX_COMPILER=clang++ \
      -DCMAKE_CXX_FLAGS="-std=c++1z -Wall -Wextra" \
      -DCMAKE_INSTALL_PREFIX=../install \
      -G "Ninja" \
      ../source
ninja install

popd >/dev/null
popd >/dev/null

# ==============================================================================
# -- ...and we are done --------------------------------------------------------
# ==============================================================================

popd >/dev/null
popd >/dev/null

set +x
echo ""
echo "Success!"
