#include "pipe_networking.h"
#include <ctype.h>
void process(int sd) {
    while(sd){
      char msg[100];
      int n = read(sd, msg, sizeof(msg));
      if (n) {
          int i = 0;
          while (msg[i]) {
              msg[i] = toupper(msg[i]);
              i++;
          }

          write(sd, msg, sizeof(msg));
      }
    }
}
void operator(int sd){
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);
  client_socket = accept(sd,(struct sockaddr *)&client_address, &sock_size);

  int f = fork();
    if (f) {
      operator(sd);
    } else {
      process(client_socket);
    }
}
int server(){
  struct addrinfo * hints, * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, "9845", hints, &results);

  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  bind(sd, results->ai_addr, results->ai_addrlen);
  free(hints);
  freeaddrinfo(results);

  listen(sd, 10);
  printf("Listening\n");
  operator(sd);
  return sd;
}

int client(){
  struct addrinfo * hints, * results;
  hints = calloc(1,sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  getaddrinfo("127.0.0.1", "9845", hints, &results);

  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  connect(sd, results->ai_addr, results->ai_addrlen);
  printf("Connected to server!\n");
/*  while(1){
    char line[100];
    printf("input string: \n");
    fgets(line, 100, stdin);
    int hoo = write(sd, line, 100);

    if (hoo == -1){
      printf("Error: %s\n", strerror(errno));
      return 0;
    }
    int goo = read(sd, line, 100);
    if (goo == -1){
          printf("Error: %s\n", strerror(errno));
          return 0;
      }
      printf("Processed String: %s \n", line);
}*/
  free(hints);
  freeaddrinfo(results);

  return sd;
}
