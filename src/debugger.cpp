/*********************** Licensing *******************************************************
*
*   Copyright 2018 @ Evandro Coan, https://github.com/evandrocoan
*
*  This program is free software; you can redistribute it and/or modify it
*  under the terms of the GNU General Public License as published by the
*  Free Software Foundation; either version 3 of the License, or ( at
*  your option ) any later version.
*
*  This program is distributed in the hope that it will be useful, but
*  WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*  General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*****************************************************************************************
*/


#include "debugger.h"

// [TUT] How to use an efficient debug system, which does not overload the final code -> C/C++ Language Implementation
// https://forums.alliedmods.net/showthread.php?t=277682#Cpp
//
// How do I use extern to share variables between source files?
// https://stackoverflow.com/questions/1433204/how-do-i-use-extern-to-share-variables-between-source-files
#if DEBUG_LEVEL > DEBUG_LEVEL_DISABLED_DEBUG
  // initialize `extern` variables shared across all source files
  std::clock_t _debugger_current_saved_c_time = std::clock();
  std::chrono::time_point<std::chrono::high_resolution_clock> _debugger_current_saved_chrono_time = std::chrono::high_resolution_clock::now();

  // Default debug level, if not defined on any other file
  int _debugger_int_debug_level = 3;
  const char* _debugger_char_debug_level = "a1 a3 b7";
#endif

