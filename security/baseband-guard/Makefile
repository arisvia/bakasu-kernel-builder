bbg-objs += baseband_guard.o
bbg-objs += tracing/tracing.o
bbg-objs += blkdev_helper.o

ccflags-y += -I$(srctree)/security/selinux -I$(srctree)/security/selinux/include -I$(srctree)/block
ccflags-y += -I$(objtree)/security/selinux -include $(srctree)/include/uapi/asm-generic/errno.h

obj-$(CONFIG_BBG) += bbg.o

ifneq ($(wildcard $(objtree)/security/selinux/flask.h),)
  $(info -- Baseband-guard: flask.h found, skip generation)
else
  $(info -- Baseband-guard: flask.h not found, starting generation)
  $(shell $(HOSTCC) -I$(srctree)/scripts/selinux/genheaders -Wall -Wmissing-prototypes -Wstrict-prototypes -O2 -fomit-frame-pointer -std=gnu89 -I$(srctree)/include/uapi -I$(srctree)/include -I$(srctree)/security/selinux/include -o $(objtree)/genheaders $(srctree)/scripts/selinux/genheaders/genheaders.c)
  $(shell $(objtree)/genheaders flask.h av_permissions.h)
  $(shell mkdir -p $(objtree)/security/selinux)
  $(shell mv $(objtree)/flask.h $(objtree)/av_permissions.h $(objtree)/security/selinux)
endif

REPO_LINK := https://github.com/vc-teahouse/Baseband-guard
COMMIT_SHA := a54e0dc9

ifeq ($(shell grep -q "file_ioctl_compat" $(srctree)/include/linux/lsm_hook_defs.h $(srctree)/include/linux/lsm_hooks.h 2>/dev/null && echo true),true)
    ccflags-y += -DBB_HAS_IOCTL_COMPAT
endif

ifeq ($(shell grep -q "selinux_state" $(srctree)/security/selinux/include/security.h 2>/dev/null && echo true),true)
    ccflags-y += -DBB_HAS_SELINUX_STATE
endif

ifneq ($(shell grep -q "disk_get_part" $(srctree)/include/linux/genhd.h 2>/dev/null && echo true),true)
    ccflags-y += -DBBG_COMPAT_HAS_BLOCK_DEVICE_API
endif

ccflags-y += -DBBG_VERSION=\"$(COMMIT_SHA)\"
ccflags-y += -DBBG_REPO=\"$(REPO_LINK)\"
