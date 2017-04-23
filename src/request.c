#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "request.h"

void set_req_key(Request *request, const char *raw, int raw_size, int offset) {
  if (raw_size > offset && raw[offset] != ' ') {
    int max_key_size = raw_size - offset > REQUEST_KEY_MAX_SIZE ? REQUEST_KEY_MAX_SIZE : raw_size - offset;
    char *key_end = memchr(raw + offset, ' ', max_key_size);

    if (key_end) {
      int key_size = key_end - raw - offset;
      request->key = strndup(raw + offset, key_size);
      request->key_size = key_size;
    } else {
      request->key = strndup(raw + offset, max_key_size);
      request->key_size = max_key_size;
    }
  } else {
    request->type = REQUEST_ERR_ARGUMENTS;
    request->key_size = 0;
  }
}

void set_req_value(Request *request, const char *raw, int raw_size, int offset) {
  if (request->type <= 0) return;

  int value_offset = offset + request->key_size + 1;
  if (value_offset < raw_size && raw[value_offset] != ' ') {
    int value_size = raw_size - value_offset;

    request->value = strndup(raw + value_offset, value_size);
    request->value_size = value_size;
  } else {
    request->type = REQUEST_ERR_ARGUMENTS;
    request->value_size = 0;
  }
}

Request *Request_new() {
  Request *req = malloc(sizeof(Request));

  req->type = 0;
  req->key = NULL;
  req->key_size = 0;
  req->value = NULL;
  req->value_size = 0;

  return req;
}

void Request_parse(Request *request, const char *raw, int raw_size) {
  assert(request);
  assert(raw);

  if ((raw_size >= 4) && (strncmp(raw, "GET ", 4) == 0)) {
    request->type = REQUEST_GET;
    set_req_key(request, raw, raw_size, 4);
  } else if ((raw_size >= 4) && (strncmp(raw, "SET ", 4) == 0)) {
    request->type = REQUEST_SET;
    set_req_key(request, raw, raw_size, 4);
    set_req_value(request, raw, raw_size, 4);
  } else if ((raw_size >= 4) && (strncmp(raw, "DEL ", 4) == 0)) {
    request->type = REQUEST_DEL;
    set_req_key(request, raw, raw_size, 4);
  } else if ((raw_size >= 5) && (strncmp(raw, "INCR ", 5) == 0)) {
    request->type = REQUEST_INCR;
    set_req_key(request, raw, raw_size, 5);
  } else if ((raw_size >= 5) && (strncmp(raw, "DECR ", 5) == 0)) {
    request->type = REQUEST_DECR;
    set_req_key(request, raw, raw_size, 5);
  } else if ((raw_size >= 4) && (strncmp(raw, "URL ", 4) == 0)) {
    request->type = REQUEST_URL;
    set_req_key(request, raw, raw_size, 4);
  } else if ((raw_size == 5) && (strncmp(raw, "FLUSH", 5) == 0)) {
    request->type = REQUEST_FLUSH;
  } else if ((raw_size == 4) && (strncmp(raw, "INFO", 4) == 0)) {
    request->type = REQUEST_INFO;
  } else {
    request->type = REQUEST_ERR_PARSING;
  }
}

void Request_free(Request *req) {
  assert(req);

  if (req->key) {
    free(req->key);
    req->key = NULL;
  }
  if (req->value) {
    free(req->value);
    req->value = NULL;
  }
}
