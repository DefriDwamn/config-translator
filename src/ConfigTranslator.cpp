#include "ConfigTranslator.hpp"
#include <boost/json/src.hpp>
#include <regex>
#include <stdexcept>

namespace json = boost::json;

ConfigTranslator::ConfigTranslator() = default;

bool ConfigTranslator::isValidName(const std::string &name) {
  static const std::regex nameRegex(R"([A-Z]+)");
  return std::regex_match(name, nameRegex);
}

std::string ConfigTranslator::processString(const std::string &str) {
  static const std::regex varRegex(R"(var\s+([A-Z_]+)\s*=\s*(.*))");
  std::smatch match;
  if (std::regex_match(str, match, varRegex)) {
    std::string name = match[1];
    std::string value = match[2];
    declareVariable(name, value);
    return "";
  }

  static const std::regex evalRegex(R"(#\[(.+)\])");
  if (std::regex_match(str, match, evalRegex)) {
    std::string name = match[1];
    return evaluateVariable(name);
  }

  return "q(" + str + ")";
}

std::string ConfigTranslator::processObject(const json::object &obj) {
  std::string result = "{\n";
  for (const auto &[key, value] : obj) {
    std::string keyStr{key};
    if (!isValidName(keyStr)) {
      throw std::runtime_error("Invalid name: " + keyStr);
    }
    result += " " + keyStr + " : " + processValue(value) + ",\n";
  }
  result += "}";
  return result;
}

std::string ConfigTranslator::processValue(const json::value &value) {
  if (value.is_object()) {
    return processObject(value.as_object());
  } else if (value.is_int64()) {
    return std::to_string(value.as_int64());
  } else if (value.is_string()) {
    std::string str = value.as_string().c_str();
    return processString(str);
  } else {
    throw std::runtime_error("Unsupported value type");
  }
}

void ConfigTranslator::declareVariable(const std::string &name,
                                       const std::string &value) {
  if (!isValidName(name)) {
    throw std::runtime_error("Invalid variable name: " + name);
  }
  variables[name] = value;
}

std::string ConfigTranslator::evaluateVariable(const std::string &name) {
  if (variables.find(name) == variables.end()) {
    throw std::runtime_error("Undefined variable: " + name);
  }
  return variables[name];
}

std::string ConfigTranslator::translate(const std::string &jsonInput) {
  json::value parsed = json::parse(jsonInput);

  if (!parsed.is_object()) {
    throw std::runtime_error("Root element must be a JSON object");
  }

  return processObject(parsed.as_object());
}
