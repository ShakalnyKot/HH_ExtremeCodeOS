#include <hyperhal/msg.h>
#include <msg.h>
#include <printf.h>
#include <types.h>

void msg_print(message_t* msg)
{
    kmsg("%s: Message priority: (%d)\n", __FUNCTION__, msg->priority);
    kmsg("%s: Message type: (%d)\n", __FUNCTION__, msg->type);
    kmsg("%s: Message sender: (%d)\n", __FUNCTION__, msg->sender);
    kmsg("%s: Message destination: (%d)\n", __FUNCTION__, msg->destination);
    kmsg("%s: Message data size: (%d)\n", __FUNCTION__, msg->dsize);
    kmsg("%s: Message divided?: (%d)\n", __FUNCTION__, msg->divided);
}

void absorb_msg(message_t* msg)
{
    switch (msg->type)
    {
    case 0x0:
        if(msg->destination < 1 && msg->sender < 1)
        {
            kmsg("Halt caused by message (msg->sender = %d)\n", msg->sender);
            do_halt();
        }
        break;
    
    default:
        kmsg("Unknown message from (msg->sender = %d)\n", msg->sender);
        break;
    }
}