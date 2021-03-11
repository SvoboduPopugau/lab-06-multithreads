// Copyright 2020 Your Name <your_email>


#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <nlohmann/json.hpp>
#include <chrono>
#include <mutex>
#include <vector>
#include <thread>
#include <csignal>
#include "../third-party/PicoSHA2/picosha2.h"

#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

using high_res_clock = std::chrono::high_resolution_clock;
using json = nlohmann::json;
namespace source = boost::log::sources;
namespace keys = boost::log::keywords;
namespace sinks = boost::log::sinks;
using namespace boost::log::trivial;

static const std::string RIGHT_HEX_END = "0000";

class MySHAMaster {
 public:
  MySHAMaster(int argc, char *argv[]);
  ~MySHAMaster();
  void Init();

  void static StopSignal(int sign){
    is_work = false;
    SaveInJson("Right_Hashes.json");
    std::cout << "Program stopped with code " << --sign << std::endl;
    exit(sign);
  }

  static void SaveInJson(const std::string& filename){
    json j_object;
    for (auto& iterator:v_true_hashes){
      j_object["Required_Hashes"].push_back(iterator);
    }
    std::ofstream out_file;
    out_file.open(filename);
    out_file << j_object.dump(4);
  }

 private:
  void Hash();
  bool HexCheck(std::string& str_hex);
  void Logging(bool& severity, std::string& str_hex, std::string& str_source);
 private:
  static bool is_work;
  long int duration;
  unsigned int number_of_threads;
  std::mutex mutex;
  std::vector<std::thread> v_threads;
  source::severity_logger<severity_level> logger;
  static std::vector<json> v_true_hashes;
};

#endif // INCLUDE_HEADER_HPP_
