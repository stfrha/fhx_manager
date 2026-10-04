#!/usr/bin/env bash
#
# deploy_php.sh - copy fhx_manager's web interface (the repo's php/ folder)
# to Apache's web root.
#
# Put this script in the root of the fhx_manager repo (next to the php/ folder).
#
# Usage:
#   ./deploy_php.sh           deploy
#   ./deploy_php.sh -n        dry run: show what would change, change nothing
#
# NOTE: the target folder is made an exact mirror of php/. Anything in the
# target that isn't in the repo is deleted (including Apache's default
# index.html, which would otherwise be shown instead of index.php).
# Files the web app creates itself at runtime can be protected by adding
# them to EXCLUDES below - excluded files are never deleted or overwritten.
 
set -euo pipefail
 
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SRC="${SCRIPT_DIR}/php/"
DEST="${DEST:-/var/www/html}"   # override with: DEST=/some/path ./deploy_php.sh
 
# Paths (relative to the web root) to leave untouched in the target.
EXCLUDES=(
  ".git"
  # "data/"
  # "settings.json"
)
 
DRY_RUN=""
case "${1:-}" in
  -n|--dry-run) DRY_RUN="--dry-run"; echo ">>> Dry run - nothing will be changed" ;;
  "") ;;
  *) echo "Usage: $0 [-n|--dry-run]"; exit 1 ;;
esac
 
if [[ ! -d "$SRC" ]]; then
  echo "Error: source folder not found: $SRC" >&2
  echo "Put this script in the repo root, next to the php/ folder." >&2
  exit 1
fi
 
if ! command -v rsync >/dev/null 2>&1; then
  echo "rsync not found, installing..."
  sudo apt-get install -y rsync
fi
 
EXCLUDE_ARGS=()
for e in "${EXCLUDES[@]}"; do
  EXCLUDE_ARGS+=(--exclude "$e")
done
 
sudo mkdir -p "$DEST"
 
echo ">>> Deploying $SRC -> $DEST"
# -a        keep structure and timestamps
# --delete  remove files in DEST that no longer exist in the repo
# --chown   files owned by Apache's user
# --chmod   dirs 755, files readable by all, writable only by owner
#           (executable bits on scripts are kept)
sudo rsync -a --delete $DRY_RUN -i \
  "${EXCLUDE_ARGS[@]}" \
  --chown=www-data:www-data \
  --chmod=D755,Fgo-w,Fa+r \
  "$SRC" "$DEST/"
 
if [[ -z "$DRY_RUN" ]]; then
  IP="$(hostname -I | awk '{print $1}')"
  echo ">>> Done. Open http://${IP}/"
fi
 