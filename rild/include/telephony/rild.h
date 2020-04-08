#ifndef __RILD_H__
#define __RILD_H__

#define LOG_TAG "RILD"
#define RIL_SHLIB 1


#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <assert.h>

#include <telephony/ril.h>
#include <telephony/record_stream.h>
#include <proto/sap-api.pb.h>
#include <libril/ril_ex.h>
//#define ANDROID_MULTI_SIM 1
//#define ANDROID_SIM_COUNT_2

#define HAVE_SYS_UIO_H
#include <cutils/log.h>
#include <cutils/uio.h>

#include <cutils/properties.h>
/* use sys/socket */
#define HAVE_SYS_SOCKET_H 1
#include <sys/socket.h>

#include <cutils/sockets.h>

#include <sys/capability.h>
#include <linux/prctl.h>
#include <sys/prctl.h>
#include <sys/stat.h>

#define __BIONIC_FORTIFY

#define HAVE_ANDROID_OS
#include <private/android_filesystem_config.h>

#include "hardware/qemu_pipe.h"

#include "proto/pb.h"

#define LIB_PATH_PROPERTY   "rild.libpath"
#define LIB_ARGS_PROPERTY   "rild.libargs"


#define MAX_LIB_ARGS        16
#define DEFAULT_SIM_COUNT   2


#define MULTISIM_COUNT_PROPERTY         "ro.multisim.simslotcount"
#define STATUS_RIL_DAEMON_PROPERTY      "init.svc.ril-daemon1"
#define STOP_DAEMON_PROPERTY            "ctl.stop"
#define START_DAEMON_PROPERTY           "ctl.start"

#define RIL_DAEMON                      "ril-daemon1"
#define LIB_SEC_RIL                     "libsec-ril"

#define ATOI_NULL_HANDLED(x) (x ? atoi(x) : 0)

#define MAX_RILDS 3
#define MAX_CLIENT_ID_LENGTH 2


#endif /* __RILD_H__ */

