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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <stdlib.h>

#include "options.hpp"
#include "contour.hpp"
#include "sqlwriter.hpp"

std::vector<Contour> parseDXFFile(std::string filename);

int
main (int argc, char *argv[])
{
  Options options;
  SQLWriter writer;

  std::vector<Contour> contours;

  options.parseArgs(argc, argv);

  writer.startTransaction();

  if (options.shouldDropTable())
    writer.dropTable(options.getTablename());

  if (options.shouldPrepare())
    writer.createTable(options.getTablename());

  if (options.shouldAppend()) {
    contours = parseDXFFile(options.getFilename());
    writer.populateTable(options.getTablename(), contours);
  }

  writer.endTransaction(options.shouldCreateIndex(), options.getTablename());

  return (EXIT_SUCCESS);
}

std::string
get_feature (std::ifstream& f)
{
  std::string line;

  std::getline(f, line);
  if (line.compare(0, 3, "  8") != 0) // 8 is the layer or feature name
    return("");
  std::getline(f, line);
  if ((line.compare(0, 7, "G804020") != 0) && 
      (line.compare(0, 8, "G8040200") == 0))
    return("");

  return line.substr(0, 8);
}

int
get_z_value (std::ifstream& f)
{
  std::string line;
  int z;

  std::getline(f, line);
  if (line.compare(0, 3, " 30") !=0) // 30 is the elevation field
    return(-1);
  std::getline(f, line);
  std::stringstream zss(line);
  if (!(zss >> z))
    return(-1);

  return(z);
}

double
get_eastings (std::ifstream& f)
{
  std::string line;
  double x;

  std::getline(f, line);
  if (line.compare(0, 3, " 10") !=0) // 10 is the eastings
    return (-1.0);

  std::getline(f, line);
  std::stringstream xss(line);
  if (!(xss >> x))
    return (-1.0);
  
  return (x);
}

double
get_northings (std::ifstream& f)
{
  std::string line;
  double y;

  std::getline(f, line);
  if (line.compare(0, 3, " 20") !=0) // 20 is the eastings
    return (-1.0);

  std::getline(f, line);
  std::stringstream xss(line);
  if (!(xss >> y))
    return (-1.0);
  
  return (y);
}

std::vector<Contour> parseDXFFile(std::string filename)
{
  std::vector<Contour> result;
  std::string line;
  std::ifstream dxf;

  dxf.open(filename.c_str());

  while (dxf.good()) {
    std::getline(dxf, line);
    if (line.compare(0, 8, "POLYLINE") == 0) {

      // Get the feature
      std::string feature = get_feature (dxf);
      if (feature.empty())
        continue;

      // Don't care about the next two lines
      std::getline(dxf, line);
      std::getline(dxf, line);

      // Get the z value
      int z = get_z_value (dxf);
      if (z < 0)
        continue;

      // Create a contour
      Contour contour(z);
      contour.addFeature(feature);

      // Add vertices to contour
      std::getline(dxf, line);
      while (line.compare(0, 6, "SEQEND") != 0) {
        if (line.compare(0, 6, "VERTEX") == 0) {
          feature = get_feature(dxf); // Ignore the return value

          double x = get_eastings(dxf);
          if (x < 0.0)
            continue;

          double y = get_northings(dxf);
          if (y < 0.0)
            continue;

          contour.addPoint(x, y);
        }
        std::getline(dxf, line);
      }
      result.push_back(contour);
    }
  }

  dxf.close();

  return(result);
}
