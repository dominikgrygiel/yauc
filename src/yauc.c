#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "request.h"
#include "response.h"

#define LISTEN_BACKLOG_SIZE 10
#define READ_BUFFER_SIZE 1024
#define CRLF "\r\n"

volatile int server_sock = -1;

Response *handle_client_request(Request *req) {
  Response *resp = Response_new();

  switch (req->type) {
    case REQUEST_INFO:
      break;
    case REQUEST_FLUSH:
      resp->type = RESPONSE_OK;
      break;
    case REQUEST_GET:
      break;
    case REQUEST_SET:
      break;
    case REQUEST_DEL:
      break;
    case REQUEST_INCR:
      break;
    case REQUEST_DECR:
      break;
    case REQUEST_URL:
      break;
    default:
      if (req->type < 0) {
        resp->type = (int)req->type;
      } else {
        resp->type = RESPONSE_ERR_UNKNOWN_REQUEST;
      }
      break;
  }

  Response_encode(resp);
  return resp;
}

void handle_client(int client_sock) {
  char read_buff[READ_BUFFER_SIZE] = {0};
  int read_bytes;

  while ((read_bytes = read(client_sock, &read_buff, READ_BUFFER_SIZE)) > 0) {
    char *req_end = strstr(read_buff, CRLF);
    Request *req = NULL;

    if (req_end) {
      int req_size = req_end - read_buff;
      char raw_req[req_size];
      memcpy(raw_req, read_buff, req_size);

      req = Request_new();
      Request_parse(req, raw_req, req_size);
    } else {
      // TODO: handle requests longer than buffer size and multiple requests inside buffer
    }

    if (req) {
      Response *resp = handle_client_request(req);

      write(client_sock, resp->encoded, resp->encoded_size);
      write(client_sock, CRLF, 2);

      Request_free(req);
      Response_free(resp);
    }
  }

  close(client_sock);
}

static int new_socket(int port) {
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    perror("socket()");
    return -1;
  }

  int on = 1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof on) == -1) {
    close(sock);
    return -1;
  }

  struct sockaddr_in address;
  address.sin_family = PF_INET;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (struct sockaddr*)&address, sizeof address) == -1) {
    perror("bind()");
    close(sock);
    return -1;
  }

  if (listen(sock, LISTEN_BACKLOG_SIZE) == -1) {
    perror("listen()");
    close(sock);
    return -1;
  }

  return sock;
}

void signal_handler(int sig) {
  switch (sig) {
    case SIGTERM:
    case SIGINT:
      fprintf(stderr, "YAUC shutting down...\n");
      close(server_sock);
      exit(0);
    default:
      fprintf(stderr, "YAUC wasn't expecting signal %d\n", sig);
      abort();
  }
}

int main(int argc, const char *argv[])
{
  fprintf(stderr, "Starting YAUC (Yet Another Useless Cache)\n");

  server_sock = new_socket(4040);
  if (server_sock == -1) {
    fprintf(stderr, "Could not create socket\n");
    exit(1);
  }

  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);

  while (1) {
    struct sockaddr_in client;
    socklen_t client_size = sizeof client;
    int client_sock = accept(server_sock, (struct sockaddr*)&client, &client_size);

    if (client_sock < 0) {
      fprintf(stderr, "Couldn't accept connection from client!\n");
    } else {
      handle_client(client_sock);
    }
  }

  return 0;
}
