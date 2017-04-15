# YAUC

Yet Another Useless Cache.

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

### Using (guard)[https://github.com/guard/guard] to automatically run tests

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
