/*  lfp2pgsql - Parse a Landform Panorama DXF file and output as SQL

    Copyright (C) 2011 Keith Sharp <keith.sharp@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

    Author: Keith Sharp <keith.sharp@gmail.com>
*/

#ifndef _OPTIONS_HPP_
#define _OPTIONS_HPP_

#include <string>

class Options
{
private:
  // Mutually exclusive, only one can be true
  bool drop;    // Drop the table then create and populate
  bool append;  // Append data to existing table
  bool prepare; // Only create the table

  bool index;   // Create a spatial index on the geocolumn

  std::string filename;      // Name of the DXF file to parse
  std::string tablename;     // Name of the table to create

  std::string program;       // Name of the program

public:
  Options();

  void parseArgs(int argc, char *argv[]);

  bool shouldDropTable();
  bool shouldAppend();
  bool shouldPrepare();

  bool shouldCreateIndex();

  std::string getTablename();
  std::string getFilename();

  void printUsage();
};

#endif // _OPTIONS_HPP_
