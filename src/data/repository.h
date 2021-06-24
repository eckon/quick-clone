#ifndef REPOSITPRY_H
#define REPOSITPRY_H

#include <string>

class Repository {
 public:
  std::string ssh_url_to_repo;
  std::string name;
  std::string name_with_namespace;
  std::string http_url_to_repo;
  std::string web_url;

  Repository(std::string ssh_url_to_repo, std::string name,
             std::string name_with_namespace, std::string http_url_to_repo,
             std::string web_url);

  Repository();
};

#endif
