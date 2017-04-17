#include <describe/describe.h>

#include "request.h"

int main(int argc, const char *argv[])
{
  describe("Request") {
    it("parses INFO requests from string") {
      char *raw = "INFO";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_INFO);
      assert_equal(req->key_size, 0);
      assert_null(req->key);
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("parses FLUSH requests from string") {
      char *raw = "FLUSH";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_FLUSH);
      assert_equal(req->key_size, 0);
      assert_null(req->key);
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("parses GET requests from string") {
      char *raw = "GET foo";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_GET);
      assert_equal(req->key_size, 3);
      assert_str_equal(req->key, "foo");
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("parses SET requests from string") {
      char *raw = "SET fooset bar";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_SET);
      assert_equal(req->key_size, 6);
      assert_str_equal(req->key, "fooset");
      assert_equal(req->value_size, 3);
      assert_str_equal(req->value, "bar");

      Request_free(req);
    }

    it("parses DEL requests from string") {
      char *raw = "DEL foodel";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_DEL);
      assert_equal(req->key_size, 6);
      assert_str_equal(req->key, "foodel");
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("parses INCR requests from string") {
      char *raw = "INCR fooincr";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_INCR);
      assert_equal(req->key_size, 7);
      assert_str_equal(req->key, "fooincr");
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("parses DECR requests from string") {
      char *raw = "DECR foodecr";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_DECR);
      assert_equal(req->key_size, 7);
      assert_str_equal(req->key, "foodecr");
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("parses URL requests from string") {
      char *raw = "URL http://example.com";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_URL);
      assert_equal(req->key_size, 18);
      assert_str_equal(req->key, "http://example.com");
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("returns an error in case of unrecognized request") {
      char *raw = "FOOBAR foo";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_ERR_PARSING);
      assert_equal(req->key_size, 0);
      assert_null(req->key);
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("returns an error in case of missing key") {
      char *raw = "GET  ";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_ERR_ARGUMENTS);
      assert_equal(req->key_size, 0);
      assert_null(req->key);
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("returns an error in case of wrong format") {
      char *raw = "GET  foo";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_ERR_ARGUMENTS);
      assert_equal(req->key_size, 0);
      assert_null(req->key);
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("returns an error in case of missing key value") {
      char *raw = "SET   ";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_ERR_ARGUMENTS);
      assert_equal(req->key_size, 0);
      assert_null(req->key);
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("returns an error in case of missing value") {
      char *raw = "SET fooset  ";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_ERR_ARGUMENTS);
      assert_equal(req->key_size, 6);
      assert_str_equal(req->key, "fooset");
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("returns an error in case of missing value") {
      char *raw = "SET fooset";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_ERR_ARGUMENTS);
      assert_equal(req->key_size, 6);
      assert_str_equal(req->key, "fooset");
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("strips too long keys") {
      char *raw = "GET aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_GET);
      assert_equal(req->key_size, 255);
      assert_str_equal(req->key, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("parses only untill first space") {
      char *raw = "GET foo bar";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_GET);
      assert_equal(req->key_size, 3);
      assert_str_equal(req->key, "foo");
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("returns an error if not whole command matched") {
      char *raw = "FLUSHfoo";
      Request *req = Request_new();
      Request_parse(req, raw, strlen(raw));
      assert_equal(req->type, REQUEST_ERR_PARSING);
      assert_equal(req->key_size, 0);
      assert_null(req->key);
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }

    it("returns an error for short requests") {
      char *raw = "GET a";
      Request *req = Request_new();
      Request_parse(req, raw, 1);
      assert_equal(req->type, REQUEST_ERR_PARSING);
      assert_equal(req->key_size, 0);
      assert_null(req->key);
      assert_equal(req->value_size, 0);
      assert_null(req->value);

      Request_free(req);
    }
  }

  return assert_failures();
}
