#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
  #define CLOSESOCK closesocket
#else
  #include <unistd.h>
  #include <arpa/inet.h>
  #include <sys/socket.h>
  #define CLOSESOCK close
  typedef int SOCKET;
  #define INVALID_SOCKET (-1)
#endif

static int recv_all(SOCKET s, void *buf, int bytes) {
    char *p = (char*)buf;
    int recvd = 0;
    while (recvd < bytes) {
        int n = recv(s, p + recvd, bytes - recvd, 0);
        if (n <= 0) return n;
        recvd += n;
    }
    return recvd;
}

static int send_all(SOCKET s, const void *buf, int bytes) {
    const char *p = (const char*)buf;
    int sent = 0;
    while (sent < bytes) {
        int n = send(s, p + sent, bytes - sent, 0);
        if (n <= 0) return n;
        sent += n;
    }
    return sent;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: server_int <puerto>\n");
        return 1;
    }
    int port = atoi(argv[1]);

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    SOCKET listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == INVALID_SOCKET) { perror("socket"); return 1; }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons((unsigned short)port);

    int opt = 1;
#ifdef _WIN32
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
#else
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    if (bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); return 1; }
    if (listen(listenfd, 5) < 0) { perror("listen"); return 1; }

    printf("Servidor C (enteros) escuchando en puerto %d\n", port);

    while (1) {
        struct sockaddr_in cli;
#ifdef _WIN32
        int clen = sizeof(cli);
#else
        socklen_t clen = sizeof(cli);
#endif
        SOCKET client = accept(listenfd, (struct sockaddr*)&cli, &clen);
        if (client == INVALID_SOCKET) { perror("accept"); continue; }

        printf("Cliente conectado.\n");

        while (1) {
            uint32_t netint;
            int r = recv_all(client, &netint, 4);
            if (r <= 0) { printf("Cliente desconectado.\n"); break; }

            uint32_t x = ntohl(netint);
            printf("Recibido: %u\n", x);

            if (x == 0) {
                printf("Fin (recibio 0).\n");
                break;
            }

            uint32_t y = x + 1;
            uint32_t nety = htonl(y);
            if (send_all(client, &nety, 4) <= 0) { printf("Error enviando.\n"); break; }

            printf("Enviado: %u\n", y);
        }

        CLOSESOCK(client);
    }

    CLOSESOCK(listenfd);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}