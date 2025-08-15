#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

const int INT_LEN = sizeof(int);

// 读取左邻管道的首个整数
int read_left_first(int lpipe[2],int *dst)
{
    if(read(lpipe[RD], dst, INT_LEN)==INT_LEN)
    {
        printf("prime %d\n", *dst);
        return 1;
    }

    return 0;
}

void pump_filter(int lpipe[2],int rpipe[2],int first)
{
    int num;

    // 从左管道读取数据
    while(read(lpipe[RD],&num, INT_LEN)==INT_LEN)
    {
        if(num % first != 0)
        {
            write(rpipe[WR], &num, INT_LEN); 
        }
    }

    close(lpipe[RD]); // 关闭左管道的读端
    close(rpipe[WR]); // 关闭右管道的写端
}

void primes(int lpipe[2])
{
    close(lpipe[WR]); // 关闭写端

    int first;

    if(read_left_first(lpipe, &first))
    {
        int p[2];
        pipe(p);
        pump_filter(lpipe, p, first);

        if(fork() == 0)
        {
            primes(p);
        }
        else
        {
            close(p[RD]);
            wait(0); // 等待子进程结束
        }
    }
    exit(0);

}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);

    // 写出初始数据
    for(int i=2; i <= 35; i++)
        write(p[WR], &i, INT_LEN);

    if(fork()==0)
    {
        primes(p);
    }
    else
    {
        close(p[WR]); // 关闭写端
        close(p[RD]);
        wait(0); // 等待子进程结束
    }

    exit(0);
}