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

#ifndef _CONTOUR_HPP_
#define _CONTOUR_HPP_

#include <vector>
#include <string>

struct Point
{
  double x, y;
};


class Contour
{
private:
  int height;
  std::string feature;
  std::vector<Point> points;

public:
  Contour(int h);

  void addPoint(double x, double y);
  void addFeature (std::string f);

  int getHeight();
  std::string getFeature();
  std::string getPointsAsWKT();
};

#endif // _CONTOUR_HPP_
