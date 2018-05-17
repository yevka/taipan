#ifndef TAIPAN_ENGINE_H
#define TAIPAN_ENGINE_H

#include <string>
#include <sstream>

class Engine {
 public:
  Engine();

  void EngineInfo() const;
  void Init();
  void Loop(int argc, char** argv);

 private:
  void NewGame();
  void Position(std::istringstream& is);
};


#endif //TAIPAN_ENGINE_H
