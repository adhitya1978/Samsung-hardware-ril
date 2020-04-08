/* //device/system/rild/rild.c
**
** Copyright 2006, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

#include "telephony/rild.h"



#define HRL_ATTRIB_HIDDEN __attribute__((visibility("hidden")))
#define HRL_ATTRIB __attribute__((visibility("default")))

extern char ril_service_name_base[MAX_SERVICE_NAME_LENGTH];
extern char ril_service_name[MAX_SERVICE_NAME_LENGTH];

static void usage(const char *argv0)
{
    fprintf(stderr, "Usage: %s -l <ril impl library> [-- <args for impl library>]\n", argv0);
    exit(-1);
}

extern void RIL_register (const RIL_RadioFunctions *callbacks);

extern void rilc_thread_pool ();

extern void RIL_register_socket (const RIL_RadioFunctions *(*rilUimInit)
                                 (const struct RIL_Env *, int, char **), RIL_SOCKET_TYPE socketType, int argc, char **argv);

extern void RIL_onRequestComplete(RIL_Token t, RIL_Errno e,
                                  void *response, size_t responselen);

extern void RIL_setServiceName(char *);

extern void RIL_setcallbacks(const RIL_RadioFunctions *(*)
                             (const struct RIL_Env *, int, char **), const RIL_RadioFunctions, size_t scallbacks);

#if defined(ANDROID_MULTI_SIM)
extern void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
                                      size_t datalen, RIL_SOCKET_ID socket_id);
#else
extern void RIL_onUnsolicitedResponse(int unsolResponse, const void *data,
                                      size_t datalen);
#endif

extern void RIL_requestTimedCallback (RIL_TimedCallback callback,
                                      void *param, const struct timeval *relativeTime);

extern void RIL_onRequestAck (RIL_Token t);

static struct RIL_Env s_rilEnv = {
    RIL_onRequestComplete,
    RIL_onUnsolicitedResponse,
    RIL_requestTimedCallback,
    RIL_onRequestAck
};

extern void RIL_startEventLoop();

static int make_argv(char * args, char ** argv)
{
    // Note: reserve argv[0]
    int count = 1;
    char * tok;
    char * s = args;

    while ((tok = strtok(s, " \0"))) {
        argv[count] = tok;
        s = NULL;
        count++;
    }
    return count;
}

/*
 * switchUser - Switches UID to radio, preserving CAP_NET_ADMIN capabilities.
 * Our group, cache, was set by init.
 */
extern void switchUser() {
    prctl(PR_SET_KEEPCAPS, 1, 0, 0, 0);
    setuid(AID_RADIO);

    struct __user_cap_header_struct header;
    struct __user_cap_data_struct cap;
    header.version = _LINUX_CAPABILITY_VERSION;
    header.pid = 0;
    cap.effective = cap.permitted = (1 << CAP_NET_ADMIN) | (1 << CAP_NET_RAW);
    cap.inheritable = 0;
    capset(&header, &cap);
}

extern int main(int argc, char **argv)
{

    const char * rilLibPath = NULL;
    const char * simcount = NULL;
    const char * secDaemon = NULL;
    char **rilArgv;
    void *dlHandle;
    /* RIL_Init */
    const RIL_RadioFunctions *(*rilInit)(const struct RIL_Env *, int, char **);
    /* RIL_SAP_Init */
    const RIL_RadioFunctions *(*rilUimInit)(const struct RIL_Env *, int, char **);
    /* pointer RIL_Init */
    const RIL_RadioFunctions * funcsRIL_Init;
    /*Library sec-ril path*/
    char libPath[PROPERTY_VALUE_MAX];
    /* SIM Count*/
    char sCount[PROPERTY_VALUE_MAX];
    /* status daemon */
    char svcDaemon[PROPERTY_VALUE_MAX];
    /* string error*/
    const char * err_str = NULL;

    unsigned char hasLibArgs = 0;

    int i;
    // ril/socket id received as -c parameter, otherwise set to 0
    const char *clientId = NULL;

    RLOGI("**RIL Daemon Started**");
    RLOGI("**RILd param count=%d**", argc);

    /** all permission all allowed */
    /** read by group | write by group | execute/search by group | read by others | write by others | execute/search by others  */
    umask(S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
    for (i = 1; i < argc ;) {
        if (0 == strcmp(argv[i], "-l") && (argc - i > 1)) {
            rilLibPath = argv[i + 1];
            i += 2;
        } else if (0 == strcmp(argv[i], "--")) {
            i++;
            hasLibArgs = 1;
            break;
        }else if (0 == strcmp(argv[i], "-c") &&  (argc - i > 1)) {
            clientId = argv[i+1];
            i += 2;
        }else {
            usage(argv[0]);
        }
    }

    if (clientId == NULL) {
        clientId = "0";
    } else if (atoi(clientId) >= MAX_RILDS) {
        RLOGE("Max Number of rild's supported is: %d", MAX_RILDS);
        exit(0);
    }

    //RLOGI("Max Number of rild's supported is: %d", atoi(clientId));

    /* check support SIM Property*/
    if(0 == property_get(MULTISIM_COUNT_PROPERTY, sCount, NULL )){
        // No lib sepcified on the command line, and nothing set in props.
        // Assume "no sim" case.
        RLOGE("SIM COUNT %d",atoi(sCount));
        goto done;
    }else{
        simcount = sCount;
    }

    //RLOGI("SIM COUNT %d", atoi(simcount));

    /* restarting RIL_DAEMON SIM2 */
    if (atoi(simcount) == DEFAULT_SIM_COUNT && !strncmp(clientId, "0", MAX_CLIENT_ID_LENGTH)){

        //RLOGI("restart second sim");

        property_get(STATUS_RIL_DAEMON_PROPERTY, svcDaemon, NULL);

        secDaemon = svcDaemon;

        //RLOGI("check status daemon %s", secDaemon);
        if(strncmp(secDaemon, "running", 7) == 0)
            property_set(STOP_DAEMON_PROPERTY, RIL_DAEMON);

        property_set(START_DAEMON_PROPERTY, RIL_DAEMON);
    }

    //    RLOGI("get status daemon %s", secDaemon);

    if (strncmp(clientId, "0", MAX_CLIENT_ID_LENGTH)) {
        strncpy(ril_service_name, ril_service_name_base, MAX_SERVICE_NAME_LENGTH);
        strncat(ril_service_name, clientId, MAX_SERVICE_NAME_LENGTH);
        //RLOGI("RIL Service name %s", ril_service_name);
        RIL_setServiceName(ril_service_name);
    }

    if (rilLibPath == NULL) {
        if ( 0 == property_get(LIB_PATH_PROPERTY, libPath, NULL)) {
            // No lib sepcified on the command line, and nothing set in props.
            // Assume "no-ril" case.
            goto done;
        } else {
            rilLibPath = libPath;
        }
    }


    // RLOGI("RIL Lib path :%s", rilLibPath);

    //RLOGI("Check available lib sec ril");
    /** make sure main daemon lib is libsec-ril
    *   hack allowing any name of sec-ril

    if(!strstr(rilLibPath, LIB_SEC_RIL)){
        RLOGE("RIL_Init Stop, No libsec-ril");
        goto done;
    }
    */

    //RLOGI("open library");
    dlHandle = dlopen(rilLibPath, RTLD_NOW);

    if (dlHandle == NULL) {
        RLOGE("dlopen failed: %s", dlerror());
        exit(-1);
    }
    //RLOGI("begin loop");
    RIL_startEventLoop();

    rilInit = (const RIL_RadioFunctions *(*)(const struct RIL_Env *, int, char **))dlsym(dlHandle, "RIL_Init");
    /*  check error */
    err_str = dlerror();

    if (rilInit == NULL) {
        RLOGE("RIL_Init not defined or exported in %s: %s\n", rilLibPath, err_str);
        exit(-1);
    }

    dlerror(); // Clear any previous dlerror

    /* endsure our wrapper have SAP init */
    rilUimInit = (const RIL_RadioFunctions *(*)(const struct RIL_Env *, int, char **))dlsym(dlHandle, "RIL_SAP_Init");

    //RLOGI("Check error after load library");

    err_str = dlerror();
    if (err_str) {
        RLOGW("RIL_SAP_Init not defined or exported in %s: %s\n", rilLibPath, err_str);
    } else if (!rilUimInit) {
        RLOGW("RIL_SAP_Init defined as null in %s. SAP Not usable\n", rilLibPath);
    }

    //RLOGI("Check if has argument");
    if (hasLibArgs) {
        rilArgv = argv + i - 1;
        argc = argc -i + 1;
    } else {
        static char * newArgv[MAX_LIB_ARGS];
        static char args[PROPERTY_VALUE_MAX];
        rilArgv = newArgv;
        property_get(LIB_ARGS_PROPERTY, args, "");
        argc = make_argv(args, rilArgv);
    }
    //RLOGI("finish check arguments");

    rilArgv[argc++] = "-c";
    rilArgv[argc++] = (char*)clientId;

    RLOGD("RIL_Init argc = %d clientId = %s", argc, rilArgv[argc-1]);

    // Make sure there's a reasonable argv[0]
    rilArgv[0] = argv[0];

    //RLOGI("rilInit Start");

    funcsRIL_Init = rilInit(&s_rilEnv, argc, rilArgv);

    //RLOGI("rilInit completed");

    RIL_register(funcsRIL_Init);

    RLOGD("RIL_Init RIL_register completed");

    if(rilUimInit)
    {
        RIL_register_socket(rilUimInit, RIL_SAP_SOCKET, argc, rilArgv);
        RLOGD("RIL_register_socket completed");
    }


done:
    rilc_thread_pool();
    RLOGD("RIL_Init starting sleep loop");
    while(true) {
        // sleep(UINT32_MAX) seems to return immediately on bionic
        sleep(0xFFFFFFFF);
    }
}

#define HEADER_SIZE 4

struct RecordStream {
    int fd;
    size_t maxRecordLen;

    unsigned char *buffer;

    unsigned char *unconsumed;
    unsigned char *read_end;
    unsigned char *buffer_end;
};

/* returns NULL; if there isn't a full record in the buffer */
static unsigned char * getEndOfRecord (unsigned char *p_begin,
                                       unsigned char *p_end)
{
    size_t len;
    unsigned char * p_ret;

    if (p_end < p_begin + HEADER_SIZE) {
        return NULL;
    }

    //First four bytes are length
    len = ntohl(*((uint32_t *)p_begin));

    p_ret = p_begin + HEADER_SIZE + len;

    if (p_end < p_ret) {
        return NULL;
    }

    return p_ret;
}

static void *getNextRecord (RecordStream *p_rs, size_t *p_outRecordLen)
{
    unsigned char *record_start, *record_end;

    record_end = getEndOfRecord (p_rs->unconsumed, p_rs->read_end);

    if (record_end != NULL) {
        /* one full line in the buffer */
        record_start = p_rs->unconsumed + HEADER_SIZE;
        p_rs->unconsumed = record_end;

        *p_outRecordLen = record_end - record_start;

        return record_start;
    }

    return NULL;
}


extern RecordStream *record_stream_new(int fd, size_t maxRecordLen){
    RecordStream *ret;

    assert (maxRecordLen <= 0xffff);

    ret = (RecordStream *)calloc(1, sizeof(RecordStream));

    ret->fd = fd;
    ret->maxRecordLen = maxRecordLen;
    ret->buffer = (unsigned char *)malloc (maxRecordLen + HEADER_SIZE);

    ret->unconsumed = ret->buffer;
    ret->read_end = ret->buffer;
    ret->buffer_end = ret->buffer + maxRecordLen + HEADER_SIZE;

    return ret;
}
extern void record_stream_free(RecordStream *p_rs)
{
    free(p_rs->buffer);
    free(p_rs);
}

extern int record_stream_get_next (RecordStream *p_rs, void ** p_outRecord,
                                   size_t *p_outRecordLen)
{
    void *ret;

    ssize_t countRead;

    /* is there one record already in the buffer? */
    ret = getNextRecord (p_rs, p_outRecordLen);

    if (ret != NULL) {
        *p_outRecord = ret;
        return 0;
    }

    // if the buffer is full and we don't have a full record
    if (p_rs->unconsumed == p_rs->buffer
            && p_rs->read_end == p_rs->buffer_end
            ) {
        // this should never happen
        //ALOGE("max record length exceeded\n");
        assert (0);
        errno = EFBIG;
        return -1;
    }

    if (p_rs->unconsumed != p_rs->buffer) {
        // move remainder to the beginning of the buffer
        size_t toMove;

        toMove = p_rs->read_end - p_rs->unconsumed;
        if (toMove) {
            memmove(p_rs->buffer, p_rs->unconsumed, toMove);
        }

        p_rs->read_end = p_rs->buffer + toMove;
        p_rs->unconsumed = p_rs->buffer;
    }

    countRead = read (p_rs->fd, p_rs->read_end, p_rs->buffer_end - p_rs->read_end);

    if (countRead <= 0) {
        /* note: end-of-stream drops through here too */
        *p_outRecord = NULL;
        return countRead;
    }

    p_rs->read_end += countRead;

    ret = getNextRecord (p_rs, p_outRecordLen);

    if (ret == NULL) {
        /* not enough of a buffer to for a whole command */
        errno = EAGAIN;
        return -1;
    }

    *p_outRecord = ret;
    return 0;
}

