# YAUC

Yet Another Useless Cache.

**NOTE:** Just a toy project, not intended for production use.

## Operation

```
$ telnet 127.0.0.1 4040
Trying 127.0.0.1...
Connected to localhost.
Escape character is '^]'.
GET a
:0
SET a test
:1
GET a
+test
DEL a
:1
GET a
:0
INCR i
:1
INCR i
:2
INCR i
:3
GET i
+3
DECR i
:2
GET i
+2
WRONG OPERATION
-40
GET
-40
SET a
-41
FLUSH
:1
GET i
:0
INFO
+Starting YAUC (Yet Another Useless Cache)

^]
telnet> Connection closed.
```

## Development

### Required packages

* [clib](https://github.com/clibs/clib)

### Installing dependencies

```bash
clib install -d
```

### Running test suite

```bash
make test
```

### Using [guard](https://github.com/guard/guard) to automatically run tests

Install guard:

```bash
gem install bundler
bundle install
```

Run guard:

```bash
bundle exec guard
```

### Checking for memory leaks

To check for memory leaks you need to build the project with `-g` flag. You can do it by running build
command like this:

```bash
CFLAGS=-g make build
```

Next you can use (valgrind)[http://valgrind.org/] to help you:

```bash
valgrind --leak-check=full ./yauc
```
