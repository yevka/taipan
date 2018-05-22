#include <iostream>
#include <exception>

#include "Engine.h"

int main(int argc, char** argv) {
  try {
    Engine taipan;
    taipan.EngineInfo();
    taipan.Init();
    taipan.Loop(argc, argv);

  } catch(std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  } catch(...) {
    std::cerr << "Unknown error" << std::endl;
    return 2;
  }

  return 0;
}
