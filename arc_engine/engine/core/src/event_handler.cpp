//
// Created by adolin on 22. 12. 20.
//

#include "../include/event_handler.h"
namespace arc {
    MouseListener EventHandler::null_mouse_listener_;
    void **EventHandler::null_mouse_vtable_;
    KeyboardListener EventHandler::null_keyboard_listener_;
    void **EventHandler::null_keyboard_vtable_;
    WindowListener EventHandler::null_window_listener_;
    void **EventHandler::null_window_vtable_;
    TouchListener EventHandler::null_touch_listener_;
    void **EventHandler::null_touch_vtable_;

    //TODO: make these vectors reserve a specific number of space
    std::pair<std::vector<std::pair<EventHandler::EventIntFn *, void *>>, int> EventHandler::button_down_fns_;
    std::pair<std::vector<std::pair<EventHandler::EventIntFn *, void *>>, int> EventHandler::button_up_fns_;
    std::pair<std::vector<std::pair<EventHandler::EventDoubleDoubleFn *, void *>>, int> EventHandler::scroll_fns_;
    std::pair<std::vector<std::pair<EventHandler::EventDoubleDoubleFn *, void *>>, int> EventHandler::move_fns_;

    std::pair<std::vector<std::pair<EventHandler::EventIntBoolFn *, void *>>, int> EventHandler::key_down_fns_;
    std::pair<std::vector<std::pair<EventHandler::EventIntFn *, void *>>, int> EventHandler::key_up_fns_;
    std::pair<std::vector<std::pair<EventHandler::EventIntFn *, void *>>, int> EventHandler::char_fns_;

    std::pair<std::vector<std::pair<EventHandler::EventIntIntFn *, void *>>, int> EventHandler::resize_fns_;
    std::pair<std::vector<std::pair<EventHandler::EventFn *, void *>>, int> EventHandler::close_fns_;

    std::pair<std::vector<std::pair<EventHandler::EventIntFloatFloatFn *, void*>>,int> EventHandler::touch_down_fns_;
    std::pair<std::vector<std::pair<EventHandler::EventIntFloatFloatFn *, void*>>,int> EventHandler::touch_up_fns_;
    std::pair<std::vector<std::pair<EventHandler::EventIntFloatFloatFn* , void*>>,int> EventHandler::touch_move_fns_;
    std::pair<std::vector<std::pair<EventHandler::EventFloatFloatFloatFloatFn * , void*>>,int> EventHandler::multi_gesture_fns_;

}