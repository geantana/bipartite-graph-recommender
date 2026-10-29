#!/usr/bin/env bash
# ==============================================================================
# Script to download the official MovieLens 100K dataset from GroupLens Research.
# License & Terms of Use: https://grouplens.org/datasets/movielens/
# Note: In accordance with GroupLens terms, the raw data is not bundled in the repo.
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"
DATA_DIR="${ROOT_DIR}/data"
TARGET_FILE="${DATA_DIR}/interactions.txt"
DATASET_URL="https://files.grouplens.org/datasets/movielens/ml-100k.zip"
TMP_ZIP="/tmp/ml-100k-$$.zip"

mkdir -p "${DATA_DIR}"

if [ -f "${TARGET_FILE}" ]; then
    echo "==> Dataset already exists at: ${TARGET_FILE}"
    exit 0
fi

echo "========================================================================"
echo " Downloading MovieLens 100K dataset from GroupLens Research..."
echo " URL: ${DATASET_URL}"
echo "========================================================================"

if command -v curl >/dev/null 2>&1; then
    curl -fSL "${DATASET_URL}" -o "${TMP_ZIP}"
elif command -v wget >/dev/null 2>&1; then
    wget -qO "${TMP_ZIP}" "${DATASET_URL}"
else
    echo "Error: Neither curl nor wget was found on the system." >&2
    exit 1
fi

echo "==> Extracting u.data to ${TARGET_FILE}..."
unzip -p "${TMP_ZIP}" ml-100k/u.data > "${TARGET_FILE}"
rm -f "${TMP_ZIP}"

LINE_COUNT=$(wc -l < "${TARGET_FILE}" | tr -d ' ')
echo "==> Successfully downloaded and extracted ${LINE_COUNT} interactions."
echo "==> Dataset ready at: ${TARGET_FILE}"
