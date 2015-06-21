#include <linux/ip.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/udp.h>
#include <linux/netfilter/x_tables.h>
#include <net/ip.h>
#include <net/ip6_checksum.h>
#include <net/ip6_route.h>
#include <net/route.h>
#include "compat_xtables.h"
#include "xt_SETPROTO.h"

static unsigned int
setproto_tg4(struct sk_buff *skb, const struct xt_action_param *par)
{
	union {
		struct iphdr *iph;
		__be16 *iphraw;
	} u;
	const struct xt_setproto_info *info;
	__be16 oldval, newval;
	if (!skb_make_writable(skb, sizeof(struct iphdr))) {
		return -1;
	}
	u.iph = ip_hdr(skb);
	info = par->targinfo;
	oldval = u.iphraw[4];
	u.iph->protocol = info->proto;
	newval = u.iphraw[4];
	csum_replace2(&u.iph->check, oldval, newval);
	return XT_CONTINUE;
}

static struct xt_target setproto_tg_reg[] __read_mostly = {
	{
		.name       = "SETPROTO",
		.revision   = 0,
		.family     = NFPROTO_IPV4,
		.table      = "raw",
		.target     = setproto_tg4,
		.targetsize = sizeof(struct xt_setproto_info),
		.me         = THIS_MODULE,
	},
};

static int __init setproto_tg_init(void)
{
	return xt_register_targets(setproto_tg_reg, ARRAY_SIZE(setproto_tg_reg));
}

static void __exit setproto_tg_exit(void)
{
	return xt_unregister_targets(setproto_tg_reg, ARRAY_SIZE(setproto_tg_reg));
}

module_init(setproto_tg_init);
module_exit(setproto_tg_exit);
MODULE_AUTHOR("Joe Hu");
MODULE_DESCRIPTION("Xtables: SETPROTO target");
MODULE_LICENSE("GPL");
MODULE_ALIAS("ipt_SETPROTO");
