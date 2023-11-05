#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>

#define MSGKEY 75

struct msgform
{
	long mtype;
	char mtext[1000];
} msg;

int msgqid;

void run_server()
{
	// 请填写
    msgform msg;
	while(1){
		int n = msgrcv(msgqid, &msg, sizeof(msg.mtext), 0, 0);
		printf("%ld\n", msg.mtype);
		if(msg.mtype == 1) break;
		printf("2021302191650 于丰林 (server)reveived\n");
	}
}

void run_client()
{
	// 请填写
	msgform msg;
	for(int i=10; i>=1; --i){
		msg.mtype = i;
		msgsnd(msgqid, &msg, sizeof(msg.mtext), 0);
		printf("(client)sent\n");
		sleep(1);
	}
}

int main()
{
    msgqid = msgget(MSGKEY, IPC_CREAT | 0666);
	int pid;
	pid = fork();
	if (pid == 0)
	{
		sleep(1);
		run_client();
		exit(0);
	}
	else
	{
		run_server();
	}
	wait(NULL);
	return 0;
}
