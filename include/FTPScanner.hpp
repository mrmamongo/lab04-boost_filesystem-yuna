//
// Created by lamp on 07.12.2020.
//

#ifndef SCANNER_FTPSCANNER_HPP
#define SCANNER_FTPSCANNER_HPP

#include "iostream"
#include <boost/filesystem.hpp>
#include <string>
#include <utility>
#include <vector>
#include <sstream>
#include <map>
#define fs boost::filesystem
using std::cout;
using std::endl;
using std::string;
using std::vector;

class FTPScanner;
struct Account;

struct Broker{
 protected:
  string name;
  std::map<string, Account> accounts;
 public:
  explicit Broker(string name_): name(std::move(name_)){}
  Broker(): name("EMPTY"){}
  friend struct Account;
  friend class FTPScanner;
  friend std::ostream& operator<<
      (std::ostream& os, const FTPScanner& scanner);
};


struct Account{
 protected:
  string accountID;
  size_t lastDate;
  size_t fileAmount;
  friend struct Broker;
  friend class FTPScanner;
  friend std::ostream& operator<<
      (std::ostream& os, const FTPScanner& scanner);
  Account(string  ID, size_t date)
      : accountID(std::move(ID)), lastDate(date), fileAmount(1) {};
  static void Scan
      (const fs::directory_entry&, const std::shared_ptr<Broker>&);
 public:
  Account() : lastDate(0), fileAmount(0){};
};

class FTPScanner {
 protected:
  std::unordered_map<string, std::shared_ptr<Broker>> brokers;
  fs::path path;
  string stringPath;

 public:
  explicit FTPScanner
      (string  p = ".");
  void Scan(const fs::path& p, std::shared_ptr<Broker> b);
  friend std::ostream& operator<<
      (std::ostream& os, const FTPScanner& scanner);
};


#endif  // SCANNER_FTPSCANNER_HPP
