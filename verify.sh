#!/system/bin/sh
if [ -f "$MODPATH/sha256.txt" ]; then
    if ! sha256sum -c "$MODPATH/sha256.txt" > /dev/null 2>&1; then
        abort "! Code modified. Rejecting."
    fi
fi