#ifndef MASTER_WINDOW_H
#define MASTER_WINDOW_H

#include "../window.h"
#include "../../../list.h"

class Master_window : public Window
{
    List windows = {};
public:
    // Master_window (int list_capacity = INIT_WINDOWS_CAPCITY);
    // ~Master_window ()
};

#endif /* MASTER_WINDOW_H */