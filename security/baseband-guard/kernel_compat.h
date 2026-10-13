#include <linux/blkdev.h>
#include <linux/security.h>
#include <linux/lsm_hooks.h>
#include <linux/version.h>
#include "objsec.h"
#include "blk.h"

#if LINUX_VERSION_CODE < KERNEL_VERSION(5,11,0)
static __maybe_unused inline int lookup_bdev_compat(char *path, dev_t *out)
{
	struct block_device *bdev;

	if (!path || !out)
		return 1;

	bdev = lookup_bdev(path);
	if (IS_ERR(bdev))
		return 1;
	*out = bdev->bd_dev;
	bdput(bdev);
	return 0;
}

static __maybe_unused bool bbg_is_named_device(dev_t dev, const char *name_prefix)
{
	struct block_device *bdev;
	bool match = false;

	bdev = blkdev_get_by_dev(dev, FMODE_READ, THIS_MODULE);
	if (IS_ERR(bdev))
		return false;

	if (bdev->bd_disk && name_prefix) {
		const char *disk_name = bdev->bd_disk->disk_name;
		size_t prefix_len = strlen(name_prefix);

		if (strncmp(disk_name, name_prefix, prefix_len) == 0)
			match = true;
	}

	blkdev_put(bdev, FMODE_READ);
	return match;
}
#endif

static __maybe_unused inline void __init security_add_hooks_compat(struct security_hook_list *hooks, int count)
{
	security_add_hooks(hooks, count, "baseband_guard");
}
