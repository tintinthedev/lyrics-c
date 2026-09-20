# lyrics-c

A small command-line tool that fetches song lyrics. Enter a song name and artist, get the lyrics back.

<img width="500" height="300" alt="image" src="https://github.com/user-attachments/assets/26175af8-267a-4af2-ad44-334062b31760" />

## A note for developers (ignore if you're a normal person)
"Why did you use C?" because AI wrote this code, so if AI does the heavy lifting (on this case, writing the low level code), why would I use a language that naturally bloats my application?

## Download

Go to [Releases](https://github.com/tintinthedev/lyrics-c/releases) and download the latest binary for your platform.

Then make it executable and run:

```bash
chmod +x lyrics
./lyrics
```

## Usage

1. Run the binary.
2. Type the **song name** and press Enter.
3. Type the **artist name** and press Enter.
4. The lyrics are printed to stdout.

To quit, press `Ctrl+C`.

**Requirement:** `libcurl` must be installed on your system. It comes pre-installed on most Linux distributions and macOS.

- **Debian/Ubuntu:** `sudo apt install libcurl4`
- **Fedora/RHEL:** `sudo dnf install libcurl`
- **Arch:** `sudo pacman -S curl`
- **macOS:** pre-installed (ships with the OS)

## Building from Source

### Requirements

- GCC (or any C99-compatible compiler)
- libcurl development files (`libcurl4-openssl-dev` on Debian/Ubuntu)
- cJSON (static library, linked at build time)

On Debian/Ubuntu, install the build dependencies with:

```bash
sudo apt install build-essential libcurl4-openssl-dev libcjson-dev
```

### Compilation Modes

#### `make all` (default)

Standard build with debug symbols. Links cJSON statically.

```bash
make
```

#### `make small`

Size-optimized build. Strips symbols, removes unwind tables, and garbage-collects unused code sections. Produces a smaller binary with no debug info.

```bash
make small
```

#### `make small-static`

Statically links libcurl in addition to cJSON. Reduces runtime dependencies further, but still requires some system libraries (OpenSSL, nghttp2, etc.).

```bash
make small-static
```

#### `make portable`

Fully static build. Produces a self-contained binary with zero runtime library dependencies. Requires additional `-dev` packages for all of libcurl's transitive dependencies.

```bash
make install-deps   # install required -dev packages (Debian/Ubuntu)
make portable
```

#### `make clean`

Removes the compiled binary and object files.

```bash
make clean
```

### Build Target Summary

| Target | cJSON | libcurl | Other deps | Binary size |
|---|---|---|---|---|
| `all` | static | dynamic | dynamic | ~57K |
| `small` | static | dynamic | dynamic | ~39K |
| `small-static` | static | static | dynamic | ~500K |
| `portable` | static | static | static | ~5-10MB |


## API

This project uses the [LRCLIB](https://lrclib.net) API -- a free, open, no-auth-required lyrics database. Lyrics are fetched via:

```
GET https://lrclib.net/api/get?artist_name={artist}&track_name={title}
```

The response is JSON containing a `plainLyrics` field with the song lyrics.
