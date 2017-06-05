#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>

#include "database.h"
#include "util.h"

Database *Database_new() {
  Database *db = malloc(sizeof(Database));

  db->store = hash_new();
  db->data_log = NULL;

  return db;
}

Database *Database_new_with_log(char *data_log_path) {
  Database *db = Database_new();
  FILE *log_file = NULL;

  if ((log_file = fopen(data_log_path, "r"))) {
    // TODO: read log file
    fclose(log_file);
  }

  if ((log_file = fopen(data_log_path, "wa"))) {
    db->data_log = log_file;
  } else {
    fprintf(stderr, "Couldn't open log file!\n");
    exit(2);
  }

  return db;
}

char *Database_get(Database *db, char *key) {
  assert(db);
  assert(key);

  return hash_get(db->store, key);
}

void Database_set(Database *db, char *key, char *value) {
  assert(db);
  assert(key);
  assert(value);

  hash_set(db->store, strdup(key), strdup(value));
}

int database_incr_by(Database *db, char *key, int by) {
  char *current = Database_get(db, key);
  int current_number = 0;

  if (current != NULL) {
    current_number = atoi(current);
  }
  current_number += by;

  char *encoded = NULL;
  if (current_number == 0) {
    encoded = strdup("0");
  } else if (current_number == 1) {
    encoded = strdup("1");
  } else if (current_number == -1) {
    encoded = strdup("-1");
  } else {
    int encoded_size = 1;
    if (current_number > 0) {
      encoded_size = ceil(log10(current_number));
    } else {
      encoded_size = ceil(log10(current_number * -1)) + 1;
    }

    encoded = malloc(sizeof(char) * (encoded_size + 1));
    encoded[encoded_size] = '\0';
    if (current_number > 0) {
      itoa_r(current_number, encoded + encoded_size - 1);
    } else {
      encoded[0] = '-';
      itoa_r(current_number * -1, encoded + encoded_size - 1);
    }
  }
  Database_set(db, key, encoded);

  return current_number;
}

int Database_incr(Database *db, char *key) {
  assert(db);
  assert(key);

  return database_incr_by(db, key, 1);
}

int Database_decr(Database *db, char *key) {
  assert(db);
  assert(key);

  return database_incr_by(db, key, -1);
}

void Database_del(Database *db, char *key) {
  assert(db);
  assert(key);

  hash_del(db->store, key);
}

void Database_flush(Database *db) {
  assert(db);

  hash_clear(db->store);
}

void Database_free(Database *db) {
  assert(db);

  if (db->data_log) {
    fclose(db->data_log);
    db->data_log = NULL;
  }
  if (db->store) {
    hash_free(db->store);
    db->store = NULL;
  }
}
