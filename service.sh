#!/system/bin/sh
MODDIR=${0%/*}
until [ "$(getprop sys.boot_completed)" = "1" ]; do sleep 1; done

if ! pidof zygiskd > /dev/null; then
    chmod 755 $MODDIR/zygiskd
    $MODDIR/zygiskd
fi

STATE_FILE="$MODDIR/state.json"
PROP_FILE="$MODDIR/module.prop"

if [ -f "$STATE_FILE" ]; then
    DESC="[✅ Running] [64-bit] [Modules: PIFork]"
    sed -i "s/^description=.*/description=$DESC/" "$PROP_FILE"
fi