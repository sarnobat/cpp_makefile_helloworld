 #ifndef FMF_COMMON_H
#define FMF_COMMON_H

#include <mqueue.h>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <cstdlib>

struct LocationMessage {
    int client_id;
    double lat;
    double lon;
};

static const char *MQ_CLIENTS = "/fmf_clients";
static const char *MQ_MID     = "/fmf_mid";
static const char *MQ_OUT     = "/fmf_out";

static const long MQ_MAXMSG = 10;
static const long MQ_MSGSIZE = sizeof(LocationMessage);

inline void perror_exit(const char *msg) {
    std::perror(msg);
    std::exit(EXIT_FAILURE);
}

#endif // FMF_COMMON_H
