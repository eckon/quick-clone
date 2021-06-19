#include <curl/curl.h>

#include <cstdio>
#include <iostream>
#include <list>
#include <nlohmann/json.hpp>
#include <string>

#include "repository.h"

static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

// TODO: this or parts of it should be in the repo file
std::list<Repository> getRepoResources(std::string searchValue) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  std::list<Repository> repositories;
  long httpCode = 0;

  std::string searchParameter = "search=" + searchValue;
  std::string resultAmount = "100";
  std::string perPageParameter = "per_page=" + resultAmount;
  std::string orderParameter = "order_by=last_activity_at";
  std::string privateToken = "PRIVATE-TOKEN: SECRET";
  std::string baseUrl = "https://registry.ascora.eu/api/v4/projects";
  std::string url = baseUrl + "?" + searchParameter + "&" + perPageParameter +
                    "&" + orderParameter;


  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, privateToken.c_str());

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    if (httpCode != 200)
      fprintf(stderr, "HTTP response was not 200 - got: %ld\n", httpCode);

    curl_easy_cleanup(curl);

    std::string data = readBuffer.data();
    auto json = nlohmann::json::parse(data);
    for (auto &el : json.items()) {
      repositories.push_back(Repository{el.value()["ssh_url_to_repo"]});
    }
  }

  return repositories;
}
