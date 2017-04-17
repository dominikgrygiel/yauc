#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED

#define REQUEST_KEY_MAX_SIZE 255

enum REQUEST_TYPES {
  REQUEST_INFO = 1,
  REQUEST_FLUSH,
  REQUEST_GET,
  REQUEST_SET,
  REQUEST_DEL,
  REQUEST_INCR,
  REQUEST_DECR,
  REQUEST_URL,

  // ERRORS
  REQUEST_ERR_PARSING = -40,
  REQUEST_ERR_ARGUMENTS = -41
};

typedef struct RequestStruct {
  enum REQUEST_TYPES type;
  char *key;
  int key_size;
  char *value;
  int value_size;
} Request;


Request *Request_new();
void Request_parse(Request *req, char *raw, int raw_size);
void Request_free(Request *req);

#endif
