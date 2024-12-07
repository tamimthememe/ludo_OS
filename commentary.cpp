#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>

using namespace std;

int main()
{

    // Ensure the named pipe exists
    if (mkfifo("ludoPipe", 0666) == -1)
    {
        perror("mkfifo");
    }

    std::cout << "Reader: Waiting for messages...\n";

    while (true)
    {
        int fd = open("ludoPipe", O_RDONLY); // Open pipe for reading
        if (fd == -1)
        {
            perror("open");
            return 1;
        }

        char buffer[1024];
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);

        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0'; // Null-terminate the string
            cout << "Read: " << buffer << endl;

            if (strcmp(buffer, "exit") == 0)
            {
                close(fd);
                break;
            }
        }

        close(fd);
    }

    return 0;
}
