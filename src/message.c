#include <string.h>
#include <stdlib.h>

#include "message.h"

void set_msg_key(Message *message, char *raw, int raw_size, int offset) {
  if (raw_size > offset && raw[offset] != ' ') {
    int max_key_size = raw_size - offset > MESSAGE_KEY_MAX_SIZE ? MESSAGE_KEY_MAX_SIZE : raw_size - offset;
    char *key_end = memchr(raw + offset, ' ', max_key_size);

    if (key_end) {
      int key_size = key_end - raw - offset;
      message->key = strndup(raw + offset, key_size);
      message->key_size = key_size;
    } else {
      message->key = strndup(raw + offset, max_key_size);
      message->key_size = max_key_size;
    }
  } else {
    message->type = MESSAGE_ERR_ARGUMENTS;
    message->key_size = 0;
  }
}

void set_msg_value(Message *message, char *raw, int raw_size, int offset) {
  if (message->type <= 0) return;

  int value_offset = offset + message->key_size + 1;
  if (value_offset < raw_size && raw[value_offset] != ' ') {
    int value_size = raw_size - value_offset;

    message->value = strndup(raw + value_offset, value_size);
    message->value_size = value_size;
  } else {
    message->type = MESSAGE_ERR_ARGUMENTS;
    message->value_size = 0;
  }
}

Message Message_parse(char *raw, int raw_size) {
  Message message = {.key = NULL, .key_size = 0, .value = NULL, .value_size = 0};

  if (strncmp(raw, "GET ", 4) == 0) {
    message.type = MESSAGE_GET;
    set_msg_key(&message, raw, raw_size, 4);
  } else if (strncmp(raw, "SET ", 4) == 0) {
    message.type = MESSAGE_SET;
    set_msg_key(&message, raw, raw_size, 4);
    set_msg_value(&message, raw, raw_size, 4);
  } else if (strncmp(raw, "DEL ", 4) == 0) {
    message.type = MESSAGE_DEL;
    set_msg_key(&message, raw, raw_size, 4);
  } else if (strncmp(raw, "INCR ", 5) == 0) {
    message.type = MESSAGE_INCR;
    set_msg_key(&message, raw, raw_size, 5);
  } else if (strncmp(raw, "DECR ", 5) == 0) {
    message.type = MESSAGE_DECR;
    set_msg_key(&message, raw, raw_size, 5);
  } else if (strncmp(raw, "URL ", 4) == 0) {
    message.type = MESSAGE_URL;
    set_msg_key(&message, raw, raw_size, 4);
  } else if ((raw_size == 5) && (strncmp(raw, "FLUSH", 5) == 0)) {
    message.type = MESSAGE_FLUSH;
  } else if ((raw_size == 4) && (strncmp(raw, "INFO", 4) == 0)) {
    message.type = MESSAGE_INFO;
  } else {
    message.type = MESSAGE_ERR_PARSING;
  }

  return message;
}

void Message_free(Message *msg) {
  if (msg) {
    if (msg->key)
      free(msg->key);
    if (msg->value)
      free(msg->value);
  }
}
