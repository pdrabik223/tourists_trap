//
// Created by pc on 05.05.2021.
//

#ifndef BLOCK_O_AUTOMATA_WIN_COLLORS_H
#define BLOCK_O_AUTOMATA_WIN_COLORS_H
#ifdef WIN32
#include <Windows.h>

#include <fcntl.h>
#include <io.h>
#include <stdio.h> /// for unicode purposes


#include <iostream>
#pragma once



enum color {
    black = 0,
    blue = 1,
    green = 2,
    aqua = 3,
    red = 4,
    purple = 5,
    yellow = 6,
    white = 7,
    gray = 8,
    light_blue = 9,
    light_green = 10,
    light_aqua = 11,
    light_red = 12,
    light_purple = 13,
    light_yellow = 14,
    bright_white = 15

};
/// console color
/// changes the console colors to given in constructor
class cc{
public:
    /// \return changes text color and background color in console
    /// \param text new text color in windows console
    /// \param background new background color in windows console
    /// \possible_colors
    /// <li>black       gray
    /// <li>blue        light_blue
    /// <li>green       light_green
    /// <li>aqua        light_aqua
    /// <li>red         light_red
    /// <li>purple      light_purple
    /// <li>yellow      light_yellow
    /// <li>white       bright_white
    cc(color text, color background);


    /// \return changes text color and background color in console
    /// \param text new text color in windows console
    ///  \possible_colors
    /// <li>black       gray
    /// <li>blue        light_blue
    /// <li>green       light_green
    /// <li>aqua        light_aqua
    /// <li>red         light_red
    /// <li>purple      light_purple
    /// <li>yellow      light_yellow
    /// <li>white       bright_white
    cc(color text);

    friend std::ostream &operator<<(std::ostream &out,const cc& ref );
 //   friend std::wostream &operator<<(std::wostream &out,const cc& ref );

private:
    color text;
    color background;
};

#endif
#endif //BLOCK_O_AUTOMATA_WIN_COLORS_H
