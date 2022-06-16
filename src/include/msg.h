#ifndef MSG_H
#define MSG_H
#include <types.h>

typedef struct message
{
    uint32_t priority;
    uint32_t type;
    uint32_t sender;
    uint32_t destination;
    uint32_t dsize;
    bool     divided;
    uint8_t  data[256];
} message_t;

#endif //  MSG_H