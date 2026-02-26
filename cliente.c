#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #pragma comment(lib, "Ws2_32.lib")
  #define CLOSESOCK closesocket
#else
  #include <unistd.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <sys/socket.h>
  #define CLOSESOCK close
  typedef int SOCKET;
  #define INVALID_SOCKET (-1)
#endif

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: client_text <ip/host> <puerto>\n");
        return 1;
    }

#ifdef _WIN32
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
#endif

    const char *host = argv[1];
    int port = atoi(argv[2]);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) { perror("socket"); return 1; }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((unsigned short)port);

    // Resolver host (IP o nombre)
    if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0) {
        struct hostent *he = gethostbyname(host);
        if (!he) { printf("No pude resolver host\n"); return 1; }
        memcpy(&addr.sin_addr, he->h_addr_list[0], he->h_length);
    }

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        CLOSESOCK(sock);
        return 1;
    }

    printf("Conectado. Escribe texto (exit para salir)\n");

    char sendbuf[1024];
    char recvbuf[2048];

    while (1) {
        printf("> ");
        if (!fgets(sendbuf, sizeof(sendbuf), stdin)) break;

        // Enviar tal cual (incluye '\n' de fgets)
        int len = (int)strlen(sendbuf);
        if (send(sock, sendbuf, len, 0) < 0) { perror("send"); break; }

        // Leer respuesta (hasta que llegue '\n')
        int total = 0;
        while (total < (int)sizeof(recvbuf) - 1) {
            int n = recv(sock, recvbuf + total, 1, 0);
            if (n <= 0) { printf("Servidor cerro.\n"); goto end; }
            if (recvbuf[total] == '\n') { total++; break; }
            total += n;
        }
        recvbuf[total] = '\0';
        printf("Servidor: %s", recvbuf);

        if (strncasecmp(sendbuf, "exit", 4) == 0) break;
    }

end:
    CLOSESOCK(sock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}