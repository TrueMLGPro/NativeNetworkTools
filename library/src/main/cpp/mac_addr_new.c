//
// Created by revit on 21.05.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <net/if_arp.h>
#include <jni.h>
#include <android/log.h>
#include <sys/endian.h>
#include "ifaddrs.h"

JNIEXPORT jstring JNICALL Java_it_alessangiorgi_ipneigh30_ArpNDK_getMACAddrNewMethod(JNIEnv *env, jclass clazz) {
    struct ifaddrs *ifaddr, *ifa;
    char mac_addr[18] = {0};

    if (getifaddrs(&ifaddr) == -1) {
        __android_log_print(ANDROID_LOG_ERROR, "ArpNDK", "Failed to get interface addresses");
        return (*env)->NewStringUTF(env, "");
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL || ifa->ifa_addr->sa_family != AF_PACKET)
            continue;

        struct sockaddr_ll *s = (struct sockaddr_ll *)ifa->ifa_addr;

        if (strcmp(ifa->ifa_name, "wlan0") == 0 && s->sll_protocol == htons(ETH_P_ARP)) {
            unsigned char *mac = s->sll_addr;
            sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
                    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            break;
        }
    }

    freeifaddrs(ifaddr);

    return (*env)->NewStringUTF(env, mac_addr);
}