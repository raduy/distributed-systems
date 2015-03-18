#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#define BUFLEN 1024

uint32_t read_int(int sock_fd);

FILE *downloadFile(int sock_fd, char *file_name_buffer, uint32_t file_content_size);

int main(int argc, char **argv) {
    int sock_fd;
    int len;
    struct sockaddr_in serv_addr;

//    if (argc != 3) {
//        printf("usage: %s <IP address> <TCP port>\n", argv[0]);
//        exit(EXIT_FAILURE);
//    }

    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (!sock_fd) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(atoi("4444"));

    //TODO ERROR HANDLING
    connect(sock_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    printf("Client conneced to server!\n");

    printf("Receiving file name length...");
    uint32_t file_name_len = read_int(sock_fd);
    printf("\t\t\tfile name received! its %d bytes\n", file_name_len);

    printf("Receiving file name...");
    char *file_name_buffer = malloc(file_name_len); //todo error handling
    recv(sock_fd, file_name_buffer, file_name_len, 0);

    printf("\t\t\t\tfile name is: %s\n", file_name_buffer);

    printf("Receiving file content size...");
    uint32_t file_content_size = read_int(sock_fd);
    printf("\t\t\tits %d bytes\n", file_content_size);

    printf("Filing file with content...");
    FILE *file = downloadFile(sock_fd, file_name_buffer, file_content_size);

    fclose(file);
    free(file_name_buffer);
    return EXIT_SUCCESS;
}

FILE *downloadFile(int sock_fd, char *file_name_buffer, uint32_t file_content_size) {
    FILE* file = fopen(file_name_buffer, "wb");
    if (!file) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    char file_buffer[1024];
    uint32_t received_bytes = 0;
    uint32_t total_received = 0;
    while(total_received < file_content_size) {
        received_bytes = recv(sock_fd, file_buffer, 1024, 0);
        fwrite(file_buffer, sizeof(char), received_bytes, file);
        total_received += received_bytes;
    }
    return file;
}

uint32_t read_int(int sock_fd) {
    uint32_t file_name_len;
    recv(sock_fd, &file_name_len, sizeof(file_name_len), 0);

    file_name_len = ntohl(file_name_len);
    return file_name_len;
}

