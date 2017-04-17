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

    it("has same error codes as request") {
      assert_equal(RESPONSE_ERR_PARSING, (int)REQUEST_ERR_PARSING);
      assert_equal(RESPONSE_ERR_ARGUMENTS, (int)REQUEST_ERR_ARGUMENTS);
    }
  }

  return assert_failures();
}
