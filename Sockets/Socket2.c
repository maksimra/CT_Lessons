    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>

    #define BUF_LEN     4096
    #define HTTP_PORT   80

    int main(const int argc, const char** argv)
    {
        int sock, count;
        char* buf;
        struct hostent* host;
        struct sockaddr_in addr;

        if (argc < 2)
        {
            fprintf(stderr, "Too few arguments\n");
            return EXIT_FAILURE;
        }

        buf = (char*)malloc(BUF_LEN);
        if (buf == NULL)
        {
            fprintf(stderr, "malloc() error\n");
            return EXIT_FAILURE;
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == -1)
        {
            fprintf(stderr, "socket() error\n");
            free(buf);
            return EXIT_FAILURE;
        }

        addr.sin_family = AF_INET;

        host = gethostbyname(argv[1]);
        if (host == NULL)
        {
            fprintf(stderr, "Unknown server\n");
            close(sock);
            free(buf);
            return EXIT_FAILURE;
        }

        addr.sin_addr = *((struct in_addr*)host->h_addr_list[0]);

        /* Host to Network Short */
        /* Каст к big endian */
        addr.sin_port = htons(HTTP_PORT);

        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        {
            fprintf(stderr, "connect() error\n");
            close(sock);
            free(buf);
            return EXIT_FAILURE;
        }

        snprintf(buf, BUF_LEN,
                "GET / HTTP/1.1\r\n"
                "Host: %s\r\n"
                "User-Agent: SimpleClient/1.0\r\n"
                "Connection: close\r\n"
                "\r\n", argv[1]);

        if (write(sock, buf, strlen(buf)) == -1)
        {
            fprintf(stderr, "write() error\n");
            close(sock);
            free(buf);
            return EXIT_FAILURE;
        }

        while ((count = read(sock, buf, BUF_LEN)) > 0)
        {
            if (write(STDOUT_FILENO, buf, count) == -1)
            {
                fprintf(stderr, "Error writing to stdout\n");
                break;
            }
        }

        if (count == -1)
        {
            fprintf(stderr, "read() error\n");
        }

        close(sock);
        free(buf);
        return EXIT_SUCCESS;
    }
