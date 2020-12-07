//
// Created by lamp on 07.12.2020.
//
#include "FTPScanner.hpp"

  int main(int argc, char* argv[])
  {
    try {
      if (argc < 2) {
        cout << "No arguments entered, scanning parent directory.....\n";
        FTPScanner scanner;
        cout << scanner;
        return 0;
      }
      FTPScanner scanner(argv[1]);
      cout << scanner;
      return 0;
    } catch (fs::filesystem_error& er) {
      cout << er.what() << endl;
      return 1;
    }
  }
