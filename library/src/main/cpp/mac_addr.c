//
// Created by TrueMLGPro on 16.05.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <linux/sockios.h>
#include <linux/if.h>
#include <linux/in.h>
#include <bits/ioctl.h>
#include <string.h>
#include <android/log.h>
#include <unistd.h>
#include <jni.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

JNIEXPORT jstring JNICALL Java_it_alessangiorgi_ipneigh30_ArpNDK_MACFromJNI(JNIEnv *env, jclass clazz) {
    int sockfd;
    struct ifreq ifr;
    char mac_addr[18] = {0};

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd >= 0) {
        memset(&ifr, 0, sizeof(struct ifreq));
        strncpy(ifr.ifr_name, "wlan0", IFNAMSIZ - 1);

        if (ioctl(sockfd, SIOCGIFINDEX, &ifr) >= 0) {
            int fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
            if (fd >= 0) {
                struct {
                    struct nlmsghdr nlh;
                    struct ifinfomsg ifi;
                } req;

                memset(&req, 0, sizeof(req));
                req.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct ifinfomsg));
                req.nlh.nlmsg_type = RTM_GETLINK;
                req.ifi.ifi_family = AF_UNSPEC;
                req.nlh.nlmsg_flags = NLM_F_REQUEST;
                req.ifi.ifi_index = ifr.ifr_ifindex;

                if (send(fd, &req, req.nlh.nlmsg_len, 0) >= 0) {
                    char buf[4096];
                    ssize_t len = recv(fd, buf, sizeof(buf), 0);
                    if (len > 0) {
                        struct nlmsghdr *nlh = (struct nlmsghdr*) buf;
                        while (NLMSG_OK(nlh, len)) {
                            if (nlh->nlmsg_type == RTM_NEWLINK) {
                                struct ifinfomsg *ifi = (struct ifinfomsg*) NLMSG_DATA(nlh);
                                struct rtattr *rta = IFLA_RTA(ifi);
                                int rtalen = IFLA_PAYLOAD(nlh);

                                while (RTA_OK(rta, rtalen)) {
                                    if (rta->rta_type == IFLA_ADDRESS) {
                                        unsigned char *mac = (unsigned char*) RTA_DATA(rta);
                                        sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
                                                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                                        close(fd);
                                        close(sockfd);
                                        return (*env)->NewStringUTF(env, mac_addr);
                                    }

                                    rta = RTA_NEXT(rta, rtalen);
                                }
                            }

                            nlh = NLMSG_NEXT(nlh, len);
                        }
                    }
                }

                close(fd);
            }

            __android_log_print(ANDROID_LOG_ERROR, "ArpNDK", "Failed to get MAC address: NETLINK_ROUTE");
            close(sockfd);
            return (*env)->NewStringUTF(env, "");
        } else {
            __android_log_print(ANDROID_LOG_ERROR, "ArpNDK", "Failed to get MAC address: ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0");
            close(sockfd);
            return (*env)->NewStringUTF(env, "");
        }
    } else {
        __android_log_print(ANDROID_LOG_ERROR, "ArpNDK", "Failed to create socket");
        return (*env)->NewStringUTF(env, "");
    }

    return (*env)->NewStringUTF(env, mac_addr);
}
