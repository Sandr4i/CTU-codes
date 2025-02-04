#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

#define EXIT_CODE_OK 0
#define EXIT_CODE_ERROR 1
#define EXIT_CODE_UNEXPECTED 2

void sigterm_handler();

int main(){
    pid_t gen, nsd;
    int pipefd[2];

    if (pipe(pipefd) == -1) _exit(EXIT_CODE_UNEXPECTED);
    
    // Process GEN
    gen = fork();
    if (gen == -1) _exit(EXIT_CODE_UNEXPECTED);
    else if (gen == 0){
        close(pipefd[0]); // Close read discriptor.
        if (dup2(pipefd[1], STDOUT_FILENO) == -1){
            close(pipefd[1]); 
            _exit(EXIT_CODE_UNEXPECTED);
        }
        
        close(pipefd[1]); // Close write discriptor
        
        signal(SIGTERM, sigterm_handler);
        
        char buffer[12];
        int buffer_size;
        srand(time(NULL));
        while (1){
            buffer_size = snprintf(buffer, 11, "%d %d\n", rand() % 4096, rand() % 4096);
            write(STDOUT_FILENO, buffer, buffer_size);
            sleep(1);
        }
    }

    // Process NSD
    nsd = fork();
    if (nsd == -1) _exit(EXIT_CODE_UNEXPECTED);
    else if (nsd == 0){
        close(pipefd[1]); // Close write discriptor
        if (dup2(pipefd[0], STDIN_FILENO) == -1){
            close(pipefd[0]);
            _exit(EXIT_CODE_UNEXPECTED);
        }
        close(pipefd[0]);
        if (execl("./nsd", "nsd", NULL) == -1){
            close(STDIN_FILENO);
            _exit(EXIT_CODE_UNEXPECTED);
        }
        close(STDIN_FILENO);
        _exit(EXIT_CODE_OK);
    }

    // Close read and write in the main process
    close(pipefd[0]);
    close(pipefd[1]);
    sleep(5);

    int gen_ret, nsd_ret;
    
    kill(gen, SIGTERM);
    wait(&gen_ret);
    wait(&nsd_ret);
    // In case exit code 2 or there is a problem with exiting
    if ((!WIFEXITED(nsd_ret) || (WIFEXITED(nsd_ret) && WEXITSTATUS(nsd_ret) == EXIT_CODE_UNEXPECTED))
        || (!WIFEXITED(gen_ret) || (WIFEXITED(gen_ret) && WEXITSTATUS(gen_ret) == EXIT_CODE_UNEXPECTED))) {
            write(STDOUT_FILENO, "ERROR\n", 6);
            close(STDOUT_FILENO);
            _exit(EXIT_CODE_ERROR);
    }
    
    write(STDOUT_FILENO, "OK\n", 3);
    close(STDOUT_FILENO);
    _exit(EXIT_CODE_OK);

    return 0;
}

void sigterm_handler(){
    close(STDOUT_FILENO);
    write(STDERR_FILENO, "GEN TERMINATED\n", 15);
    _exit(EXIT_CODE_OK);
}
