#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <vector>

class ApiConfig {
 public:
  std::string name;
  std::string accessToken;
  std::string url;

  ApiConfig();
  ApiConfig(std::string name, std::string accessToken, std::string url);
};

class ApiConfigCollection {
 public:
  std::vector<ApiConfig> apiConfigurations;
  int selected;

  ApiConfigCollection();

  void init();
  void generatePlaceholderConfigFile(std::string path, std::string filename);
};

#endif
