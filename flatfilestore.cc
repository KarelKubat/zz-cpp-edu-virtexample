#include <iostream>
#include "flatfilestore"

Flatfilestore::Flatfilestore(std::string const &filename) {
  std::cout << "Flatfilestore: using flatfile " << filename << '\n';

  // The constructor would now open filename and assign it to the class
  // field 'stream' for later usage
}

void Flatfilestore::insert(std::string const &name, std::string const &email,
                          std::string const &passwd) {
  std::cout << "Flatfilestore: inserting into flatfile:\n"
            << "  name " << name << ", email " << email
            << ", password " << passwd << '\n';
}

void Flatfilestore::remove(std::string const &email) {
  std::cout << "Flatfilestore: removing from flatfile:\n"
            << "  email " << email << '\n';
}
