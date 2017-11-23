# Example of Virtual Classes

Nothing to see here, move along unless you know some C++, you know the basics
of class derivation, you know about constructors and destructors, and you want
to learn about pure virtual classes and what good they are.

## What this is about

This demonstrates how to create a pure virtual class (interface) in C++ and how
to derive concrete implementations from it. This example builds a hypothetical
system to insert or remove users in some storage that could be used for a random
website. A truly usable example might need more, like a lookup, but that's just
more of the same.

### Virtual class

A class `Store` is defined as a pure virtual class (like a template or
interface definition) and it requires that concrete implementations of such a
store have:

*  a method `insert(name, email, password)` to add a newly registered user
*  a method `remove(email)` to delete a user from the store.

Class `Store` cannot be used as-is, because it only declares the methods but
never defines (implements) them. In C++ this is done by:

*  declaring the method `virtual`
*  adding `= 0` to the end of the declaration, meaning: not implemented.

See e.g. the header `store` which says:

```c++
virtual void remove(std::string const &email) = 0;
```

### Three concrete implementations

Next, three implementations are given as an example. All three "stores" are
implemented only to show how the principle; they don't really store anything
anywhere, they only print to `std::cout` when they are invoked. All three
implementations **must** implement `Store`s pure virtual method before they can
be considered a concrete implementation. The three examples are:

*  A flat file storage (class `Flatfilestore`)
*  Storage in a Sqlite3 database (class `Sqlite3store`)
*  Storage in using a Postgresql database (class `Postgresqlstore`).

All these classes implement `Store`s pure virtual functions, but they of course
may implement more method while still complying with the template. For example 
they have their own specific constructors:

*  `Flatfilestore`s and `Sqlite3store`s  constructors expect a filename
*  `Postgresqlstore`s constructor expects four strings to represent a
    database server, a database name, a database connection user and a
    connection password.

Similar to different constructors, the three implementations may, and do, define
different class variables:

*   `Flatfilestore` shows how a `std::fstream` would be opened in the
    constructor and how this stream would be the class variable to perform
    inserts and deletes,
*   `Sqlite3store` simply has it's constructor's parameter (filename) as the
    class variable,
*   `Postgresqlstore` has a connection to its database as class variable.  (In
    this example the connection is represented only as an `int`, this would in
    reality be a pointer to some Postgresql-defined structure).

### What good is this?

Having a virtual (template) class and multiple concrete implementations is
used as follows:

*  You declare a pointer to the base class, in this case `Store *mystore`
*  You point it to whatever concrete implementation you need at the moment:

   ```c++
   mystore = new Flatfilestore("/etc/myfile");
   // or:  = new Sqlite3store(...)     if you need this storage backend
   // or:  = new Postgresqlstore(...)  if you need this storage backend
   ```

*  You call whatever needs to be done:

   ```c++
   mystore->insert(...);       // actually calls Flatfilestore's insert
   mystore->remove(...);       // actually calls Flatfilestore's remove
   ```

*  To release the memory and to call necessary destructors, there is a
   cleanup:

   ```c++
   delete store;               // actually calls Flatfilestore's destructor
   							   // (if there is one)
   ```

Having a virtual (template) class and multiple concrete implementations solves
two major problems:

*  Without it, your code would get needlessly convoluted.
*  Without it, adding more concrete implementations would get ugly and
   error-prone real fast.

To make this clear, consider that you would have to use `switch` statements
all over your code in order to support the three storage backend candidates.
Some hypothetical code to illustrate:

```c++
enum RequestedBackend {
  want_flatfile,
  want_sqlite3,
  want_postgres
};

RequestedBackend wanted_backend = want_flatfile;
// or want_sqlite3 or want_postgres, depending on some commandline flag
// or depending on the user's choice

// Create store
Flatfilestore   *flat;
Sqlite3store    *sqlite3;
Postgresqlstore *postgres;
switch (wanted_backend) {
  case want_flatfile:
    flat = new Flatfilestore("/my/file");
    break;
  case want_sqlite3:
    sqlite3 = new Sqlite3store("/etc/my.db");
    break;
  case want_postgres:
    postgres = new Postgresqlstore("dbserver", "dbname", "dbuser", "dbpass");
    break;
}

// Insert something
switch (wanted_backend) {
  case want_flatfile:
    flat->insert("John Doe", "jonhdoe@example.com", "secret");
    break;
  case want_sqlite3:
    sqlite3->insert("John Doe", "jonhdoe@example.com", "secret");
    break;
  case want_postgres:
    postgres->insert("John Doe", "jonhdoe@example.com", "secret");
    break;
}

// Remove something
switch (wanted_backend) {
  case want_flatfile:
    flat->remove("jonhdoe@example.com");
    break;
  case want_sqlite3:
    sqlite3->remove("jonhdoe@example.com");
    break;
  case want_postgres:
    postgres->remove("jonhdoe@example.com");
    break;
}

// Clean up
switch (wanted_backend) {
  case want_flatfile:
    delete flat;
    break;
  case want_sqlite3:
    delete sqlite3;
    break;
  case want_postgres:
    delete postgres;
    break;
}

```

You get the idea. This is (a) needlessly long, and (b) error-prone, because 
you might forget one of the cases in one of the switches. The nightmare only 
continues when you would like to add a new storage backend, say a MySQL-based
database. You would have to edit all `switch` statements to add the right 
MySQL-backend invocation.

### Best practices

When looking at the provided sources, please have a look at the two following
practices.

#### Virtual but not pure virtual destructor in Store

When looking at the header `store` and the implementation `store.cc`, you can
see that the header declares the destructor as:

*   virtual
*   but not pure virtual (`= 0` is missing)

In the same vein, the implementation in `store.cc` defines just one function: 
the destructor. (Here, the keyword `virtual` doesn't have to be repeated, it
is already implicit, having been set in the header.)

There is a good reason for this:

*  Derived classes **may** define their own destructor. Given that `Store`s
   destructor is virtual, the derived destructor will be called, and not 
   `Store`s destructor.
   
*  Derived classes **don't have to** define their own destructor. If they
   don't, `Store`s destructor will be called, which doesn't do anything, but
   is there.
   
**Best practice:** When writing a virtual class (or pure virtual), always
provide a virtual destructor.

#### Gating against multiple inclusion in header store

The main function in `main.cc` includes the headers `flatfilestore`, 
`postgresqlstore` and `sqlite3store`. Each of these includes the header
`store`. So all in all, `main.cc` would include `store` three times.

In order to guarantee that the information in `store` is seen by the compiler
only once, the header is **gated**. That ensures that there is no
re-interpretation of it; if there were, the compiler would throw a redefinition
error.

Gating a header is very simple:

```c++
#ifndef _SOME_SYMBOL_
#define _SOME_SYMBOL_

//
// Header contents go here
//

#endif // _SOME_SYMBOL_
```

Upon the first inclusion by any source file, `_SOME_SYMBOL_` is not yet 
defined (`#ifndef` evaluates to true). Therefore, the symbol is defined and 
the header contents are parsed.

Upon each subsequent inclusion, `_SOME_SYMBOL_` is already defined, so that
the `#ifndef` evaluates to false and everything up to the end of the header 
is skipped (more precisely: up to the `#endif`).

Gating headers against multiple inclusion is always a good practice. In this
example it's done less-than-religiously (only in `store` and not in the other
headers).

## Organization and files

Each class is declared in its header and defined in its `.cc` file. Notably:

*  `store` is the header for class `Store`, `store.cc` defines the class
*  `flatfilestore` is the header for class `Flatfilestore`, `flatfilestore.cc`
   defines it
*  `postgresqlstore` is the header for class `Postgresqlstore`,
   `postgresqlstore.cc` defines it
*  `sqlite3store` is the header for class `Sqlite3store`, `sqlite3store.cc`
   defines it

The exception is `main.cc` which is the main program, and `Makefile` for Unixy
compilations.
