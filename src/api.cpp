#include <curl/curl.h>

#include <iostream>
#include <nlohmann/json.hpp>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}

int main(int argc, char **argv) {
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  std::string searchValue = "bighost";
  std::string searchParameter = "search=" + searchValue;
  std::string resultAmount = "2";
  std::string perPageParameter = "per_page=" + resultAmount;
  std::string orderParameter = "order_by=last_activity_at";
  std::string url = "https://registry.ascora.eu/api/v4/projects?" +
                    searchParameter + "&" + perPageParameter + "&" +
                    orderParameter;
  std::string privateToken = "PRIVATE-TOKEN: SECRET";

  struct curl_slist *headers = NULL;
  headers = curl_slist_append(headers, privateToken.c_str());

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
    curl_easy_cleanup(curl);

    std::string data = readBuffer.data();

    // std::cout << data << std::endl;
    auto json = nlohmann::json::parse(data);
    // std::cout << json.dump(4) << std::endl;
    for (auto &el : json.items()) {
      std::cout << el.value()["ssh_url_to_repo"] << std::endl;
    }
  }

  return 0;
}
