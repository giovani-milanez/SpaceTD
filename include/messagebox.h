/*
 * messagebox.h
 *
 *  Created on: 14/10/2014
 *      Author: giovani
 */

#ifndef MESSAGEBOX_H_
#define MESSAGEBOX_H_

//Cross-platform message box method.
#include <string>
#include <vector>

//Helper macro
#define MB_BUTTONS(...) ((char*[]) {__VA_ARGS__})

typedef struct SDL_Window SDL_Window;

//Flexible message box function.
//Returns the index of button pressed on success or a negative value on a failure.
//The parent argument can be set to NULL if not available.
int showMessageBox(SDL_Window *parent, std::string msg, std::string title, std::vector<std::string> buttons, int defbutton = 0);


#endif /* MESSAGEBOX_H_ */
