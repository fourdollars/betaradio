/**
 * Copyright (c) 2007 Shih-Yuan Lee (FourDollars) <fourdollars@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <curl/curl.h>
#include "channel.h"

#define RADIO_URL "http://hichannel.hinet.net/api/streamFreeRadio.jsp?id="
#define LIVE_URL "http://hichannel.hinet.net/api/streamFreeLive.jsp?id="

struct FileStruct {
	FILE *stream;
	size_t size;
	size_t nmemb;
};

static int my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct FileStruct *out = (struct FileStruct *) stream;

	if (out && !out->stream) {
		out->stream = tmpfile();
		if (!out->stream)
			return -1;
	}

	out->size = size;
	out->nmemb = nmemb;

	return fwrite(buffer, size, nmemb, out->stream);
}

static void wmp_init(void)
{
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

static void wmp_fini(void)
{
	curl_global_cleanup();
}

static char *wmp_get_content(const char *url)
{
	CURL *curl;
	CURLcode res;
	char *content = NULL;
	struct FileStruct file = {
		NULL, 0, 0
	};

	curl = curl_easy_init();

	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_USERAGENT, "Windows Media Player");
		curl_easy_setopt(curl, CURLOPT_REFERER, "http://hichannel.hinet.net");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if (CURLE_OK != res) {
			fprintf(stderr, "curl error number %d\n", res);
		}
	}

	if (file.size != 0 && file.nmemb != 0) {
		content = (char *) malloc(file.size * file.nmemb + 1);
		bzero(content, file.size * file.nmemb + 1);
		rewind(file.stream);
		fread(content, file.size, file.nmemb, file.stream);
	}

	if (file.stream)
		fclose(file.stream);

	return content;
}

static char *fetch_url(const char *keyword, char* content)
{
	char *begin = NULL;
	char *end = NULL;
	char *url = NULL;

	begin = strstr(content, keyword);

	if (begin == NULL)
		return NULL;

	end = strstr(begin, "\"");

	url = (char *) malloc(end - begin + 1);
	bzero(url, end - begin + 1);
	strncpy(url, begin, end - begin);

	return url;
}

char *get_channel_url_by_id(const char *id, int bLive)
{
	char *url = NULL;
	char *content = NULL;
	char *mms = NULL;
	
	if (bLive) {
		url = (char *) malloc(strlen(LIVE_URL) + 4);
		bzero(url, strlen(LIVE_URL) + 4);
		sprintf(url, LIVE_URL "%s", id);
	} else {
		url = (char *) malloc(strlen(RADIO_URL) + 4);
		bzero(url, strlen(RADIO_URL) + 4);
		sprintf(url, RADIO_URL "%s", id);
	}

	wmp_init();
	
	content = wmp_get_content(url);
	free(url);

	if (content != NULL) {
		url = fetch_url("http", content);
	} else {
		fprintf(stderr, "Can't get content from %s.\n", url);
		wmp_fini();
		return NULL;
	}

	if (content != NULL && url != NULL) {
		free(content);
		content = wmp_get_content(url);
		if (content == NULL) {
			fprintf(stderr, "Can't get content from %s.\n", url);
			free(url);
			wmp_fini();
			return NULL;
		}
		free(url);
		mms = fetch_url("mms", content);
		if (mms == NULL)
			fprintf(stderr, "Can't get mms from %s.\n", url);
		free(content);
	}

	wmp_fini();

	return mms;
}
