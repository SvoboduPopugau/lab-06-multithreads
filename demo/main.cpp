#include <MySHAMaster.hpp>
#include <iostream>

//Для многопотока
#include <thread>
#include <chrono>
//Для хешей
#include "../third-party/PicoSHA2/picosha2.h"
#include <nlohmann/json.hpp>
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
//#include <boost/utility/empty_deleter>
#include <boost/shared_ptr.hpp>
#include <boost/log/trivial.hpp>


int main(int argc, char *argv[]) {
  signal(SIGINT, MySHAMaster::StopSignal);
  MySHAMaster first (argc, argv);
  first.Init();
}