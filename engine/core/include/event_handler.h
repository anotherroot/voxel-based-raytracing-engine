//
// Created by adolin on 22. 12. 20.
//

#ifndef ARC_ENGINE_EVENT_HANDLER_H
#define ARC_ENGINE_EVENT_HANDLER_H

#include <set>
#include "listeners.h"

namespace arc{
    class Engine;
    class EventHandler : public Caller{
        typedef bool (EventFn )(void*);
        typedef bool (EventIntFn )(void*, int);
        typedef bool (EventIntIntFn )(void*, int, int);
        typedef bool (EventDoubleDoubleFn)(void*, double, double);
        typedef bool (EventIntBoolFn)(void*, int, bool);
        typedef bool (EventIntFloatFloatFn)(void*, int, float, float);
        typedef bool (EventFloatFloatFloatFloatFn)(void*, float, float, float, float);


        static MouseListener null_mouse_listener_;
        static void** null_mouse_vtable_;
        static KeyboardListener null_keyboard_listener_;
        static void** null_keyboard_vtable_;
        static WindowListener null_window_listener_;
        static void** null_window_vtable_;
        static TouchListener null_touch_listener_;
        static void** null_touch_vtable_;

        static std::pair<std::vector<std::pair<EventIntFn *, void*>>,int> button_down_fns_;
        static std::pair<std::vector<std::pair<EventIntFn *, void*>>,int> button_up_fns_;
        static std::pair<std::vector<std::pair<EventDoubleDoubleFn*, void*>>,int> scroll_fns_;
        static std::pair<std::vector<std::pair<EventDoubleDoubleFn*, void*>>,int> move_fns_;

        static std::pair<std::vector<std::pair<EventIntBoolFn*, void*>>,int> key_down_fns_;
        static std::pair<std::vector<std::pair<EventIntFn*, void*>>,int> key_up_fns_;
        static std::pair<std::vector<std::pair<EventIntFn*, void*>>,int> char_fns_;

        static std::pair<std::vector<std::pair<EventIntIntFn*, void*>>,int> resize_fns_;
        static std::pair<std::vector<std::pair<EventFn*, void*>>,int> close_fns_;

        static std::pair<std::vector<std::pair<EventIntFloatFloatFn*, void*>>,int> touch_down_fns_;
        static std::pair<std::vector<std::pair<EventIntFloatFloatFn*, void*>>,int> touch_up_fns_;
        static std::pair<std::vector<std::pair<EventIntFloatFloatFn*, void*>>,int> touch_move_fns_;
        static std::pair<std::vector<std::pair<EventFloatFloatFloatFloatFn *, void*>>,int> multi_gesture_fns_;

    public:
        enum Order{
            front, second, back
        };


        static void SubscribeMouse(MouseListener* listener, Order order = back){
            auto button_down = GetVTable(listener)[0];
            auto button_up = GetVTable(listener)[1];
            auto scroll = GetVTable(listener)[2];
            auto move = GetVTable(listener)[3];



            if(button_down != null_mouse_vtable_[0]){
                Insert(button_down_fns_,button_down,listener,order);
            }
            if(button_up != null_mouse_vtable_[1]){
                Insert(button_up_fns_,button_up,listener,order);
            }
            if(scroll != null_mouse_vtable_[2]){
                Insert(scroll_fns_, scroll,listener,order);
            }
            if(move != null_mouse_vtable_[3]){
                Insert(move_fns_,move,listener,order);
            }
        }
        static void SubscribeKeyboard(KeyboardListener* listener, Order order = back){
            auto key_down = GetVTable(listener)[0];
            auto key_up = GetVTable(listener)[1];
            auto on_char = GetVTable(listener)[2];


            if(key_down != null_keyboard_vtable_[0]){
                Insert(key_down_fns_, key_down, listener,order);
            }
            if(key_up != null_keyboard_vtable_[1]){
                Insert(key_up_fns_, key_up, listener,order);
            }
            if(on_char != null_keyboard_vtable_[2]){
                Insert(char_fns_, on_char,listener,order);
            }
        }

        static void SubscribeWindow(WindowListener* listener, Order order = back){
            auto resize = GetVTable(listener)[0];
            auto close = GetVTable(listener)[1];


            if(resize != null_window_vtable_[0]){
                Insert(resize_fns_, resize, listener,order);
            }
            if(close != null_window_vtable_[1]){
                Insert(close_fns_, close, listener,order);
            }
        }

        static void SubscribeTouch(TouchListener* listener, Order order = back){
            auto touch_down = GetVTable(listener)[0];
            auto touch_up = GetVTable(listener)[1];
            auto touch_move = GetVTable(listener)[2];
            auto multi_gesture = GetVTable(listener)[3];


            if(touch_down != null_touch_vtable_[0]){
                Insert(touch_down_fns_, touch_down, listener, order);
            }
            if(touch_up != null_touch_vtable_[1]){
                Insert(touch_up_fns_, touch_up, listener, order);
            }
            if(touch_move != null_touch_vtable_[2]){
                Insert(touch_move_fns_, touch_move, listener, order);
            }
            if(multi_gesture != null_touch_vtable_[2]){
                Insert(multi_gesture_fns_, multi_gesture, listener, order);
            }
        }


    private:

        template<typename T>
        static void Insert(std::pair<std::vector<std::pair<T *, void*>>,int>& list,void* funk,void* obj, Order order){
            auto it = list.first.begin();
            if(order == Order::second){
                it+=list.second;
                list.second++;
            }
            else if(order == Order::back){
                it+=list.first.size();
            }
            else{
                list.second++;
            }
            list.first.insert(it,{(T*)funk, obj});
        }

        template<typename T>
        static void InitFunk(std::pair<std::vector<std::pair<T *, void*>>,int>& list){
            list.first.clear();
            list.first.reserve(10);
            list.second = 0;
        }
        static void Init(){
            null_mouse_vtable_ = GetVTable(&null_mouse_listener_);
            null_keyboard_vtable_ = GetVTable(&null_keyboard_listener_);
            null_window_vtable_ = GetVTable(&null_window_listener_);
            null_touch_vtable_ = GetVTable(&null_touch_listener_);

            InitFunk(button_down_fns_);
            InitFunk(button_up_fns_);
            InitFunk(scroll_fns_);
            InitFunk(move_fns_);

            InitFunk(key_down_fns_);
            InitFunk(key_up_fns_);
            InitFunk(char_fns_);

            InitFunk(resize_fns_);
            InitFunk(close_fns_);

            InitFunk(touch_down_fns_);
            InitFunk(touch_up_fns_);
            InitFunk(touch_move_fns_);
            InitFunk(multi_gesture_fns_);


        }

        static void OnButtonDown(int button){
            for (auto& kv:button_down_fns_.first) {
                if(kv.first(kv.second,button)){
                    break;
                }
            }
        }
        static void OnButtonUp(int button){
            for (auto& kv:button_up_fns_.first) {
                if(kv.first(kv.second, button)){
                    break;
                }
            }
        }
        static void OnScroll(double x_offset,double y_offset ){
            for (auto& kv:scroll_fns_.first) {
                if(kv.first(kv.second, x_offset,y_offset)){
                    break;
                }
            }
        }
        static void OnMove(double x_pos, double y_pos){
            for (auto& kv:move_fns_.first) {
                if(kv.first(kv.second,x_pos, y_pos)){
                    break;
                }
            }
        }


        static void OnKeyDown(int key,bool repeat){

            for (auto& kv:key_down_fns_.first) {
                if(kv.first(kv.second,key,repeat)){
                    break;
                }
            }
        }
        static void OnKeyUp(int key){
            for (auto& kv:key_up_fns_.first) {
                if(kv.first(kv.second,key)){
                    break;
                }
            }
        }
        static void OnChar(int key_code){
            for (auto& kv:char_fns_.first) {
                if(kv.first(kv.second, key_code)){
                    break;
                }
            }
        }

        static void OnResize(int width, int height){
            for (auto& kv:resize_fns_.first) {
                if(kv.first(kv.second, width,height)){
                    break;
                }
            }
        }

        static void OnClose(){
            for (auto& kv:close_fns_.first) {
                if(kv.first(kv.second)){
                    break;
                }
            }
        }

        static void OnTouchDown(int id, float x, float y){
            for (auto& kv:touch_down_fns_.first) {
                if(kv.first(kv.second, id, x, y)){
                    break;
                }
            }
        }

        static void OnTouchUp(int id, float x, float y){
            for (auto& kv:touch_up_fns_.first) {
                if(kv.first(kv.second, id, x, y)){
                    break;
                }
            }
        }

        static void OnTouchMove(int id, float dx, float dy){
            for (auto& kv:touch_move_fns_.first) {
                if(kv.first(kv.second, id, dx, dy)){
                    break;
                }
            }
        }

        static void OnMultiGesture(float x, float y, float dist, float theta){
            for (auto& kv:multi_gesture_fns_.first) {
                if(kv.first(kv.second, x, y, dist, theta)){
                    break;
                }
            }
        }

        friend class Window;
        friend class Engine;
    };
}



#endif //ARC_ENGINE_EVENT_HANDLER_H
