CC = gcc
CFLAGS = -Wall -Wextra -I/usr/local/include
LDFLAGS = -lcurl /usr/local/lib/libcjson.a

SRCS = main.c http.c lyrics.c
OBJS = $(SRCS:.c=.o)
TARGET = lyrics

SIZE_CFLAGS = -Os -s \
	-fdata-sections -ffunction-sections \
	-fno-unwind-tables -fno-asynchronous-unwind-tables \
	-fomit-frame-pointer
SIZE_LDFLAGS = -Wl,--gc-sections -Wl,--strip-all

.PHONY: all small small-static portable install-deps clean

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

small: $(SRCS)
	$(CC) $(CFLAGS) $(SIZE_CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS) $(SIZE_LDFLAGS)

small-static: $(SRCS)
	$(CC) $(CFLAGS) $(SIZE_CFLAGS) -o $(TARGET) $(SRCS) $(SIZE_LDFLAGS) \
		/usr/local/lib/libcjson.a -Wl,-Bstatic -lcurl -Wl,-Bdynamic \
		-lnghttp2 -lidn2 -lrtmp -lssh -lpsl \
		-lssl -lcrypto -lgssapi_krb5 -lldap -llber \
		-lzstd -lbrotlidec -lz

portable: $(SRCS)
	$(CC) $(CFLAGS) $(SIZE_CFLAGS) -static -o $(TARGET) $(SRCS) $(SIZE_LDFLAGS) \
		/usr/local/lib/libcjson.a -lcurl \
		-lnghttp2 -lidn2 -lrtmp -lssh -lpsl \
		-lssl -lcrypto -lgssapi_krb5 -lldap -llber \
		-lzstd -lbrotlidec -lz

install-deps:
	@echo "Installing dependencies for fully static build..."
	sudo apt install -y \
		libssl-dev libnghttp2-dev libgnutls28-dev libssh-dev \
		libbrotli-dev libzstd-dev libidn2-dev libunistring-dev \
		libpsl-dev librtmp-dev libldap-dev libsasl2-dev libkrb5-dev
	@echo "Done. Run 'make portable' to build."

clean:
	rm -f $(TARGET) $(OBJS)
