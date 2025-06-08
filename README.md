
# License Generator

### Objective : 

This is a simple C++ console application that manages licenses using SQLite as the database backend. It supports creating new licenses and searching existing ones.


## Features

- Create new licenses with user name, organization, and a generated license key.
- Search licenses by user name or organization.
- Stores data persistently in an SQLite database (`licenses.db`).
- Supports automatic database schema migration to add new fields without losing existing data.

## How to Use

1. Compile the program with a C++ compiler linking against SQLite3.
2. Run the executable.
3. Choose from the menu:
   - `1` to create a new license.
   - `2` to search licenses.
   - `3` to exit.
4. When creating a license, enter your name and organization.
5. The program will generate and store a license key.
6. Use the search feature to find licenses by name or organization.


## Extending the Schema

To add new fields:

1. Add the new columns to the `CREATE TABLE` statement in the code.
2. Update the `createLicense()` function to accept input for new fields and insert them into the database.
3. Use the `ensureColumnExists()` function to add new columns to existing databases without data loss.


## Dependencies

- SQLite3 library
- Standard C++11 or later compiler

## For simpleprogs Folders
The starting out programmes made for this feature
1. licencearray.cpp
2. orderlicence.cpp - uses licence_numbers.csv to save the data
3. randomlicence.app 

  

