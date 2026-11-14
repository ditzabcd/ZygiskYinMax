#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdarg.h>

static const char *DENY[] = {"/data/adb/ksu", "/data/adb/modules", "/system/bin/su", "/data/adb/magisk", NULL};

static int check_path(const char *path) {
    if (!path) return 0;
    for (int i = 0; DENY[i]; i++) {
        if (strstr(path, DENY[i])) return 1;
    }
    return 0;
}

static int (*orig_openat)(int, const char*, int, ...) = NULL;
static int (*orig_stat)(const char*, struct stat*) = NULL;
static int (*orig_access)(const char*, int) = NULL;

static int hook_openat(int dirfd, const char *pathname, int flags, ...) {
    if (check_path(pathname)) { errno = ENOENT; return -1; }
    if (!orig_openat) orig_openat = (int (*)(int, const char*, int, ...))dlsym(RTLD_NEXT, "openat");
    va_list ap; va_start(ap, flags);
    mode_t mode = va_arg(ap, mode_t);
    va_end(ap);
    return orig_openat(dirfd, pathname, flags, mode);
}

static int hook_stat(const char *path, struct stat *buf) {
    if (check_path(path)) { errno = ENOENT; return -1; }
    if (!orig_stat) orig_stat = (int (*)(const char*, struct stat*))dlsym(RTLD_NEXT, "stat");
    return orig_stat(path, buf);
}

static int hook_access(const char *path, int mode) {
    if (check_path(path)) { errno = ENOENT; return -1; }
    if (!orig_access) orig_access = (int (*)(const char*, int))dlsym(RTLD_NEXT, "access");
    return orig_access(path, mode);
}

__attribute__((constructor)) static void init() {
    orig_openat = (int (*)(int, const char*, int, ...))dlsym(RTLD_NEXT, "openat");
    orig_stat = (int (*)(const char*, struct stat*))dlsym(RTLD_NEXT, "stat");
    orig_access = (int (*)(const char*, int))dlsym(RTLD_NEXT, "access");
}