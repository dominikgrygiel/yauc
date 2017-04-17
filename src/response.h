#ifndef RESPONSE_H_INCLUDED
#define RESPONSE_H_INCLUDED

enum RESPONSE_TYPES {
  RESPONSE_OK = 1,
  RESPONSE_STRING,
  RESPONSE_INTEGER,

  // ERRORS
  RESPONSE_ERR_UNKNOWN_REQUEST = -30,

  // REQUEST ERRORS
  RESPONSE_ERR_PARSING = -40,
  RESPONSE_ERR_ARGUMENTS = -41
};

typedef struct ResponseStruct {
  enum RESPONSE_TYPES type;
  char *encoded;
  int encoded_size;
} Response;

Response *Response_new();
void Response_encode(Response *resp);
void Response_free(Response *resp);

#endif
