#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

enum {
  MESSAGE_GET = 1
};

typedef struct MessageStruct {
  char type;
} Message;


Message Message_parse(char *);

#endif
