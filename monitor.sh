#!/usr/bin/env bash
set -euo pipefail

PORT="/dev/ttyACM0"
BAUD=115200

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; BOLD='\033[1m'; DIM='\033[2m'; NC='\033[0m'

section() { echo -e "\n${CYAN}${BOLD}▶ $1${NC}"; }
ok()      { echo -e "${GREEN}✓${NC} $1"; }
warn()    { echo -e "${YELLOW}⚠${NC} $1"; }

section "Preflight"

# Check minicom
if ! command -v minicom >/dev/null 2>&1; then
    echo -e "${RED}ERROR: minicom not found. Run: sudo apt install minicom${NC}"; exit 1
fi
ok "minicom found"

# Check port exists
if [ ! -e "$PORT" ]; then
    echo -e "${RED}ERROR: $PORT not found. Is the STM32 plugged in?${NC}"
    echo -e "${DIM}  Check available ports: ls /dev/ttyUSB*${NC}"
    exit 1
fi
ok "Port found: $PORT"

# Check permission
if [ ! -r "$PORT" ] || [ ! -w "$PORT" ]; then
    warn "No permission on $PORT — fixing..."
    sudo chmod 666 "$PORT"
    ok "Permission granted"
    echo -e "${DIM}  Tip: add yourself to dialout group to make this permanent:${NC}"
    echo -e "${DIM}  sudo usermod -aG dialout $USER  (then log out/in)${NC}"
fi

section "Connecting to STM32"
ok "Port:  $PORT"
ok "Baud:  $BAUD"
echo -e "  ${DIM}Press Ctrl+A then X to exit minicom${NC}\n"
sleep 1

minicom -b "$BAUD" -o -D "$PORT"

echo -e "\n${GREEN}✓${NC} Session ended.\n"
