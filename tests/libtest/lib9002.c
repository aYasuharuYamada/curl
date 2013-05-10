/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2011, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at http://curl.haxx.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/
#include "test.h"

#include <curl/mprintf.h>

#include "memdebug.h"

static const char *HOSTHEADER = "Host: www.host.foo.com";

/* build host entry */
static struct curl_slist *sethost(struct curl_slist *headers)
{
  (void)headers;
  return curl_slist_append(NULL, HOSTHEADER);
}

static char *suburl(const char *base, int i)
{
  return curl_maprintf("%s%.4d", base, i);
}

/* test function */
int test(char *URL)
{
  int res = TEST_ERR_MAJOR_BAD;
  CURLcode code = CURLE_OK;
  CURL *curl = NULL;
  struct curl_slist *headers;
  char *url;

  if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
    goto test_cleanup;
  }

  if((curl = curl_easy_init()) == NULL) {
    goto test_cleanup;
  }


  headers = sethost(NULL);
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

  curl_easy_setopt(curl, CURLOPT_COOKIEJAR, "log/jar9002.txt");

  curl_easy_setopt(curl, CURLOPT_MAXCOOKIES, 5);

  url = suburl(URL, 1);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_free(url);
  code = curl_easy_perform(curl);
  if(CURLE_OK != code) {
    goto test_cleanup;
  }

  sleep(1);

  url = suburl(URL, 2);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_free(url);
  code = curl_easy_perform(curl);
  if(CURLE_OK != code) {
    goto test_cleanup;
  }

  sleep(1);

  url = suburl(URL, 1);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_free(url);
  code = curl_easy_perform(curl);
  if(CURLE_OK != code) {
    goto test_cleanup;
  }

  sleep(1);

  url = suburl(URL, 3);
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_free(url);
  code = curl_easy_perform(curl);
  if(CURLE_OK != code) {
    goto test_cleanup;
  }

  res = 0;

test_cleanup:
  if(headers) curl_slist_free_all(headers);
  if(curl) curl_easy_cleanup(curl);
  curl_global_cleanup();

  return res;
}

