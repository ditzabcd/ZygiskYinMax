#!/system/bin/sh
ui_print "- Installing ZygiskYin"
. $MODPATH/verify.sh
set_perm_recursive $MODPATH 0 0 0755 0644
set_perm $MODPATH/zygiskd 0 0 0755
set_perm $MODPATH/post-fs-data.sh 0 0 0755
set_perm $MODPATH/service.sh 0 0 0755