// Copyright 2020 Your Name <your_email>

#include <MySHAMaster.hpp>

MySHAMaster::MySHAMaster(int argc, char **argv) {
  if (argc == 1){
    number_of_threads = std::thread::hardware_concurrency();
  }else{
    number_of_threads = static_cast<unsigned>(std::stoi(argv[1]));
  }
  duration = 0;
}
void MySHAMaster::Init() {
  boost::log::add_common_attributes();
  boost::log::add_console_log(std::clog,
                          keys::format = "[%Severity%] %TimeStamp%: %Message%");
  boost::log::add_file_log(
      keys::file_name = "Sample_%N.log",
      keys::rotation_size = 20 * 1024 * 1024,
      keys::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0),
      keys::format = "[%Severity%] %TimeStamp%: %Message%"
      );
  is_work = true;
  for(unsigned int i = 0; i < number_of_threads; i++){
    v_threads.emplace_back(std::thread([&](){
      Hash();
    }));
  }
}
void MySHAMaster::Hash() {
  std::chrono::system_clock::time_point start = high_res_clock::now();
  std::string str_source;
  std::string str_hex;
  while (is_work){
    str_source = std::to_string(std::rand());
    str_hex = picosha2::hash256_hex_string(str_source);
    std::chrono::system_clock::time_point end = high_res_clock::now();
    duration += static_cast<int>(std::chrono::nanoseconds(end - start).count());
    bool indicator = HexCheck(str_hex);
    mutex.lock();
    Logging(indicator, str_hex, str_source);

    mutex.unlock();
  }
}

bool MySHAMaster::HexCheck(std::string& str_hex) {
  if (RIGHT_HEX_END == str_hex.substr(str_hex.length() - 4)){
    return true;
  }else{
    return false;
  }
}
void MySHAMaster::Logging(bool& sever, std::string& str_hex,
                          std::string& str_source) {
  if (sever){
    BOOST_LOG_SEV(logger, info) << std::endl
                              << "Data: " << str_source
                              << "\tHash: " << str_hex
                              << "\tThread ID: " << std::this_thread::get_id()
                              << std::endl << std::endl;
    BOOST_LOG_TRIVIAL(info);
    json j_object = {
        {"Data", str_source},
        {"Hash", str_hex},
        {"Timestamp", duration}
    };
    v_true_hashes.push_back(j_object);
  }else{
    BOOST_LOG_SEV(logger, trace) << std::endl
                                << "Data: " << str_source
                                << "\tHash: " << str_hex
                                << "\tThread ID: " << std::this_thread::get_id()
                                << std::endl << std::endl;
  }
}
MySHAMaster::~MySHAMaster() {
    for(auto& iterator:v_threads){
      iterator.join();
    }
}

std::vector<json> MySHAMaster::v_true_hashes;
bool MySHAMaster::is_work;
