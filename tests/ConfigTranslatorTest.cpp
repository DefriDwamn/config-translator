#include "ConfigTranslator.hpp"
#include <gtest/gtest.h>

namespace json = boost::json;

TEST(ConfigTranslatorTest, ValidName) {
  ConfigTranslator translator;
  EXPECT_TRUE(translator.isValidName("MYVAR"));
}

TEST(ConfigTranslatorTest, InvalidNameWithUnderscore) {
  ConfigTranslator translator;
  EXPECT_FALSE(translator.isValidName("MY_VAR"));
}

TEST(ConfigTranslatorTest, InvalidNameWithLowercase) {
  ConfigTranslator translator;
  EXPECT_FALSE(translator.isValidName("myvar"));
}

TEST(ConfigTranslatorTest, ProcessVarDeclaration) {
  ConfigTranslator translator;
  std::string input = "var MYVAR = 100";
  EXPECT_EQ(translator.processString(input), "");
}

TEST(ConfigTranslatorTest, ProcessVariableEvaluation) {
  ConfigTranslator translator;
  translator.declareVariable("MYVAR", "100");
  std::string input = "#[MYVAR]";
  EXPECT_EQ(translator.processString(input), "100");
}

TEST(ConfigTranslatorTest, ProcessStringLiteral) {
  ConfigTranslator translator;
  std::string input = "Hello, World";
  EXPECT_EQ(translator.processString(input), "q(Hello, World)");
}

TEST(ConfigTranslatorTest, ProcessEmptyObject) {
  ConfigTranslator translator;
  json::object obj;
  EXPECT_EQ(translator.processObject(obj), "{\n}");
}

TEST(ConfigTranslatorTest, ProcessObjectWithSingleKeyValue) {
  ConfigTranslator translator;
  json::object obj;
  obj["KEY"] = 42;
  EXPECT_EQ(translator.processObject(obj), "{\n KEY : 42,\n}");
}

TEST(ConfigTranslatorTest, ProcessValueInt) {
  ConfigTranslator translator;
  json::value val = 100;
  EXPECT_EQ(translator.processValue(val), "100");
}

TEST(ConfigTranslatorTest, ProcessValueString) {
  ConfigTranslator translator;
  json::value val = "Hello";
  EXPECT_EQ(translator.processValue(val), "q(Hello)");
}

TEST(ConfigTranslatorTest, ProcessValueObject) {
  ConfigTranslator translator;
  json::object obj;
  obj["SUBKEY"] = 42;
  json::value val = obj;
  EXPECT_EQ(translator.processValue(val), "{\n SUBKEY : 42,\n}");
}

TEST(ConfigTranslatorTest, DeclareValidVariable) {
  ConfigTranslator translator;
  EXPECT_NO_THROW(translator.declareVariable("MYVAR", "100"));
}

TEST(ConfigTranslatorTest, DeclareInvalidVariable) {
  ConfigTranslator translator;
  EXPECT_THROW(translator.declareVariable("myvar", "100"), std::runtime_error);
}

TEST(ConfigTranslatorTest, EvaluateExistingVariable) {
  ConfigTranslator translator;
  translator.declareVariable("MYVAR", "100");
  EXPECT_EQ(translator.evaluateVariable("MYVAR"), "100");
}

TEST(ConfigTranslatorTest, EvaluateNonExistentVariable) {
  ConfigTranslator translator;
  EXPECT_THROW(translator.evaluateVariable("NONEXISTENT_VAR"),
               std::runtime_error);
}
