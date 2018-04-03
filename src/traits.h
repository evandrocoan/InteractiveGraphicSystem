#ifndef GTKMM_APP_TRAITS_H
#define GTKMM_APP_TRAITS_H

typedef long double big_double;

constexpr const int MATRICES_DIMENSION = 3;
constexpr const int MINIMUM_ZOOM_LIMIT = 1;
constexpr const int MAXIMUM_ZOOM_LIMIT = 5000;

constexpr const int WORLD_AXES_SIZE = 5000;
constexpr const int DEFAULT_ZOOM_SCALE = 30;

constexpr const big_double CLIPPING_WINDOW_MARGIN_DISTANCE = 0.1;

constexpr const char* const DEFAULT_MOVE_LENGTH = "20";
constexpr const char* const DEFAULT_ROTATE_ANGLE = "15";

#endif // GTKMM_APP_TRAITS_H
