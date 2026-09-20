#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lyrics.h"

static char *read_line(const char *prompt) {
    printf("%s", prompt);

    char *line = NULL;
    size_t len = 0;
    ssize_t nread = getline(&line, &len, stdin);

    if (nread <= 0) {
        free(line);
        return NULL;
    }

    if (line[nread - 1] == '\n')
        line[nread - 1] = '\0';

    return line;
}

int main(void) {
    char *title = read_line("Song name: ");
    if (!title) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }

    char *artist = read_line("Artist name: ");
    if (!artist) {
        fprintf(stderr, "Error reading input.\n");
        free(title);
        return 1;
    }

    if (strlen(title) == 0 || strlen(artist) == 0) {
        fprintf(stderr, "Error: song name and artist cannot be empty.\n");
        free(title);
        free(artist);
        return 1;
    }

    printf("\nSearching for \"%s\" by %s...\n\n", title, artist);

    char *lyrics = fetch_lyrics(artist, title);

    if (lyrics) {
        printf("--- Lyrics ---\n\n%s\n", lyrics);
        free(lyrics);
    } else {
        printf("Lyrics not found. Try a different song or artist.\n");
    }

    free(title);
    free(artist);

    return 0;
}
