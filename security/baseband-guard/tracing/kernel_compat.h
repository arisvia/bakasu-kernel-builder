#include <linux/version.h>
#include <linux/cred.h>
#include "objsec.h"
#include "security.h"
#include "tracing.h"

static __maybe_unused inline bool selinux_initialized_compat(void)
{
#ifdef BB_HAS_SELINUX_STATE
	return selinux_state.initialized;
#else
	return ss_initialized;
#endif
}
