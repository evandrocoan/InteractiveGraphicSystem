
#include "facade.h"

void Facade::add_test_objects()
{
  std::vector<big_double> polygon_coord_list;

  polygon_coord_list.clear();
  polygon_coord_list.push_back(-50); polygon_coord_list.push_back(-50); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(-50); polygon_coord_list.push_back(-30); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(-30); polygon_coord_list.push_back(-30); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(-30); polygon_coord_list.push_back(-50); polygon_coord_list.push_back(1);
  this->addPolygon("Polygon", polygon_coord_list, _default_coordinate_value_parameter, _default_coordinate_value_parameter, CurveType::POLYGON);

  polygon_coord_list.clear();
  polygon_coord_list.push_back(0); polygon_coord_list.push_back(0); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(10); polygon_coord_list.push_back(10); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(20); polygon_coord_list.push_back(15); polygon_coord_list.push_back(1);
  polygon_coord_list.push_back(20); polygon_coord_list.push_back(0); polygon_coord_list.push_back(1);
  this->addPolygon("Bezier", polygon_coord_list, _default_coordinate_value_parameter, _default_coordinate_value_parameter, CurveType::BEZIER);
}
