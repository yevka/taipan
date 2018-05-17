#include <iostream>

#include "Engine.h"

int main(int argc, char** argv) {

  Engine taipan;
  taipan.EngineInfo();
  taipan.Init();
  taipan.Loop(argc, argv);

  return 0;
}
