#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/user.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <elf.h>

#define SOCK_PATH "/data/adb/zygiskyin.sock"
#define LIB_PATH  "/data/adb/modules/zygiskyin/libzygisk.so"
#define STATE_PATH "/data/adb/modules/zygiskyin/state.json"

static pid_t find_zygote() {
    DIR *d = opendir("/proc");
    if (!d) return -1;
    struct dirent *e;
    pid_t ret = -1;
    while ((e = readdir(d))) {
        if (e->d_name[0] < '0' || e->d_name[0] > '9') continue;
        char path[64], buf[64] = {0};
        snprintf(path, sizeof(path), "/proc/%s/cmdline", e->d_name);
        int fd = open(path, O_RDONLY);
        if (fd < 0) continue;
        read(fd, buf, sizeof(buf) - 1);
        close(fd);
        if (strncmp(buf, "zygote64", 8) == 0) {
            ret = (pid_t)atoi(e->d_name);
            break;
        }
    }
    closedir(d);
    return ret;
}

static int inject_library(pid_t pid) {
    if (ptrace(PTRACE_ATTACH, pid, NULL, NULL) != 0) return -1;
    int status;
    waitpid(pid, &status, 0);
    if (!WIFSTOPPED(status)) return -1;

    struct user_pt_regs regs;
    struct iovec iov;
    iov.iov_base = &regs;
    iov.iov_len = sizeof(regs);
    
    ptrace(PTRACE_GETREGSET, pid, (void*)NT_PRSTATUS, &iov);

    for (int i = 0; i < (int)strlen(LIB_PATH); i += 8) {
        long word = 0;
        memcpy(&word, (const char*)LIB_PATH + i, 8);
        ptrace(PTRACE_POKEDATA, pid, (void*)(regs.sp - 0x1000 + i), (void*)word);
    }
    
    ptrace(PTRACE_DETACH, pid, NULL, NULL);
    return 0;
}

int main() {
    pid_t zygote = find_zygote();
    if (zygote > 0) inject_library(zygote);
    
    int fd = open(STATE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        const char *json = "{\"zygote\":1,\"bit\":64,\"modules\":[\"PIFork\"]}";
        write(fd, json, strlen(json));
        close(fd);
    }
    return 0;
}