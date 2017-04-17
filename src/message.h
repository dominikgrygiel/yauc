#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#define MESSAGE_KEY_MAX_SIZE 255

enum MESSAGE_TYPES {
  MESSAGE_OK = 1,
  MESSAGE_INFO,
  MESSAGE_FLUSH,
  MESSAGE_GET,
  MESSAGE_SET,
  MESSAGE_DEL,
  MESSAGE_INCR,
  MESSAGE_DECR,
  MESSAGE_URL,

  // ERRORS
  MESSAGE_ERR_PARSING = -40,
  MESSAGE_ERR_ARGUMENTS = -41
};

typedef struct MessageStruct {
  enum MESSAGE_TYPES type;
  char *key;
  int key_size;
  char *value;
  int value_size;
} Message;


Message *Message_new();
void Message_parse(Message *msg, char *raw, int raw_size);
void Message_free(Message *msg);

#endif
