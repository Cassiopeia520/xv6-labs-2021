#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

#define PIPE_READ 0
#define PIPE_WRITE 1

int main(int argc, char *argv[])
{
    // 用于传递的字节
    char token = 'p';

    // 双向通信管道
    int child_to_parent[2];
    int parent_to_child[2];

    // 创建管道
    pipe(child_to_parent);
    pipe(parent_to_child);

    int pid = fork();
    int exit_code = 0;

    if(pid<0)
    {
        // 创建子进程失败
        printf("fork failed\n");
        exit(1);
    }
    
    if(pid == 0)
    {
        // 子进程
        close(parent_to_child[PIPE_WRITE]); 
        close(child_to_parent[PIPE_READ]);  

        // 子进程先从父进程里读数据，再往父进程里写数据
        if (read(parent_to_child[PIPE_READ], &token, sizeof(char)) != sizeof(char)) 
        {
            fprintf(2, "child read() error!\n");
            exit_code = 1; 
        }
        else 
        {
            // 读取成功，打印“received ping”
            fprintf(1, "%d: received ping\n", getpid());
        }
        
        if (write(child_to_parent[PIPE_WRITE], &token, sizeof(char)) != sizeof(char)) 
        {
            fprintf(2, "child write() error!\n");
            exit_code = 1;
        }
        
        close(parent_to_child[PIPE_READ]);
        close(child_to_parent[PIPE_WRITE]);

        exit(exit_code);
    }
    else
    {
        // 父进程
        close(parent_to_child[PIPE_READ]);  
        close(child_to_parent[PIPE_WRITE]); 

        // 父进程先往子进程里写数据，再从子进程里读数据
        if (write(parent_to_child[PIPE_WRITE], &token, sizeof(char)) != sizeof(char)) 
        {
            fprintf(2, "parent write() error!\n");
            exit_code = 1;
        }

        if (read(child_to_parent[PIPE_READ], &token, sizeof(char)) != sizeof(char)) 
        {
            fprintf(2, "parent read() error!\n");
            exit_code = 1;
        } 
        else 
        {
            // 读取成功，打印“received pong”
            fprintf(1, "%d: received pong\n", getpid());
        }
        

        close(parent_to_child[PIPE_WRITE]);
        close(child_to_parent[PIPE_READ]);

        exit(exit_code); // 等待子进程结束
    }

    exit(0);
}