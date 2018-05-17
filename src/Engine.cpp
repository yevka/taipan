#include "Engine.h"

#include <iostream>

#include "version.h"
#include "MoveGenerator.h"
#include "FEN.h"


Engine::Engine() {

}

void Engine::EngineInfo() const {
  std::cout << engine_info() << std::endl;
}

void Engine::Init() {

}

// On ucinewgame following steps are needed to reset the state
void Engine::NewGame() {

}

void Engine::Position(std::istringstream &is) {
  const FEN StartFEN("[FEN \"W:W21,22,23,24,25,26,27,28,29,30,31,32:B1,2,3,4,5,6,7,8,9,10,11,12\"]");

  FEN fen;
  std::string token;

  is >> token;
  if (token == "startpos") {
    fen = StartFEN;
    is >> token; // Consume "moves" token if any
  } else if (token == "fen") {
    std::string fenStr;
    while (is >> token && token != "moves") { fenStr += token + " "; }
    fen = FEN(fenStr);
  } else { return; }

  // Parse move list (if any)
  //while (is >> token && (m = UCI::to_move(pos, token)) != MOVE_NONE) {
  //  States->push_back(StateInfo());
  //  pos.do_move(m, States->back());
  //}
}

void Engine::Loop(int argc, char **argv) {
  std::string token, cmd;

  do {
    // Block here waiting for input or EOF
    if (argc == 1 && !std::getline(std::cin, cmd)) {
      cmd = "quit";
    }

    std::istringstream is(cmd);

    token.clear(); // getline() could return empty or blank line
    is >> std::skipws >> token;

    // The GUI sends 'ponderhit' to tell us to ponder on the same move the
    // opponent has played. In case Signals.stopOnPonderhit is set we are
    // waiting for 'ponderhit' to stop the search (for instance because we
    // already ran out of time), otherwise we should continue searching but
    // switching from pondering to normal search.
    if (token == "quit" || token == "stop") {

    } else if (token == "uci") {
      std::cout << "id name " << engine_info() << "\nuciok" << std::endl;

    } else if (token == "ucinewgame") {

    } else if (token == "isready") {
      std::cout << "readyok" << std::endl;

    } else if (token == "go") {

    } else if (token == "position") {
      Position(is);

    } else if (token == "setoption") {

    }
      // Additional custom non-UCI commands, useful for debugging
    else if (token == "perft") {

    } else {
      std::cout << "Unknown command: " << cmd << std::endl;
    }

  } while (token != "quit" && argc == 1); // Passed args have one-shot behaviour

}
