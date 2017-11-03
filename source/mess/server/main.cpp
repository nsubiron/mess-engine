#include <iostream>

#include "crypto/secure_string.h"
#include "mess/game/Game.h"
#include "mess/game/Python.h"

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <unistd.h> // getpass

enum MainFunctionReturnValues {
  Success,
  InvalidArgument,
  PythonException,
  StdException,
  UnknownException
};

namespace fs = boost::filesystem;
namespace po = boost::program_options;

using namespace mess::game;

static crypto::secure_string prompt_for_password(const char *prompt) {
  using namespace crypto;
  auto buffer = buffer_view::make_mutable(getpass(prompt));
  return secure_string::clean_buffer_and_make(buffer);
}

static void do_the_thing(const GameSettings &settings) {
  boost::asio::io_service io_service;

  auto game_pwd = prompt_for_password("game password: ");
  auto master_pwd = prompt_for_password("admin password: ");

  Game game(io_service, settings, game_pwd, master_pwd);

  game_pwd.clear();
  master_pwd.clear();

  game.AsyncRun();

  io_service.run();
}

int main(int argc, char *argv[]) {
  try {

    GameSettings settings;

    {
      po::options_description desc("Allowed options");
      desc.add_options()
        ("help,h",
            "show this help message and exit")
        ("game-name",
            po::value<std::string>(&settings.game_name)->required(),
            "name of the game")
        ("python-folder",
            po::value<std::string>(&settings.python_code_folder),
            "folder containing the python code")
        ("port",
            po::value<uint32_t>(&settings.server_port),
            "output folder to save converted images")
      ;

    try {

        // Parse command-line args.
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        // Print help and exit if requested.
        if (vm.count("help")) {
          std::cout << desc << "\n";
          return Success;
        }

        // Throw if any argument is invalid.
        po::notify(vm);

        // Check if input_folder exists.
        if (!fs::is_directory(settings.python_code_folder)) {
          throw std::invalid_argument("not a folder: " + settings.python_code_folder);
        }

      } catch (const po::error &e) {
        std::cerr << desc << "\n" << e.what() << std::endl;
        return InvalidArgument;
      }
    }

    do_the_thing(settings);

    return Success;
  } catch (const boost::python::error_already_set&) {
    Python::PrintError();
    return PythonException;
  } catch (const std::exception &e) {
    std::cerr << "EXCEPTION: " << e.what() << std::endl;
    return StdException;
  } catch (...) {
    std::cerr << "UNKNOWN EXCEPTION!" << std::endl;
    return UnknownException;
  }
}
