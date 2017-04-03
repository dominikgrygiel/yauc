#include <describe/describe.h>

#include "message.h"

int main(int argc, const char *argv[])
{
  describe("Message") {
    it("parses from string") {
      Message msg = Message_parse("GET foo");
      assert_equal(msg.type, MESSAGE_GET);
    }
  }

  return assert_failures();
}
