#include <describe/describe.h>

#include "request.h"
#include "response.h"

int main(int argc, const char *argv[])
{
  describe("Response") {
    it("encodes OK responses") {
      Response *resp = Response_new();
      resp->type = RESPONSE_OK;
      Response_encode(resp);

      assert_str_equal(resp->encoded, ":1");
      assert_equal(resp->encoded_size, 2);

      Response_free(resp);
    }

    it("encodes empty responses") {
      Response *resp = Response_new();
      resp->type = RESPONSE_EMPTY;
      Response_encode(resp);

      assert_str_equal(resp->encoded, ":0");
      assert_equal(resp->encoded_size, 2);

      Response_free(resp);
    }

    it("encodes string repsonses") {
      Response *resp = Response_new_string("Hello World!");
      Response_encode(resp);

      assert_str_equal(resp->encoded, "+Hello World!");
      assert_equal(resp->encoded_size, 13);

      Response_free(resp);
    }

    it("encodes integer responses") {
      Response *resp = Response_new_number(42);
      Response_encode(resp);

      assert_str_equal(resp->encoded, ":42");
      assert_equal(resp->encoded_size, 3);

      Response_free(resp);
    }

    it("encodes integer responses when number = 1") {
      Response *resp = Response_new_number(1);
      Response_encode(resp);

      assert_str_equal(resp->encoded, ":1");
      assert_equal(resp->encoded_size, 2);

      Response_free(resp);
    }

    it("encodes integer responses when number = 0") {
      Response *resp = Response_new_number(0);
      Response_encode(resp);

      assert_str_equal(resp->encoded, ":0");
      assert_equal(resp->encoded_size, 2);

      Response_free(resp);
    }

    it("encodes integer responses when number = -1") {
      Response *resp = Response_new_number(-1);
      Response_encode(resp);

      assert_str_equal(resp->encoded, ":-1");
      assert_equal(resp->encoded_size, 3);

      Response_free(resp);
    }

    it("encodes negative integer responses") {
      Response *resp = Response_new_number(-42);
      Response_encode(resp);

      assert_str_equal(resp->encoded, ":-42");
      assert_equal(resp->encoded_size, 4);

      Response_free(resp);
    }

    it("encodes RESPONSE_ERR_UNKNOWN_REQUEST responses") {
      Response *resp = Response_new();
      resp->type = RESPONSE_ERR_UNKNOWN_REQUEST;
      Response_encode(resp);

      assert_str_equal(resp->encoded, "-30");
      assert_equal(resp->encoded_size, 3);

      Response_free(resp);
    }

    it("encodes RESPONSE_ERR_PARSING responses") {
      Response *resp = Response_new();
      resp->type = RESPONSE_ERR_PARSING;
      Response_encode(resp);

      assert_str_equal(resp->encoded, "-40");
      assert_equal(resp->encoded_size, 3);

      Response_free(resp);
    }

    it("encodes RESPONSE_ERR_ARGUMENTS responses") {
      Response *resp = Response_new();
      resp->type = RESPONSE_ERR_ARGUMENTS;
      Response_encode(resp);

      assert_str_equal(resp->encoded, "-41");
      assert_equal(resp->encoded_size, 3);

      Response_free(resp);
    }

    it("has same error codes as request") {
      assert_equal(RESPONSE_ERR_PARSING, (int)REQUEST_ERR_PARSING);
      assert_equal(RESPONSE_ERR_ARGUMENTS, (int)REQUEST_ERR_ARGUMENTS);
    }
  }

  return assert_failures();
}
