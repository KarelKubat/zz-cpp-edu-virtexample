#include <iostream>
#include "postgresqlstore"

// Hypothetical constructor of a Postgresql storage back end. It receives
// a couple of parameters that define how to connect to the database and
// supposedly initializes the connection.
Postgresqlstore::Postgresqlstore(std::string const &dbserver,
                                 std::string const &dbname,
                                 std::string const &dbuser,
                                 std::string const &dbpass) {
  // This would do some magic and initialize the connection to the
  // database server. In this case the connection is represented by just
  // an int, in real life it's some pointer.
  connection = 42;
  std::cout << "Postgresqlstore: Connected to server " << dbserver
            << " using database name " << dbname << ", database user "
            << dbuser << ", database password " << dbpass << '\n';
}

// Hypothetical destructor to close the hypothetical connection to
Postgresqlstore::~Postgresqlstore() {
  std::cout << "Postgresqlstore: disconnecting\n";

  // Setting the connection to a value 0 doesn't really make sense when the
  // object is about to go away, but the idea is that some Postgresql method
  // to commit all changes and to disconnect would be called here.
  connection = 0;
}

// Methods dictacted by interface
void Postgresqlstore::insert(std::string const &name, std::string const &email,
                          std::string const &passwd) {
  std::cout << "Postgresqlstore: inserting using connection " << connection
            << ":\n"
            << "  name " << name << ", email " << email
            << ", password " << passwd << '\n';
}

void Postgresqlstore::remove(std::string const &email) {
  std::cout << "Postgresqlstore: removing using connection " << connection
            << ":\n"
            << "  email " << email << '\n';
}
