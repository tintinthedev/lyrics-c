#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <curl/curl.h>
#include <cJSON.h>
#include "http.h"
#include "lyrics.h"

static char *build_url(const char *artist, const char *title) {
    CURL *curl = curl_easy_init();
    if (!curl)
        return NULL;

    char *enc_artist = curl_easy_escape(curl, artist, 0);
    char *enc_title = curl_easy_escape(curl, title, 0);
    if (!enc_artist || !enc_title) {
        curl_free(enc_artist);
        curl_free(enc_title);
        curl_easy_cleanup(curl);
        return NULL;
    }

    size_t url_len = strlen("https://lrclib.net/api/get?artist_name=")
                   + strlen(enc_artist)
                   + strlen("&track_name=")
                   + strlen(enc_title)
                   + 1;

    char *url = malloc(url_len);
    if (url)
        snprintf(url, url_len, "https://lrclib.net/api/get?artist_name=%s&track_name=%s",
                 enc_artist, enc_title);

    curl_free(enc_artist);
    curl_free(enc_title);
    curl_easy_cleanup(curl);

    return url;
}

char *fetch_lyrics(const char *artist, const char *title) {
    char *url = build_url(artist, title);
    if (!url)
        return NULL;

    char *response = http_get(url);
    free(url);

    if (!response)
        return NULL;

    cJSON *json = cJSON_Parse(response);
    free(response);

    if (!json)
        return NULL;

    cJSON *lyrics = cJSON_GetObjectItem(json, "plainLyrics");
    if (!cJSON_IsString(lyrics)) {
        cJSON_Delete(json);
        return NULL;
    }

    char *result = strdup(lyrics->valuestring);
    cJSON_Delete(json);

    return result;
}
