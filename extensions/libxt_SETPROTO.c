#include <stdio.h>
#include <getopt.h>
#include <xtables.h>
#include "compat_user.h"
#include "xt_SETPROTO.h"

static void setproto_tg_help(void)
{
	printf(
"SETPROTO options:\n"
"  --proto value	New protocol number\n"
);
}

static const struct xt_option_entry SETPROTO_opts[] = {
	{.name = "proto", .id = 1, .type = XTTYPE_PROTOCOL,
	 .flags = XTOPT_PUT | XTOPT_MAND,
	 XTOPT_POINTER(struct xt_setproto_info, proto)},
	XTOPT_TABLEEND,
};


static void SETPROTO_parse(struct xt_option_call *cb)
{
	xtables_option_parse(cb);
}

static void SETPROTO_check(struct xt_fcheck_call *cb)
{
	// Nothing to check
}

static void SETPROTO_print(const void *ip, const struct xt_entry_target *target,
                         int numeric)
{
	const struct xt_setproto_info *info =
		(const struct xt_setproto_info *)target->data;
	printf(" SETPROTO %u", info->proto);
}

static void SETPROTO_save(const void *ip, const struct xt_entry_target *target)
{
	const struct xt_setproto_info *info =
		(const struct xt_setproto_info *)target->data;

	printf(" --proto %u", info->proto);
}

static struct xtables_target setproto_tg_reg = {
	.version       = XTABLES_VERSION,
	.name          = "SETPROTO",
	.family        = NFPROTO_IPV4,
	.help          = setproto_tg_help,
	.size          = XT_ALIGN(sizeof(struct xt_setproto_info)),
	.userspacesize = XT_ALIGN(sizeof(struct xt_setproto_info)),
	.x6_parse      = SETPROTO_parse,
	.x6_fcheck     = SETPROTO_check,
	.x6_options    = SETPROTO_opts,
	.print         = SETPROTO_print,
	.save          = SETPROTO_save,
};

static __attribute__((constructor)) void setproto_tg_ldr(void)
{
	xtables_register_target(&setproto_tg_reg);
}
