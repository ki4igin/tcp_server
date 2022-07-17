#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <signal.h>

#include <netinet/in.h>
#include <unistd.h>

#define _BSD_SOURCE
#include <netdb.h>

#include "get_num.h"

// #define SV_SOCK_PATH "/tmp/us_xfr"
// #define BUF_SIZE 100
#define BACKLOG 50
#define INT_LEN 30
#define PORT_NUM "2021"

#define arrlen(_arr) (sizeof(_arr) / sizeof((_arr)[0]))

static void errMsg()
{
}

uint32_t tx_buf[64 * 1024];

int main(int argc, char *argv[])
{
    printf("Hello\n");

    for (uint32_t i = 0; i < arrlen(tx_buf); i++)
    {
        tx_buf[i] = i;
    }

    struct sockaddr_storage claddr;
    int lfd, cfd, optval;
    socklen_t addrlen;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
#define ADDRSTRLEN (NI_MAXHOST + NI_MAXSERV + 10)
    char addrStr[ADDRSTRLEN];
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
    {
        exit(EXIT_FAILURE);
    }

    if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
    {
        exit(EXIT_FAILURE);
    }

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC; /* Поддержка IPv4 или IPv6 */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
    /* Универсальный IP-адрес; имя службы имеет числовой формат */

    if (getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0)
    {
        exit(EXIT_FAILURE);
    }

    optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (lfd == -1)
            continue; /* В случае ошибки пробуем следующий адрес */
        if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval,
                       sizeof(optval)) == -1)
            exit(EXIT_FAILURE);

        if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break; /* Успех */
        /* Вызов bind() завершился неудачно; закрываем этот сокет и пробуем следующий адрес */
        close(lfd);
    }
    if (rp == NULL)
        exit(EXIT_FAILURE);
    if (listen(lfd, BACKLOG) == -1)
        exit(EXIT_FAILURE);

    freeaddrinfo(result);

    addrlen = sizeof(struct sockaddr_storage);
    cfd = accept(lfd, (struct sockaddr *)&claddr, &addrlen);
    if (cfd == -1)
    {
        exit(EXIT_FAILURE);
    }
    if (getnameinfo((struct sockaddr *)&claddr, addrlen,
                    host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
        snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
    else
        snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
    printf("Connection from %s\n", addrStr);
    /* Считываем запрос клиента, возвращаем в ответ элемент последовательности */

    while (1)
    {
        uint32_t size = write(cfd, tx_buf, sizeof(tx_buf));
        if (size != sizeof(tx_buf))
        {
            fprintf(stderr, "Error on write, send %d bytes\n", size);
            break;
        }
        static uint32_t cnt = 0;
        if ((cnt++ & 0xF) == 0)
        {
            printf(" send %d bytes\n", size);
        }
        usleep(100000);
    }

    if (close(cfd) == -1) /* Закрываем соединение */
        exit(EXIT_FAILURE);

    // getchar();
    exit(EXIT_SUCCESS);
}