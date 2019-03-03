#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>
#include<unistd.h>
#include<event.h>
#include<signal.h>


void sig_cb(int fd,short ev,void*arg)
{
    printf("fd=%d\n",fd);
}

void timeout_cb(int fd,short ev,void*arg)
{
    printf("time out\n");
}

int main()
{
    struct event_base* base = event_init();
    assert(base != NULL);

    //struct event *sig_ev = evsignal_new(base,SIGINT,sig_cb,NULL);
    struct event *sig_ev = event_new(base,SIGINT,EV_SIGNAL|EV_PERSIST,sig_cb,NULL);
    assert(sig_ev != NULL);
    event_add(sig_ev,NULL);

    struct timeval tv = {3,0};
    //struct event *time_ev = evtimer_new(base,timeout_cb,NULL);
    //struct event *time_ev = event_new(base,-1,EV_TIMEOUT,timeout_cb,NULL);
    struct event *time_ev = event_new(base,-1,EV_TIMEOUT|EV_PERSIST,timeout_cb,NULL);
    event_add(time_ev,&tv);

    event_base_dispatch(base);

    event_free(time_ev);
    event_free(sig_ev);
    event_base_free(base);
    

}
