#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_EXIT 1

#define STDIN 0
#define STDOUT 1


int sys_read(char *arr, unsigned size){
    unsigned ret;
    asm volatile("int $0x80" : "=a"(ret) : "a"(SYS_READ), "b"(STDIN), "c"(arr), "d"(size): "memory", "cc");
    return ret;
}

int sys_write(char *arr, unsigned size){
    unsigned ret;
    asm volatile("int $0x80": "=a"(ret) : "a"(SYS_WRITE), "b"(STDOUT), "c"(arr), "d"(size) : "memory");
    return ret;
}

void print(unsigned long long num){
    char temp[26];
    char buf[11];
    int i = 0, j = 0;
    if (num == 0){
        sys_write("0x0\n", 5);
        return;
    }
    while (num > 0){
        temp[i++] = "0123456789abcdef"[num%16];
        num /= 16;
    }
    
    buf[j++] = '0';
    buf[j++] = 'x';
    while(--i >= 0){
        buf[j++] = temp[i];
    }
    buf[j++] = '\n';
    buf[j++] = '\0';
    sys_write(buf, 11);
}

void sys_exit(int code){
    asm volatile("int $0x80" : : "a"(SYS_EXIT), "b"(code));
}

int isnum(char ch)
{
    return ch >= '0' && ch <= '9';
}

int isspc(char ch)
{
    return ch == ' ' || ch == '\n';
}

int _start(){
    unsigned long long  num = 0;
    char buf[20];
    int ret, i = 0, num_digits = 0;;
    unsigned chars_to_process = 0;
    for( ; ; i++, chars_to_process--){
        if (chars_to_process == 0){
            ret = sys_read(buf, sizeof(buf));
            if (ret < 0) sys_exit(1);
            i = 0;
            chars_to_process = ret;
        }
        if (num_digits > 0 && (chars_to_process == 0 || !isnum(buf[i]))) {
            print(num);
            num_digits = 0;
            num = 0;
        }
        if(chars_to_process == 0 || (!isspc(buf[i]) && !isnum(buf[i]))) sys_exit(0);
        if (isnum(buf[i])){
            num = num * 10 + buf[i] - '0';
            num_digits++;
        }
        else if (isspc(buf[i])) continue;
        else if (num_digits == 0 && num == 0) {
            print(num);
        }
    }
}