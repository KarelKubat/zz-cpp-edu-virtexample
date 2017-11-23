#include <iostream>
#include "sqlite3store"

Sqlite3store::Sqlite3store(std::string const &dbf): dbfile(dbf) {
  std::cout << "Sqlite3store: initialized to use database file " << dbfile
            << '\n';
}

void Sqlite3store::insert(std::string const &name, std::string const &email,
                          std::string const &passwd) {
  std::cout << "Sqlite3store: inserting into database file " << dbfile << ":\n"
            << "  name " << name << ", email " << email
            << ", password " << passwd << '\n';
}

void Sqlite3store::remove(std::string const &email) {
  std::cout << "Sqlite3store: removing from database file " << dbfile << ":\n"
            << "  email " << email << '\n';
}
