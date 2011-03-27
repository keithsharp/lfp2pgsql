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

#include "sqlwriter.hpp"

void
SQLWriter::startTransaction()
{
  std::cout << "BEGIN;" << std::endl;

  return;
}

void
SQLWriter::endTransaction(bool createIndex, std::string table)
{
  if (createIndex)
    std::cout << "CREATE INDEX \"" << table << "_the_geom_gist\" ON \"" << table 
              << "\" using gist (\"the_geom\" gist_geometry_ops);" << std::endl;

  std::cout << "COMMIT;" << std::endl;

  return;
}

void
SQLWriter::dropTable(std::string table)
{
  std::cout << "DROP TABLE IF EXISTS \"" << table << "\";" << std::endl;
  
  return;
}

void
SQLWriter::createTable(std::string table)
{
  std::cout << "CREATE TABLE \"" << table
            << "\" (gid serial PRIMARY KEY, feature char(8), height int);" 
            << std::endl;

  std::cout << "SELECT AddGeometryColumn('', '" << table 
            << "', 'the_geom', '27700', 'LINESTRING', 2);" << std::endl;
  
  return;
}

void
SQLWriter::populateTable(std::string table, std::vector<Contour> contours)
{
  for (int i = 0; i < contours.size(); i++) {
    std::cout << "INSERT INTO \"" << table
              << "\" (feature, height, the_geom) VALUES ('"
              << contours[i].getFeature() << "', "
              << contours[i].getHeight() << ", "
              << "GeomFromText('"
              << contours[i].getPointsAsWKT() 
              << "', 27700));" << std::endl;
  }

  return;
}
