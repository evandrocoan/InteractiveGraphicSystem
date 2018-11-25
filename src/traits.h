#ifndef GTKMM_APP_TRAITS_H
#define GTKMM_APP_TRAITS_H

typedef long double big_double;
constexpr const int MATRICES_DIMENSION = 4;

constexpr const big_double MINIMUM_ZOOM_LIMIT = 0.001;
constexpr const big_double MAXIMUM_ZOOM_LIMIT = 5000.0;

constexpr const big_double CLIPPING_WINDOW_MARGIN_DISTANCE = 0.1;

constexpr const char* const WHOLE_WORLD = "Whole World";
constexpr const char* const DEFAULT_MOVE_LENGTH = "15";
constexpr const char* const PROGRAM_AUTHORS = "Graphics Computing - Interactive Graphic System - Evandro Coan, Marcello da Silva Klingelfus Junior";

#endif // GTKMM_APP_TRAITS_H
