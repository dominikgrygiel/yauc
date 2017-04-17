#include <stdlib.h>
#include <string.h>

#include "response.h"

Response *Response_new() {
  Response *resp = malloc(sizeof(Response));

  resp->type = 0;
  resp->encoded = NULL;
  resp->encoded_size = 0;

  return resp;
}

void Response_encode(Response *resp) {
  switch (resp->type) {
    case RESPONSE_OK:
      resp->encoded = strdup(":1");
      resp->encoded_size = 2;
      break;
  }
}

void Response_free(Response *resp) {
  if (resp) {
    if (resp->encoded) {
      free(resp->encoded);
      resp->encoded = NULL;
    }
  }
}
