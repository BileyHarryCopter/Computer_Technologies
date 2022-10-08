#include <stdio.h>
#include <unistd.h>

int main (int argc, char* argv[])
{
    execl ("/bin/ls", "/bin/ls", argv[1], (char*)NULL);

    return 0;
}