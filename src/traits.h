#ifndef GTKMM_APP_TRAITS_H
#define GTKMM_APP_TRAITS_H

typedef long double big_double;

const int MATRICES_DIMENSION = 3;
const int MINIMUM_ZOOM_LIMIT = 1;
const int MAXIMUM_ZOOM_LIMIT = 5000;

const int world_axes_size = 5000;
const int default_zoom_scale = 30;
const int clipping_window_margin_distance = 30;

const char* const default_move_length = "20";
const char* const default_rotate_angle = "15";

#endif // GTKMM_APP_TRAITS_H
