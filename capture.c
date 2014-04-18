#include <sys/ioctl.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if.h>
#include <stdlib.h>
#include <assert.h>

#ifdef ANDROID
#include <android/log.h>
#define TAG "capture"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define LOG(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)

#else
#define LOG(...) fprintf(stdout, __VA_ARGS__)
#define LOGE(...) fprintf(stderr, __VA_ARGS__)
#endif

#ifndef MSG_TRUNC
#define MSG_TRUNC   0x20
#endif

int openSocket() {
    int sockFd;
    sockFd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    return sockFd;
}

int bind2dev(int fd, const char *dev) {
    struct ifreq ifr;   
    struct sockaddr_ll sll;
    int err;
    socklen_t   errlen = sizeof(err);
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, dev, sizeof(ifr.ifr_name));
    if (ioctl(fd, SIOCGIFINDEX, &ifr) == -1) {
        LOGE("bind2dev ioctl error:%s\r\n", strerror(errno));
        return -2;
    }

    memset(&sll, 0, sizeof(sll));
    sll.sll_family  = AF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol= htons(ETH_P_ALL);

    if (bind(fd, (struct sockaddr *)&sll, sizeof(sll)) == -1) {
        if (errno == ENETDOWN) {
            LOGE("interface '%s' is not up.\r\n", dev);
            return -3;
        } else {
            LOGE("bind '%s' error: %s, not bind\r\n",dev, strerror(errno));
            return -4;
        }
    }
    if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &errlen) == -1) {
        LOGE("getsockopt '%s' error: %s\r\n", dev, strerror(errno));
        return -5;
    }
    if (err == ENETDOWN) {
        LOGE("interface '%s' is not up.\r\n", dev);
        return -3;
    } else if (err > 0){
        LOGE("bind '%s' error: %s\r\n", dev,  strerror(err));
        return -4;
    }
    return 0;
}

int readCap(int fd, char *pBuf, int size) {
    struct sockaddr_ll from;
    int fromlen = sizeof(from);
    return recvfrom(fd, pBuf, size, MSG_TRUNC, (struct sockaddr*)&from, &fromlen);
}

void dumpInt(int n) {
    int i;
    for(i=0; i<4; i++) {
        LOG("%02X ", (n>> (i*8)) & 0xff);
    }
}

static char *spBuf;
static int soff;
static int slen;

static int hookBuf(char *p, int len) {
    assert(len > 0);
    spBuf = (char *)malloc(len);
    if (spBuf == NULL) {
        return -1;
    }
    memcpy(spBuf, p, len);
    slen = len;
    soff = 0;
    return 0;
}

static void off(int u) {
    soff += u;
}
static void setOff(int o) {
    soff = o;
}

typedef U8 unsigned char;
typedef U16 unsigned short;
typedef U32 unsigned int;
typedef U64 unsigned long long;

#define getType(type) \
    static type get#type() { \
        type ret = *((type *)spBuf); \
        soff += sizeof(type); \
        return ret; \
    }

getType(U8)
getType(U16)
getType(U32)
getType(U64)

void parseCap() {
    parseLinkLayer();
}

int main(int argc, char *argv[]) {

    int sockFd;
    int ret = 0;
    char *pBuf;
    sockFd = openSocket();
    if (sockFd < 0) {
        ret = -1;
        goto bad;
    }
    
    int n;
    if ( (n = bind2dev(sockFd, "eth0")) != 0) {
        ret = n;
        goto bad;
    }

    pBuf = (char *)malloc(1<<20);
    int cnt = 0;
    while (cnt < 100) {
        int len = readCap(sockFd, pBuf, 1<<20);

        LOG("read len %d\r\n", len);

        if (hookBuf(pBuf, len) != 0) {
            LOGE("hookBuf failed.\r\n");
            goto bad;
        }

        int *pI = (int *)pBuf;
        int i = 0;
        while ((char *)pI < pBuf + len) {
            if (i % 8 == 0) LOG("\r\n%08X: ", ((char *)pI) - pBuf);
            dumpInt(*pI);
            pI++;
            i++;
        }
        parseCap();
        LOG("\r\n");
        cnt++;
        unHookBuf();
    }

bad:
    if (sockFd >= 0) close(sockFd);
    if (pBuf != NULL) free(pBuf);
    return ret;
}
