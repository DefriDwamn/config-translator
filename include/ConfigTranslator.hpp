#pragma once
#include <boost/json.hpp>
#include <string>
#include <unordered_map>

class ConfigTranslator {
public:
  ConfigTranslator();

  std::string translate(const std::string &jsonInput);

private:
  std::unordered_map<std::string, std::string> variables;

  bool isValidName(const std::string &name);
  std::string processValue(const boost::json::value &value);
  std::string processObject(const boost::json::object &obj);
  std::string processString(const std::string &str);
  void declareVariable(const std::string &name, const std::string &value);
  std::string evaluateVariable(const std::string &name);
};
