#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnewline-eof"
#pragma GCC diagnostic ignored "-Wextra-semi"
#include <hash/hash.h>
#pragma GCC diagnostic pop

typedef struct DatabaseStruct {
  hash_t *store;
} Database;

Database *Database_new();
char *Database_info(Database *db);
void Database_flush(Database *db);
char *Database_get(Database *db, char *key);
void Database_set(Database *db, char *key, char *value);
void Database_del(Database *db, char *key);
int Database_incr(Database *db, char *key);
int Database_decr(Database *db, char *key);
void Database_free(Database *db);

#endif
