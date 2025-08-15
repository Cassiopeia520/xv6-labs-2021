#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

#define MAXLEN 32

int main(int argc, char *argv[]) 
{
    char *cmd = "echo";                           
    char line_buf[MAXLEN * MAXARG] = {0};         
    char *wp;                                     
    char *argv_exec[MAXARG];                      
    int base_argc = 0;                            
    int i;
    int nread;                                    

    // 读取参数
    if (argc > 1) 
    {
        if (argc + 1 > MAXARG) 
        {
            fprintf(2, "xargs: too many args\n");
            exit(1);
        }
        cmd = argv[1];

        for (i = 1; i < argc; ++i) 
        {
            argv_exec[base_argc++] = argv[i];
        }
    } 
    else 
    {
        argv_exec[base_argc++] = cmd;
    }

    wp = line_buf;

    while (1) 
    {
        // 逐字符读取，直到换行或 EOF
        while (1) 
        {
            nread = read(0, wp, 1);
            if (nread == 0 || *wp == '\n') 
            {
                break;
            }
            ++wp;
        }
        *wp = 0;  // 行字符串结束

        // 将该行作为一个额外参数
        argv_exec[base_argc] = line_buf;
        argv_exec[base_argc + 1] = 0; 

        // fork/exec 执行
        if (fork() == 0) 
        {
            exec(cmd, argv_exec);
            fprintf(2, "xargs: exec %s failed\n", cmd);
            exit(1);
        } 
        else 
        {
            wait((int *)0);
            wp = line_buf;  // 重置行缓冲，继续读下一行
        }

        if (nread == 0)
        {
            break;  // EOF
        }
    }
    exit(0);
}