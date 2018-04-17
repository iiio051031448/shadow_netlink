#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>

#define S_LOG(fmt, ...) \
    printk("[%s][%d]" fmt "\n", __func__, __LINE__, ##__VA_ARGS__)

struct shadow_dev_pri {
    int id; 
};

int shadow_netdev_open(struct net_device *dev)
{
    return 0;
}

int  shadow_netdev_stop(struct net_device *dev)
{
    return 0;
}

netdev_tx_t shadow_netdev_start_xmit(struct sk_buff *skb,
                           struct net_device *dev)
{
    return NETDEV_TX_OK;
}

int shadow_netdev_ioctl(struct net_device *dev,
                            struct ifreq *ifr, int cmd)
{
    return 0;
}

static void shadow_et_drvinfo_get(struct net_device *pDev,
                               struct ethtool_drvinfo *pInfo)
{
    strcpy(pInfo->driver, "shadow net link");
    sprintf(pInfo->bus_info, "CSR 0x%08x", 0xDEADC0DE);
}

static struct ethtool_ops shadow_ethtool_ops = {
    .get_drvinfo = shadow_et_drvinfo_get,
};

static struct net_device_ops g_shadow_netdev_ops = {
    .ndo_open = shadow_netdev_open,
    .ndo_stop = shadow_netdev_stop,
    .ndo_start_xmit = shadow_netdev_start_xmit,
    .ndo_do_ioctl = shadow_netdev_ioctl,
};

static struct net_device *g_shadow_ndev = NULL;
int shadow_netdev_init(void)
{
    int ret;
    
    g_shadow_ndev = alloc_etherdev(sizeof(struct shadow_dev_pri));
    if (NULL == g_shadow_ndev) {
        S_LOG("alloc net_device failed.");
        return -1;
    }

    g_shadow_ndev->netdev_ops = &g_shadow_netdev_ops;
    g_shadow_ndev->ethtool_ops = &shadow_ethtool_ops,

    ret = register_netdev(g_shadow_ndev);
    if (0 != ret) {
        S_LOG("register_netdev failed.");
        goto error;
    }
    S_LOG("register_netdev SUCCESS.");

    return 0;

error:
    free_netdev(g_shadow_ndev);
    g_shadow_ndev = NULL;

    return -1;
}

void shadow_netdev_fini(void)
{
    if (NULL != g_shadow_ndev) {
        unregister_netdev(g_shadow_ndev);
        free_netdev(g_shadow_ndev);
        g_shadow_ndev = NULL;
    }

    return;
}

static int __init shadow_init(void)
{
    shadow_netdev_init();
    
    S_LOG("init success.");

    return 0;
}

static void __exit shadow_fini(void)
{
    shadow_netdev_fini();

    S_LOG("fini success.");

    return;
}

module_init(shadow_init);
module_exit(shadow_fini);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lyj051031448@163.com");
MODULE_DESCRIPTION("shadow link");
