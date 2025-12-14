#!/usr/bin/env bash
set -u

# --- Configuration ---
DEST="Current"
TEMPLATE_SRC="dsa_lib/template.h"

# Defaults
MODE="CF"       # Options: CF (Numeric), ICPC (Alphabetic)
COUNT=7         # Default for CF is 7
COUNT_SET=false # Track if user manually set the count

# --- 1. Parse Arguments ---
while [[ "$#" -gt 0 ]]; do
    case $1 in
        -i|--i|-icpc|--icpc)
            MODE="ICPC"
            # Only change default count to 12 if user hasn't manually set a number yet
            if [ "$COUNT_SET" = false ]; then
                COUNT=12
            fi
            ;;
        -c|--c|-cf|--cf|-codeforces|--codeforces)
            MODE="CF"
            # Only change default count to 7 if user hasn't manually set a number yet
            if [ "$COUNT_SET" = false ]; then
                COUNT=7
            fi
            ;;
        -n|--n|-number|--number)
            # Ensure a value was actually provided
            if [[ -z "${2:-}" ]]; then
                echo "Error: Argument -n requires a number or letter."
                exit 1
            fi

            VAL="$2"

            # Check if integer
            if [[ "$VAL" =~ ^[0-9]+$ ]]; then
                COUNT="$VAL"
            # Check if single letter (a-z or A-Z)
            elif [[ "$VAL" =~ ^[a-zA-Z]$ ]]; then
                # Convert to Uppercase
                UPPER_CHAR=$(echo "$VAL" | tr '[:lower:]' '[:upper:]')
                # Get ASCII value
                ASCII_VAL=$(printf "%d" "'$UPPER_CHAR")
                # Subtract 64 (ASCII 'A' is 65, we want 1)
                COUNT=$((ASCII_VAL - 64))
            else
                echo "Error: Invalid value for -n. Please use a number (e.g., 7) or a letter (e.g., G)."
                exit 1
            fi

            COUNT_SET=true
            shift # Skip the value argument
            ;;
        -h|--help)
            echo "Usage: ./reset.sh [flags]"
            echo "  (no flags)     : Codeforces mode (1.cpp to 7.cpp)"
            echo "  -c, --cf       : Explicit Codeforces mode"
            echo "  -i, --icpc     : ICPC mode (A.cpp to L.cpp)"
            echo "  -n, --number X : Generate X files."
            echo "                   X can be a number (7) or a letter (G -> 7)."
            exit 0
            ;;
        *)
            echo "Unknown parameter passed: $1"
            exit 1
            ;;
    esac
    shift
done

echo "--- Starting Reset ($MODE Mode | $COUNT Files) ---"

# --- 2. Check Template ---
if [ ! -f "$TEMPLATE_SRC" ]; then
    echo "ERROR: Template file not found at: $TEMPLATE_SRC"
    echo "Current directory is: $(pwd)"
    exit 1
fi

# --- 3. Setup Directory ---
echo "Recreating $DEST folder..."
if [ -d "$DEST" ]; then
    rm -rf "$DEST"
fi
mkdir -p "$DEST"

# --- 4. Create I/O Files ---
touch "$DEST/input.txt" "$DEST/output.txt"

# --- 5. Define Main Logic ---
define_body() {
    cat <<EOM
void solve(){
    
}



signed main(){
    fast;
    int t = 1;
    cin >> t;
    while(t--){
        solve();
    }
    return 0;
}
EOM
}

# --- 6. Generate Solution Files ---
echo "Generating files..."

# Array of alphabet for ICPC mode
ALPHABET=({A..Z})

for (( i=0; i<COUNT; i++ )); do
    # Determine Filename based on Mode
    if [ "$MODE" == "ICPC" ]; then
        # Check if we exceeded A-Z
        if [ $i -ge 26 ]; then
            echo "Error: Cannot generate more than 26 files in ICPC mode."
            exit 1
        fi
        NAME="${ALPHABET[$i]}"
    else
        # Codeforces mode uses 1-based indexing
        NAME="$((i+1))"
    fi

    FILE="$DEST/$NAME.cpp"
    
    # Step A: Filter out include guards
    grep -vE "^#ifndef CP_TEMPLATE_H|^#define CP_TEMPLATE_H|^#endif" "$TEMPLATE_SRC" > "$FILE"
    
    # Step B: Add exactly 10 blank lines
    for (( k=0; k<10; k++ )); do
        echo "" >> "$FILE"
    done
    
    # Step C: Append the solve/main logic
    define_body >> "$FILE"
    
    echo "  -> Created $NAME.cpp"
done

echo "--- Reset Complete ---"