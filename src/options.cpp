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

#include <iostream>

#include <stdlib.h>

#include "options.hpp"

Options::Options()
{
  drop = false;
  append = false;
  prepare = false;

  index = false;

  program = "lfp2pgsql";
}

void
Options::parseArgs(int argc, char *argv[])
{
  int c;
  int x = 0;

  opterr = 0;

  while ((c = getopt (argc, argv, "dacpI")) != -1)
    switch(c) {
    case 'd':
      drop = true;
      prepare = true;
      append = true;
      x++;
      break;
    case 'a':
      append = true;
      x++;
      break;
    case 'c':
      prepare = true;
      append = true;
      x++;
      break;
    case 'p':
      prepare = true;
      x++;
      break;
    case 'I':
      index = true;
      break;
    case '?':
      printUsage();
      exit(EXIT_SUCCESS);
    }

  if (x == 0) {
    prepare = true;
    append = true;
  }

  if (x > 1) {
    printUsage();
    exit(EXIT_FAILURE);
  }

  if (argc - optind != 2) {
    printUsage();
    exit(EXIT_FAILURE);
  }

  filename = argv[optind];
  tablename = argv[optind + 1];

  return;
}

bool
Options::shouldDropTable()
{
  return drop;
}

bool
Options::shouldAppend()
{
  return append;
}

bool
Options::shouldPrepare()
{
  return prepare;
}

bool
Options::shouldCreateIndex()
{
  return index;
}

std::string
Options::getTablename()
{
  return tablename;
}

std::string
Options::getFilename()
{
  return filename;
}

void
Options::printUsage()
{
  std::cout << "USAGE: " << program 
            << " [<options>] <DXF file> <table name>"
            << std::endl;

  std::cout << "OPTIONS:" << std::endl;
  std::cout << "  (-d|a|c|p) These are mutually exclusive options:" 
            << std::endl
            << "    -d  Drops the table, then recreates it and populates"
            << std::endl
            << "        it with current shape file data." << std::endl
            << "    -a  Appends shape file into current table, must be"
            << std::endl
            << "        exactly the same table schema." << std::endl
            << "    -c  Creates a new table and populates it, this is the"
            << std::endl
            << "        default if you do not specify any options." 
            << std::endl
            << "    -p  Prepare mode, only creates the table." << std::endl;
  std::cout << "  -I  Create a spatial index on the geocolumn." << std::endl;
  std::cout << "  -?  Display this help screen." << std::endl;

  return;
}
