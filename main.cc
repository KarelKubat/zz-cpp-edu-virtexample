#include <iostream>
#include <string>

#include <stdlib.h>

#include "sqlite3store"         // Backend that uses sqlite3
#include "postgresqlstore"      // Backend that uses postgresql
#include "flatfilestore"        // Backend that uses flat files

int main(int argc, char **argv) {
  // We expect 1 commandline argument to say what store we will use.
  if (argc != 2) {
    std::cerr << "You must give one commandline argument which is one of:\n"
              << "  sqlite3 postgres flatfile\n";
    exit(1);
  }

  // Instantiate the right backend.
  Store *store;
  if (argv[1] == std::string("sqlite3"))
    store = new Sqlite3store("/etc/sqlite3/mystore.db");
  else if (argv[1] == std::string("postgres"))
    store = new Postgresqlstore("dbserver.my.net", "mydb", "dbuser", "dbpass");
  else if (argv[1] == std::string("flatfile"))
    store = new Flatfilestore("/tmp/flatfile.csv");
  else {
    std::cerr << "Bad backend specifier, use one of:\n"
              << "  sqlite3 postgres flatfile\n";
    exit(1);
  }

  // Do something fancy with the backend.
  store->insert("John Doe", "johndoe@example.com", "secret");
  store->remove("janedoe@example.com");

  // All done
  delete store;
  return 0;
}
