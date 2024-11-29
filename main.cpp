#include "ConfigTranslator.hpp"
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <sstream>

namespace po = boost::program_options;

int main(int argc, char *argv[]) {
  try {
    po::options_description desc("Options");
    desc.add_options()("help,h", "Show help message")(
        "input,i", po::value<std::string>(), "Input JSON file")(
        "output,o", po::value<std::string>(), "Output file (default: stdout)");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << std::endl;
      return 0;
    }

    if (!vm.count("input")) {
      throw std::runtime_error(
          "Input file is required. Use --input to specify the file.");
    }

    std::string input_file = vm["input"].as<std::string>();
    std::ifstream input_stream(input_file);
    if (!input_stream.is_open()) {
      throw std::runtime_error("Failed to open input file: " + input_file);
    }

    std::stringstream buffer;
    buffer << input_stream.rdbuf();
    input_stream.close();
    std::string input_data = buffer.str();

    ConfigTranslator translator;

    std::string output_data = translator.translate(input_data);

    if (vm.count("output")) {
      std::string output_file = vm["output"].as<std::string>();
      std::ofstream output_stream(output_file);
      if (!output_stream.is_open()) {
        throw std::runtime_error("Failed to open output file: " + output_file);
      }
      output_stream << output_data;
      output_stream.close();
    } else {
      std::cout << output_data << std::endl;
    }

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
