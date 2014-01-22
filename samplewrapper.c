#include<stdio.h>
#include<stdlib.h>
#include<linux/ipc.h>
#include<linux/msg.h>
main()
{
int qid;
key_t msgkey;
struct mymsgbuf
{
long mtype;
int request;
double number;
}
msg;
msgkey=ftok(".",'m');
if((qid=open_queue(msgkey))==-1)
{
perror("open_queue");
exit(1);
}
msg.mtype=1;
msg.request=1;
msg.number=1000;
if((send_message(qid,&msg))==-1)
{
perror("send_message");
exit(1);
}
}
