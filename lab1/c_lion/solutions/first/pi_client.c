#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define BUFLEN 1024

struct length_variable {
    char length_in_bytes;
    unsigned char *value;
};

char compute_size_of_number(long long n) {
    if (n >> 8 == 0) {
        return 1;
    }
    if (n >> 16 == 0) {
        return 2;
    }
    if (n >> 32 == 0) {
        return 4;
    }

    return 8;
}

int request_size;
struct length_variable result_lv;
unsigned char buffer[8];

void print_bytes(void *p, size_t len) {


    result_lv.value = buffer;

    size_t i;
    printf("(");
    for (i = 0; i < len; ++i) {
        buffer[i] = ((unsigned char *) p)[i];

        printf("%02X ", ((unsigned char *) p)[i]);
    }
    printf(")\n");

    for (i = 0; i < len; ++i) {
//        buffer[i] = ((unsigned char *) p)[i];

        printf("%02X ", result_lv.value[i]);
    }
}

void print_double(unsigned long x) {
    print_bytes(&x, sizeof(x));
}

unsigned char result[10];

unsigned char *prepare_user_request() {
    unsigned long requested_digit;
    scanf("%lu", &requested_digit);

    printf("ok, you want %lu th number of PI fract. Lets ask serwer for that\n", requested_digit);

    char size_in_bytes = compute_size_of_number(requested_digit);
    result_lv.length_in_bytes = size_in_bytes;


    print_double(requested_digit);


    int size_bytes = result_lv.length_in_bytes;

//    char *result = calloc(size_bytes + 2, sizeof(unsigned char));
//    result = (unsigned char *) result;


//    char *tmp = calloc(sizeof(long), sizeof(char));

//    memcpy(tmp, &requested_digit, sizeof(long));
//    for (int i = 0; i < 10; i++) {
//        printf("%02x .", ((unsigned char *) tmp)[i]);
//    }

//    int ulong_size = sizeof(unsigned long);
//    for (int i = 0; i < ulong_size / 2; i++) {
//        int swap = tmp[i];
//        tmp[i] = tmp[ulong_size - i];
//        tmp[ulong_size - i] = swap;
//    }

    printf("Your number is %d bytes size\n", size_bytes);

//    uint32_t higher_bytes, lower_bytes;
//    higher_bytes = (uint32_t)(requested_digit >> 32) & 0xffffffff;
//    lower_bytes = (uint32_t) requested_digit & 0xffffffff;
//    higher_bytes = htonl(higher_bytes);
//    lower_bytes = htonl(lower_bytes);
//
//    memcpy(tmp, &higher_bytes, sizeof(long) / 2);
//    tmp++;
//    tmp++;
//    tmp++;
//    tmp++;
//    memcpy(tmp, &lower_bytes, sizeof(long) / 2);
//
//    tmp--;
//    tmp--;
//    tmp--;
//    tmp--;

//    for (int i = 0; i < 10; i++) {
//        printf("%02X  ", tmp[i]);
//    }


    for (int i = 1; i <= 8; i++) {
        unsigned char a = result_lv.value[i];
        printf("%02x \t", a);
    }
    printf("\n");


    for (int i = 1; i <= size_bytes; i++) {
        unsigned char a = result_lv.value[size_bytes - i];
        printf("insert %02x \n", a);
        result[i] = a;
    }
    result[0] = size_bytes;
    result[size_bytes + 1] = '\n';

    request_size = size_bytes + 2;

    for (int i = 0; i < 10; i++) {
        printf("%02X ", result[i]);
    }

    return result;
}

int main(int argc, char **argv) {
    int sock_fd;
    int len;
    struct sockaddr_in serv_addr;
    char recvline[BUFLEN];

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

    unsigned char *request;
    request = prepare_user_request();

    printf("request length: %d\n", request_size);
    for (int i = 0; i < 10; i++) {
        printf("%d ", request[i]);
    }

    printf("\n\n\n%d\n", request_size);
    for (int i = 0; i < 10; i++) {
        printf("%02X , ", result[i]);
    }
    len = send(sock_fd, request, request_size, 0);

    printf("Sent %d bytes to PiServer!\n", len);

    len = recv(sock_fd, recvline, BUFLEN, 0);
    printf("Respone received! Bytes: %d\n", len);
    printf("Server claims the response is: %01X\n", recvline[0]);

    return EXIT_SUCCESS;
}

