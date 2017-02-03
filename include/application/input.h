#ifndef MESHPROCESSOR_IO_H
#define MESHPROCESSOR_IO_H


#include <wx/event.h>
#include <iostream>

using namespace std;

class Input {
public:
    static void mouseMoved(wxMouseEvent &event) {
        cout << "check\n";
    }

private:
    float mousePosX, mousePosY;
};

#endif
