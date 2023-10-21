CFLAGS = -Wall -Wextra -pedantic -I ./ -fPIC
CFLAGS_RELEASE = -O3 -DNDEBUG
CFLAGS_DEBUG = -O0 -g -DDEBUG
STATIC = ar cr $@ $^

Files := $(wildcard SA/*/*.c)

all: release

clean:
	rm -rf bin/*
	rm -rf SA/*/*.o
	rm -f /usr/local/lib/libsa.so

debug: CFLAGS += $(CFLAGS_DEBUG)
debug: bin/debug/tests

release: CFLAGS += $(CFLAGS_RELEASE)
release: bin/release/tests

bin/%/tests: tests/tests.o /usr/local/lib/libsa.so
	@mkdir -p $(@D)
	gcc -o $@ $< -lsa $(CFLAGS)

/usr/local/lib/libsa.so: $(patsubst %.c,%.o,$(Files))
	gcc -o $@ $^ -shared -lssl -lcrypto -lm $(CFLAGS)

%.o: %.c
	@mkdir -p $(@D)
	gcc -c $^ -o $@ $(CFLAGS)

.PRECIOUS: /usr/local/lib/libsa.so
