#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>

#define __packed            __attribute__((packed))

#define S_CFG_LOG(fmt, ...) \
    printf("[%s][%d]" fmt "\n", __func__, __LINE__, ##__VA_ARGS__)

#define FMT_DEV_MAC "%.2x:%.2x:%.2x:%.2x:%.2x:%.2x"//"%02X:%02X:%02X:%02X:%02X:%02X"
#define PRINT_DEV_MC(m) (m)[0], (m)[1], (m)[2], (m)[3], (m)[4], (m)[5]

#if 0
struct iphdr {
    u8 ver_len;
    u8 dscp_ecn;
    u16 total_length;
    u16 ident;
    u16 frag_off_flags;
    u8 ttl;
    u8 proto;
    u16 checksum;
    u32 src;
    u32 dest;
} __packed;
#endif

void _dump_data(unsigned char *ptr, int len)
{
    int i;

    for (i = 0; i < len; i++) {
        if (!(i%16))
            printf("\n %04x", i);
        printf(" %02x", ptr[i]);
    }
    printf("\n");

    return;
}

#define DUMP_DATA(p, l) \
    _dump_data((unsigned char *)p, l)

int raw_sock(void)
{
    int fd = -1;
    int n = -1;
    char buf[2048];

    struct ethhdr *eth_hdr = NULL;

    fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_IP));
    if (fd < 0) {
        S_CFG_LOG("create sock error");
        return -1;
    }

    while (1) {
        memset(buf, 0, sizeof(buf));
        n = recv(fd, buf, sizeof(buf), 0);
        if (n < 0) {
            S_CFG_LOG("recv error.");
            break;
        }

        //DUMP_DATA(buf, n);
        eth_hdr = (struct ethhdr*)buf;
        S_CFG_LOG("from "FMT_DEV_MAC" to "FMT_DEV_MAC", eth proto %d", 
                PRINT_DEV_MC(eth_hdr->h_source), PRINT_DEV_MC(eth_hdr->h_dest), ntohs(eth_hdr->h_proto));
    }

    close(fd);
    fd = -1;

    return 0;
}

int main(void)
{
    raw_sock();

    return 0;
}
