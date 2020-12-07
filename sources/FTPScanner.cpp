//
// Created by lamp on 07.12.2020.
//

#include "FTPScanner.hpp"

size_t GetInfo(const string&, string&);


FTPScanner::FTPScanner(string  p)
    : stringPath(std::move(p)) {
  cout << "========================SCANNED FILES========================\n";
      Scan(fs::path(stringPath), std::make_shared<Broker>(Broker()));
}

void FTPScanner::Scan(const fs::path& p, std::shared_ptr<Broker> b) {
  try
  {
    if (exists(p))
    {
      if (is_directory(p))
      {
        for (fs::directory_entry& x : fs::directory_iterator(p))
        {
          if (is_directory(x)) {
            Broker brok(x.path().filename().string());
            b = std::make_shared<Broker>(brok);
            Scan(x.path(), b);
            if (b->name != "EMPTY" &&
                b->name != "docs" &&
                b->name != "common"
                ) {
              brokers.insert(std::make_pair(b->name, b));
            }
          } else {
            if(
                x.path().stem().extension() != ".old" &&
                x.path().stem() != "readme" &&
                x.path().stem() != "total" &&
                x.path().stem() != "report" ) {
              cout << b->name << " "
                   << x.path().filename().string() << endl;
              Account::Scan(x, b);
            }
          }
        }
      }
      else
        throw
            std::runtime_error
            {" exists, but is not a regular file or directory\n"};
    }
    else
      throw std::runtime_error{" does not exist\n"};
  } catch (const fs::filesystem_error& ex)
  {
    cout << ex.what() << endl;
  }
}

void Account::Scan
    (const fs::directory_entry& x, const std::shared_ptr<Broker>& b) {
  string scanningID;
  size_t date = GetInfo(x.path().stem().string(), scanningID);
  if (b->accounts.find(scanningID) != b->accounts.end()){
    ++(b->accounts[scanningID].fileAmount);
    if (b->accounts[scanningID].lastDate < date) {
      b->accounts[scanningID].lastDate = date;
    }
  } else if (b->accounts.find(scanningID) == b->accounts.end()){
    Account account(scanningID, date);
    b->accounts.insert(std::make_pair(scanningID, account));
  }
}

std::ostream& operator<<(std::ostream& os, const FTPScanner& scanner) {
  cout << "\n\n======================BROKER INFO======================\n";
  for (const auto& broker : scanner.brokers) {
    for (const auto& account : broker.second->accounts) {
      os << "broker:" << broker.first << " "
      << "account:"<< account.first << " "
         << "files:"<< account.second.fileAmount << " "
         << "lastDate:" << account.second.lastDate << endl;
    }
  }
  return os;
}

size_t GetInfo(const string& s, string& ID) {
  auto IDSymbolOffset = s.find_first_of('_');
  auto dateSymbolOffset = s.find_last_of('_');
  ID = s.substr(IDSymbolOffset+1, s.length() - 17);
  std::stringstream ss;
  size_t date;
  ss << s.substr(dateSymbolOffset+1, s.length());
  ss >> date;
  return date;
}

