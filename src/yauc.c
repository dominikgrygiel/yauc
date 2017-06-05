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
#include "database.h"

#define LISTEN_BACKLOG_SIZE 10
#define READ_BUFFER_SIZE 1024
#define CRLF "\r\n"

#define WELCOME_MESSAGE "Starting YAUC (Yet Another Useless Cache)\n"
#define DB_LOG_PATH "yauc.db"

volatile int server_sock = -1;
Database *db = NULL;
FILE *db_log = NULL;

Response *handle_request(Request *req) {
  Response *resp = Response_new();
  char *value;

  switch (req->type) {
    case REQUEST_INFO:
      Response_set_string(resp, WELCOME_MESSAGE);
      break;
    case REQUEST_FLUSH:
      Database_flush(db);
      resp->type = RESPONSE_OK;
      break;
    case REQUEST_GET:
      if ((value = Database_get(db, req->key))) {
        Response_set_string(resp, value);
      } else {
        resp->type = RESPONSE_EMPTY;
      }
      break;
    case REQUEST_SET:
      Database_set(db, req->key, req->value);
      resp->type = RESPONSE_OK;
      break;
    case REQUEST_DEL:
      Database_del(db, req->key);
      resp->type = RESPONSE_OK;
      break;
    case REQUEST_INCR:
      Response_set_number(resp, Database_incr(db, req->key));
      break;
    case REQUEST_DECR:
      Response_set_number(resp, Database_decr(db, req->key));
      break;
    case REQUEST_URL:
      Response_set_string(resp, "<html><body><h1>Welcome!</h1></body></html>");
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

    int raw_req_size = 0;
    char *raw_req = NULL;

    if (req_end) {
      raw_req_size = req_end - read_buff;
      raw_req = malloc(sizeof(char) * raw_req_size);
      memcpy(raw_req, read_buff, raw_req_size);

      req = Request_new();
      Request_parse(req, raw_req, raw_req_size);
    } else {
      // TODO: handle requests longer than buffer size and multiple requests inside buffer
    }

    if (req) {
      Response *resp = handle_request(req);

      fwrite(raw_req, sizeof(char), raw_req_size, db_log);
      fwrite("\n", sizeof(char), 1, db_log);

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

void setup_read_db_log() {
  if ((db_log = fopen(DB_LOG_PATH, "r"))) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, db_log)) != -1) {
      Request *req = Request_new();
      Request_parse(req, line, read - 1);
      line[read - 1] = '\0';

      Response *resp = handle_request(req);

      Request_free(req);
      Response_free(resp);
    }
    fclose(db_log);
  }

  db_log = fopen(DB_LOG_PATH, "a");
  if (db_log == NULL) {
    fprintf(stderr, "Couldn't open log file!\n");
    exit(2);
  }
}

void signal_handler(int sig) {
  switch (sig) {
    case SIGTERM:
    case SIGINT:
      fprintf(stderr, "YAUC shutting down...\n");
      Database_free(db);
      db = NULL;
      fclose(db_log);
      db_log = NULL;
      close(server_sock);
      exit(0);
    default:
      fprintf(stderr, "YAUC wasn't expecting signal %d\n", sig);
      abort();
  }
}

int main(int argc, const char *argv[])
{
  fprintf(stderr, WELCOME_MESSAGE);

  server_sock = new_socket(4040);
  if (server_sock == -1) {
    fprintf(stderr, "Could not create socket\n");
    exit(1);
  }

  db = Database_new();
  setup_read_db_log();

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
