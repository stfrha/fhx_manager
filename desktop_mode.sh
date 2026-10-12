#!/bin/bash
#
# desktop_mode.sh - switch the desktop between black (used while the theatre
# videos are shown on the receiver) and normal.
#
# Usage:
#   ./desktop_mode.sh black     black background, no icons, auto-hiding panel
#   ./desktop_mode.sh normal    restore the previous desktop settings
#
# The user's own settings are saved as *.fhx_saved while black is active.

DESK="$HOME/.config/pcmanfm/default/desktop-items-0.conf"
PANEL="$HOME/.config/wf-panel-pi/wf-panel-pi.ini"
MARK="# fhx_manager black desktop"

is_black() {
   grep -q "^$MARK" "$1" 2>/dev/null
}

save() {
   # Keep the user's file, unless it is already our black version
   if [[ -e "$1" ]] && ! is_black "$1"; then
      mv "$1" "$1.fhx_saved"
   fi
}

restore() {
   if is_black "$1"; then
      rm -f "$1"
   fi
   if [[ -e "$1.fhx_saved" ]]; then
      mv "$1.fhx_saved" "$1"
   fi
}

case "${1:-}" in
   black)
      mkdir -p "$(dirname "$DESK")" "$(dirname "$PANEL")"
      save "$DESK"
      save "$PANEL"
      printf '%s\n[*]\nwallpaper_mode=color\ndesktop_bg=#000000000000\ndesktop_shadow=#000000000000\nshow_documents=0\nshow_trash=0\nshow_mounts=0\n' "$MARK" > "$DESK"
      printf '%s\n[panel]\nautohide=true\nautohide_duration=500\nnotify_enable=false\n' "$MARK" > "$PANEL"
      ;;
   normal)
      # Nothing to do if black is not active
      if ! is_black "$DESK" && ! is_black "$PANEL"; then
         exit 0
      fi
      restore "$DESK"
      restore "$PANEL"
      ;;
   *)
      echo "Usage: $0 black|normal"
      exit 1
      ;;
esac

# Both are restarted automatically (lwrespawn) and read the new settings
pkill -x pcmanfm
pkill -x wf-panel-pi
exit 0
