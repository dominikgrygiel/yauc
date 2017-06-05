#include <describe/describe.h>

#include "database.h"

int main(int argc, const char *argv[])
{
  describe("Database") {
    it("returns empty response for non-existent keys") {
      Database *db = Database_new();

      assert_null(Database_get(db, "foo"));

      Database_free(db);
    }

    it("returns value of existing key") {
      Database *db = Database_new();
      Database_set(db, "somekey", "somevalue");

      assert_str_equal(Database_get(db, "somekey"), "somevalue");

      Database_free(db);
    }

    it("deletes non-existing keys") {
      Database *db = Database_new();

      Database_del(db, "foo");
      assert_null(Database_get(db, "foo"));

      Database_free(db);
    }

    it("deletes existing keys") {
      Database *db = Database_new();

      Database_set(db, "foo", "bar");
      assert_str_equal(Database_get(db, "foo"), "bar");
      Database_del(db, "foo");
      assert_null(Database_get(db, "foo"));

      Database_free(db);
    }

    it("flushes database") {
      Database *db = Database_new();

      Database_set(db, "foo", "bar");
      Database_set(db, "bar", "baz");
      assert_str_equal(Database_get(db, "foo"), "bar");
      assert_str_equal(Database_get(db, "bar"), "baz");

      Database_flush(db);
      assert_null(Database_get(db, "foo"));
      assert_null(Database_get(db, "bar"));

      Database_free(db);
    }

    it("increments non-existing keys") {
      Database *db = Database_new();

      assert_equal(Database_incr(db, "foo"), 1);
      assert_equal(Database_incr(db, "foo"), 2);
      assert_equal(Database_incr(db, "foo"), 3);

      Database_free(db);
    }

    it("decrements non-existing keys") {
      Database *db = Database_new();

      assert_equal(Database_decr(db, "foo"), -1);
      assert_equal(Database_decr(db, "foo"), -2);
      assert_equal(Database_decr(db, "foo"), -3);

      assert_equal(Database_incr(db, "bar"), 1);
      assert_equal(Database_incr(db, "bar"), 2);
      assert_equal(Database_incr(db, "bar"), 3);
      assert_equal(Database_decr(db, "bar"), 2);
      assert_equal(Database_decr(db, "bar"), 1);
      assert_equal(Database_decr(db, "bar"), 0);

      Database_free(db);
    }

  }

  return assert_failures();
}
