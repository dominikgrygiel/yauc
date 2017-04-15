#include <describe/describe.h>

#include "message.h"

int main(int argc, const char *argv[])
{
  describe("Message") {
    it("parses INFO messages from string") {
      char *raw = "INFO";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_INFO);
      assert_equal(msg.key_size, 0);
      assert_null(msg.key);
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("parses FLUSH messages from string") {
      char *raw = "FLUSH";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_FLUSH);
      assert_equal(msg.key_size, 0);
      assert_null(msg.key);
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("parses GET messages from string") {
      char *raw = "GET foo";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_GET);
      assert_equal(msg.key_size, 3);
      assert_str_equal(msg.key, "foo");
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("parses SET messages from string") {
      char *raw = "SET fooset bar";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_SET);
      assert_equal(msg.key_size, 6);
      assert_str_equal(msg.key, "fooset");
      assert_equal(msg.value_size, 3);
      assert_str_equal(msg.value, "bar");

      Message_free(&msg);
    }

    it("parses DEL messages from string") {
      char *raw = "DEL foodel";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_DEL);
      assert_equal(msg.key_size, 6);
      assert_str_equal(msg.key, "foodel");
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("parses INCR messages from string") {
      char *raw = "INCR fooincr";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_INCR);
      assert_equal(msg.key_size, 7);
      assert_str_equal(msg.key, "fooincr");
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("parses DECR messages from string") {
      char *raw = "DECR foodecr";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_DECR);
      assert_equal(msg.key_size, 7);
      assert_str_equal(msg.key, "foodecr");
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("parses URL messages from string") {
      char *raw = "URL http://example.com";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_URL);
      assert_equal(msg.key_size, 18);
      assert_str_equal(msg.key, "http://example.com");
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("returns an error in case of unrecognized message") {
      char *raw = "FOOBAR foo";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_ERR_PARSING);
      assert_equal(msg.key_size, 0);
      assert_null(msg.key);
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("returns an error in case of missing key") {
      char *raw = "GET  ";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_ERR_ARGUMENTS);
      assert_equal(msg.key_size, 0);
      assert_null(msg.key);
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("returns an error in case of wrong format") {
      char *raw = "GET  foo";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_ERR_ARGUMENTS);
      assert_equal(msg.key_size, 0);
      assert_null(msg.key);
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("returns an error in case of missing key value") {
      char *raw = "SET   ";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_ERR_ARGUMENTS);
      assert_equal(msg.key_size, 0);
      assert_null(msg.key);
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("returns an error in case of missing value") {
      char *raw = "SET fooset  ";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_ERR_ARGUMENTS);
      assert_equal(msg.key_size, 6);
      assert_str_equal(msg.key, "fooset");
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("returns an error in case of missing value") {
      char *raw = "SET fooset";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_ERR_ARGUMENTS);
      assert_equal(msg.key_size, 6);
      assert_str_equal(msg.key, "fooset");
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("strips too long keys") {
      char *raw = "GET aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_GET);
      assert_equal(msg.key_size, 255);
      assert_str_equal(msg.key, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("parses only untill first space") {
      char *raw = "GET foo bar";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_GET);
      assert_equal(msg.key_size, 3);
      assert_str_equal(msg.key, "foo");
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }

    it("returns an error if not whole command matched") {
      char *raw = "FLUSHfoo";
      Message msg = Message_parse(raw, strlen(raw));
      assert_equal(msg.type, MESSAGE_ERR_PARSING);
      assert_equal(msg.key_size, 0);
      assert_null(msg.key);
      assert_equal(msg.value_size, 0);
      assert_null(msg.value);

      Message_free(&msg);
    }
  }

  return assert_failures();
}
