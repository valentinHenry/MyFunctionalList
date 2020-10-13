CC ?= gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -fsanitize=address

TEST_LDLIBS= -lcriterion

OBJS = list.o

TEST_OBJS = test.o

all: $(OBJS)

testsuite: $(TEST_OBJS) $(OBJS)
	$(CC) $^ $(TEST_LDLIBS) $(CFLAGS) -o $@

check: testsuite
	./testsuite --verbose

clean:
	$(RM) testsuite $(TEST_OBJS) $(OBJS)

.PHONY: check clean
