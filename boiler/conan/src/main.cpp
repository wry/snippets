#include <stdio.h>
#include <curl/curl.h>

int main()
{
  curl_global_init(CURL_GLOBAL_DEFAULT);

  CURL *curl = curl_easy_init();
  if (curl)
  {
    struct curl_slist *list = curl_slist_append(list, "user-agent: libcurl");

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
    curl_easy_setopt(curl, CURLOPT_URL, "https://api.github.com/repos/conan-io/conan/stargazers");

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl);
  }

  curl_global_cleanup();

  return 0;
}

