#include "api.h"

#include <curl/curl.h>

#include <nlohmann/json.hpp>

static size_t writeCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

// TODO: this or parts of it should be in the repo file
std::vector<Repository> getRepoResources(std::string searchValue) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;
  std::vector<Repository> repositories;
  long httpCode = 0;

  std::string gitlabAccessToken = std::getenv("GITLAB_ACCESS_TOKEN");
  std::string gitlabProjectsUrl = std::getenv("GITLAB_PROJECTS_URL");

  std::string searchParameter = "search=" + searchValue;
  std::string resultAmount = "100";
  std::string perPageParameter = "per_page=" + resultAmount;
  std::string orderParameter = "order_by=last_activity_at";
  std::string privateToken = "PRIVATE-TOKEN: " + gitlabAccessToken;
  std::string baseUrl = gitlabProjectsUrl;
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

    // TODO: get more output for error
    if (res != CURLE_OK)
      throw std::string("Could not get url resource, code: ") +
          std::to_string(res);

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
    if (httpCode != 200)
      throw std::string("HTTP response was not 200 - got: ") +
          std::to_string(httpCode);

    curl_easy_cleanup(curl);

    std::string data = readBuffer.data();
    auto json = nlohmann::json::parse(data);
    for (auto &el : json.items()) {
      auto val = el.value();
      Repository repo(val["ssh_url_to_repo"], val["name"],
                      val["name_with_namespace"], val["http_url_to_repo"],
                      val["web_url"]);
      repositories.push_back(repo);
    }
  }

  return repositories;
}
