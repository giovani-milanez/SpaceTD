/*
 * messagebox.cpp
 *
 *  Created on: 14/10/2014
 *      Author: giovani
 */

#include "messagebox.h"

#include <SDL2/SDL.h> //SDL 2.0 header file

int showMessageBox(SDL_Window *parent, std::string msg, std::string title, std::vector<std::string> buttons, int defbutton)
{
    //Variables.
    int resultButton = 0;
    SDL_MessageBoxData mbdata;

    //Set the message box information.
    mbdata.flags = SDL_MESSAGEBOX_INFORMATION;
    mbdata.message = msg.c_str();
    mbdata.title = title.c_str();
    mbdata.colorScheme = NULL;
    mbdata.window = parent;
    mbdata.numbuttons = buttons.size();

    //Allocate buttons.
    SDL_MessageBoxButtonData *butarray = new SDL_MessageBoxButtonData[mbdata.numbuttons];

    //Set the button values.
    for(unsigned char i = 0; i < mbdata.numbuttons; i++)
    {
        //Is this button the default button?
        if(i == defbutton)
        {
            butarray[i].flags = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
        } else
        {
            butarray[i].flags = 0;
        }

        //Set button text.
        if(buttons[i] != "")
        {
            butarray[i].text = buttons[i].c_str();
        }

        //Set button ID.
        butarray[i].buttonid = i;
    }

    //Set the message box data's button array.
    mbdata.buttons = butarray;

    //Display the message box.
    int retval = SDL_ShowMessageBox(&mbdata, &resultButton);

    //Deallocate the buttons array to prevent memory leaks.
    delete[] butarray;

    //Return the result (-1 on failure or if the dialog was closed).
    return retval < 0 ? -1 : resultButton;
}


