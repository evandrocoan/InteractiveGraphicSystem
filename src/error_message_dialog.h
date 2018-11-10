/*********************** Licensing *******************************************************
*
*   Copyright 2017 @ Evandro Coan
*
*   Program Main Page: https://github.com/evandrocoan/ObjectBeautifier
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

#ifndef GTKMM_APP_ERROR_MESSAGE_DIALOG_H
#define GTKMM_APP_ERROR_MESSAGE_DIALOG_H

#include <gtkmm.h>
#include <stdexcept>

#include "debugger.h"

inline void errorMessage(const std::runtime_error& error)
{
  LOG( 1, "%s", error.what() );

  // https://stackoverflow.com/questions/18554282/destroy-gtkmm-message-dialog
  Gtk::MessageDialog dialog("Error message", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK);
  dialog.set_secondary_text( error.what() );
  dialog.set_default_response( Gtk::RESPONSE_OK );
  dialog.run();
}

#endif // GTKMM_APP_ERROR_MESSAGE_DIALOG_H

