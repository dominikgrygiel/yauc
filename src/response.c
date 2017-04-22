#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "response.h"

void itoa_r(int n, char *buff_end) {
  while (n > 0) {
    *buff_end = (char) n % 10 + '0';
    buff_end--;
    n /= 10;
  }
}

Response *Response_new() {
  Response *resp = malloc(sizeof(Response));

  resp->type = 0;
  resp->text = NULL;
  resp->encoded = NULL;
  resp->encoded_size = 0;

  return resp;
}

Response *Response_new_string(char *text) {
  assert(text);

  Response *resp = Response_new();

  resp->type = RESPONSE_STRING;
  resp->text = strdup(text);

  return resp;
}

Response *Response_new_number(int number) {
  Response *resp = Response_new();

  resp->type = RESPONSE_NUMBER;
  resp->number = number;

  return resp;
}

void Response_set_string(Response *resp, char *text) {
  assert(resp);
  assert(text);

  resp->type = RESPONSE_STRING;
  resp->text = strdup(text);
}

void Response_set_number(Response *resp, int number) {
  assert(resp);

  resp->type = RESPONSE_NUMBER;
  resp->number = number;
}

void Response_encode(Response *resp) {
  assert(resp);

  switch (resp->type) {
    case RESPONSE_OK:
      resp->encoded_size = 2;
      resp->encoded = strdup(":1");
      break;
    case RESPONSE_STRING:
      resp->encoded_size = strlen(resp->text) + 1;
      resp->encoded = malloc(sizeof(char) * (resp->encoded_size + 1));
      resp->encoded[0] = '+';
      resp->encoded[resp->encoded_size] = '\0';
      strcpy(resp->encoded + 1, resp->text);
      break;
    case RESPONSE_NUMBER:
      if (resp->number == 0) {
        resp->encoded_size = 2;
        resp->encoded = strdup(":0");
      } else if (resp->number > 0) {
        resp->encoded_size = ceil(log10(resp->number)) + 1;
        resp->encoded = malloc(sizeof(char) * (resp->encoded_size + 1));
        resp->encoded[0] = ':';
        resp->encoded[resp->encoded_size] = '\0';
        itoa_r(resp->number, resp->encoded + resp->encoded_size - 1);
      } else {
        resp->encoded_size = ceil(log10(resp->number * -1)) + 2;
        resp->encoded = malloc(sizeof(char) * (resp->encoded_size + 1));
        resp->encoded[0] = ':';
        resp->encoded[1] = '-';
        resp->encoded[resp->encoded_size] = '\0';
        itoa_r(resp->number * -1, resp->encoded + resp->encoded_size - 1);
      }
      break;
    case RESPONSE_ERR_UNKNOWN_REQUEST:
    case RESPONSE_ERR_PARSING:
    case RESPONSE_ERR_ARGUMENTS:
      resp->encoded_size = ceil(log10(resp->type * -1)) + 1;
      resp->encoded = malloc(sizeof(char) * (resp->encoded_size + 1));
      resp->encoded[0] = '-';
      resp->encoded[resp->encoded_size] = '\0';
      itoa_r(resp->type * -1, resp->encoded + resp->encoded_size - 1);
      break;
  }
}

void Response_free(Response *resp) {
  assert(resp);

  if (resp->encoded) {
    free(resp->encoded);
    resp->encoded = NULL;
  }
  if ((resp->type == RESPONSE_STRING) && resp->text) {
    free(resp->text);
    resp->text = NULL;
  }
}
