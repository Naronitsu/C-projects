/*===============================================================
  file_io_notes.c  —  Revision notes: FILES in C (POSIX + stdio)
  ===============================================================

  BIG PICTURE
  - Two common layers:
    (1) stdio: FILE* + fopen/fread/fwrite/fprintf...
        - buffered, convenient, portable
    (2) POSIX: file descriptor (int fd) + open/read/write/lseek...
        - unbuffered (you control buffering), closer to OS

  WHEN TO USE WHICH?
  - stdio (FILE*): text processing, easy formatting, portability.
  - POSIX (fd): networking-style streams, precise control, performance,
               select/poll/epoll integration, atomic append, file locks.

  COMMON PITFALLS
  - Forgetting to close => resource leaks, missing flush.
  - Mixing stdio FILE* and fd operations on same underlying file
    without extreme care => buffer confusion.
  - Not checking return values (short reads/writes happen).
  - Assuming write() writes everything in one go (it may not).
  - Using feof() wrong: feof becomes true only AFTER a read fails.

  ---------------------------------------------------------------
  STDIO CHEAT SHEET
  ---------------------------------------------------------------
  fopen(path, mode)
    modes:
      "r"  read text, must exist
      "w"  write text, truncates/creates
      "a"  append text, creates
      "rb" "wb" "ab" for binary
      "r+" read/write (must exist)
      "w+" read/write (trunc/create)
      "a+" read/append

  fread/fwrite:
    size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
    returns number of *items* read (nmemb), not bytes.

  fprintf/fscanf:
    formatted I/O; beware scanf pitfalls and whitespace.

  fgets:
    safest for line input:
      char *fgets(char *s, int size, FILE *stream);
    keeps newline if it fits; returns NULL on EOF/error.

  fseek/ftell/rewind:
    for random access in *seekable* streams.

  fflush:
    flushes output buffer; for input streams, behavior differs by platform;
    don’t rely on fflush(stdin).

  ---------------------------------------------------------------
  POSIX FILE DESCRIPTOR CHEAT SHEET (UNIX-LIKE)
  ---------------------------------------------------------------
  open:
    int open(const char *path, int flags, mode_t mode_optional);
    flags:
      O_RDONLY, O_WRONLY, O_RDWR
      O_CREAT (needs mode e.g. 0644)
      O_TRUNC, O_APPEND
      O_NONBLOCK (useful sometimes)
    returns fd >= 0 or -1 (errno set)

  read/write:
    ssize_t read(int fd, void *buf, size_t count);
    ssize_t write(int fd, const void *buf, size_t count);

    - read can return:
        >0 bytes read
         0 => EOF
        -1 => error (errno)
    - write can be short; loop until all bytes are written.

  lseek:
    off_t lseek(int fd, off_t offset, int whence); // SEEK_SET/CUR/END
    - not valid on pipes/sockets.

  close:
    close(fd);

  errno + perror/strerror:
    on error, check errno; use perror("label") for quick print.

  ---------------------------------------------------------------
  SHORT-READ/SHORT-WRITE LOOP PATTERNS
  ---------------------------------------------------------------
  - robust read fully (for fixed-size blocks) and robust write fully.
  - for text lines, prefer fgets (stdio) or implement line buffering.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef __unix__
#include <fcntl.h>
#include <unistd.h>
#endif

/* Robustly write all bytes (POSIX). Returns 0 on success, -1 on error. */
#ifdef __unix__
static int write_all(int fd, const void *buf, size_t n) {
    const unsigned char *p = (const unsigned char *)buf;
    size_t total = 0;
    while (total < n) {
        ssize_t w = write(fd, p + total, n - total);
        if (w < 0) {
            if (errno == EINTR) continue; // interrupted by signal; retry
            return -1;
        }
        total += (size_t)w;
    }
    return 0;
}
#endif

int main(void) {
    /* Example: stdio write + read */
    {
        FILE *f = fopen("example.txt", "w");
        if (!f) {
            perror("fopen write");
            return 1;
        }
        fprintf(f, "Hello files!\nNumber: %d\n", 42);
        fclose(f); // flushes and closes
    }

    {
        FILE *f = fopen("example.txt", "r");
        if (!f) {
            perror("fopen read");
            return 1;
        }
        char line[256];
        while (fgets(line, sizeof line, f)) {
            fputs(line, stdout);
        }
        if (ferror(f)) {
            perror("read error");
        }
        fclose(f);
    }

#ifdef __unix__
    /* Example: POSIX open/write/read */
    {
        int fd = open("example2.bin", O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            perror("open");
            return 1;
        }
        const char payload[] = "raw-bytes\n";
        if (write_all(fd, payload, sizeof payload - 1) < 0) {
            perror("write_all");
            close(fd);
            return 1;
        }
        close(fd);
    }
#endif

    return 0;
}
