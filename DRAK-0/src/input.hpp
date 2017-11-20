// The MIT License (MIT)
//
// Copyright (c) 2017 Drew Wibbenmeyer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#include "bit_array.hpp"

namespace drak {

    struct InputData {
        bool _p1_key_up;
        bool _p1_key_down;
        bool _p1_key_left;
        bool _p1_key_right;
        bool _p1_key_a;
        bool _p1_key_b;
        bool _p1_key_x;
        bool _p1_key_y;
        bool _p1_key_start;
        bool _p1_key_select;

        bool _p2_key_up;
        bool _p2_key_down;
        bool _p2_key_left;
        bool _p2_key_right;
        bool _p2_key_a;
        bool _p2_key_b;
        bool _p2_key_x;
        bool _p2_key_y;
        bool _p2_key_start;
        bool _p2_key_select;

        short _p1_mouse_x;
        short _p1_mouse_y;
        bool _p1_mouse_left;
        bool _p1_mouse_right;

        short _p2_mouse_y;
        short _p2_mouse_x;
        bool _p2_mouse_left;
        bool _p2_mouse_right;
    };

    constexpr InputData DefaultInputData = InputData{false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,0,0,false,false,0,0,false,false};

    class Input {
    public:
        static constexpr int P1KeyUpBit = 0;
        static constexpr int P1KeyDownBit = 1;
        static constexpr int P1KeyLeftBit = 2;
        static constexpr int P1KeyRightBit = 3;
        static constexpr int P1KeyABit = 4;
        static constexpr int P1KeyBBit = 5;
        static constexpr int P1KeyXBit = 6;
        static constexpr int P1KeyYBit = 7;
        static constexpr int P1KeyStartBit = 8;
        static constexpr int P1KeySelectBit = 9;

        static constexpr int P2KeyUpBit = 10;
        static constexpr int P2KeyDownBit = 11;
        static constexpr int P2KeyLeftBit = 12;
        static constexpr int P2KeyRightBit = 13;
        static constexpr int P2KeyABit = 14;
        static constexpr int P2KeyBBit = 15;
        static constexpr int P2KeyXBit = 16;
        static constexpr int P2KeyYBit = 17;
        static constexpr int P2KeyStartBit = 18;
        static constexpr int P2KeySelectBit = 19;

        static constexpr int P1MouseXBit = 20;
        static constexpr int P1MouseYBit = 20 + 16;
        static constexpr int P1MouseLeftBit = P1MouseYBit + 16;
        static constexpr int P1MouseRightBit = P1MouseLeftBit + 1;

        static constexpr int P2MouseXBit = P1MouseRightBit + 1;
        static constexpr int P2MouseYBit = P2MouseXBit + 16;
        static constexpr int P2MouseLeftBit = P2MouseYBit + 16;
        static constexpr int P2MouseRightBit = P2MouseLeftBit + 1;

    private:
        InputData _current_input;
        gsl::span<unsigned char> _input_buffer_bytes;
        BitArray<11> _input_buffer_bits;

    public:
        Input() { }

        Input(gsl::span<unsigned char> & input_buffer_bytes) {
            _input_buffer_bytes = input_buffer_bytes;
            _input_buffer_bits = BitArray<11>(input_buffer_bytes);
            _current_input = DefaultInputData;
        }

        void Update(sf::RenderWindow & window, sf::View const& view) {
            // Load from SFML
            _current_input._p1_key_up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
            _current_input._p1_key_down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
            _current_input._p1_key_left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
            _current_input._p1_key_right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            _current_input._p1_key_a = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
            _current_input._p1_key_b = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
            _current_input._p1_key_x = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
            _current_input._p1_key_y = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            _current_input._p1_key_start = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
            _current_input._p1_key_select = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

            _current_input._p2_key_up = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
            _current_input._p2_key_down = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
            _current_input._p2_key_left = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
            _current_input._p2_key_right = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
            _current_input._p2_key_a = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
            _current_input._p2_key_b = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
            _current_input._p2_key_x = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
            _current_input._p2_key_y = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            _current_input._p2_key_start = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
            _current_input._p2_key_select = sf::Keyboard::isKeyPressed(sf::Keyboard::D);

            auto mouse_pos_win = sf::Mouse::getPosition(window);
            auto mouse_pos_scrn = window.mapPixelToCoords(mouse_pos_win, view);

            _current_input._p1_mouse_x = static_cast<short>(mouse_pos_scrn.x);
            _current_input._p1_mouse_y = static_cast<short>(mouse_pos_scrn.y);
            _current_input._p1_mouse_left = sf::Mouse::isButtonPressed(sf::Mouse::Left);
            _current_input._p1_mouse_right = sf::Mouse::isButtonPressed(sf::Mouse::Right);

            _current_input._p2_mouse_x = static_cast<short>(mouse_pos_scrn.x);
            _current_input._p2_mouse_y = static_cast<short>(mouse_pos_scrn.y);
            _current_input._p2_mouse_left = sf::Mouse::isButtonPressed(sf::Mouse::Left);
            _current_input._p2_mouse_right = sf::Mouse::isButtonPressed(sf::Mouse::Right);

            // Upload to memory
            _input_buffer_bits.SetBit(P1KeyUpBit, _current_input._p1_key_up);
            _input_buffer_bits.SetBit(P1KeyDownBit, _current_input._p1_key_down);
            _input_buffer_bits.SetBit(P1KeyLeftBit, _current_input._p1_key_left);
            _input_buffer_bits.SetBit(P1KeyRightBit, _current_input._p1_key_right);

            _input_buffer_bits.SetBit(P1KeyABit, _current_input._p1_key_a);
            _input_buffer_bits.SetBit(P1KeyBBit, _current_input._p1_key_b);
            _input_buffer_bits.SetBit(P1KeyXBit, _current_input._p1_key_x);
            _input_buffer_bits.SetBit(P1KeyYBit, _current_input._p1_key_y);

            _input_buffer_bits.SetBit(P1KeyStartBit, _current_input._p1_key_start);
            _input_buffer_bits.SetBit(P1KeySelectBit, _current_input._p1_key_select);


            _input_buffer_bits.SetBit(P2KeyUpBit, _current_input._p2_key_up);
            _input_buffer_bits.SetBit(P2KeyDownBit, _current_input._p2_key_down);
            _input_buffer_bits.SetBit(P2KeyLeftBit, _current_input._p2_key_left);
            _input_buffer_bits.SetBit(P2KeyRightBit, _current_input._p2_key_right);

            _input_buffer_bits.SetBit(P2KeyABit, _current_input._p2_key_a);
            _input_buffer_bits.SetBit(P2KeyBBit, _current_input._p2_key_b);
            _input_buffer_bits.SetBit(P2KeyXBit, _current_input._p2_key_x);
            _input_buffer_bits.SetBit(P2KeyYBit, _current_input._p2_key_y);

            _input_buffer_bits.SetBit(P2KeyStartBit, _current_input._p2_key_start);
            _input_buffer_bits.SetBit(P2KeySelectBit, _current_input._p2_key_select);


            _input_buffer_bits.SetBitsAt(P1MouseXBit, 8, static_cast<unsigned char>((_current_input._p1_mouse_x & 0xFF00) >> 8));
            _input_buffer_bits.SetBitsAt(P1MouseXBit+8, 8, static_cast<unsigned char>(_current_input._p1_mouse_x & 0x00FF));
            _input_buffer_bits.SetBitsAt(P1MouseYBit, 8, static_cast<unsigned char>((_current_input._p1_mouse_y & 0xFF00) >> 8));
            _input_buffer_bits.SetBitsAt(P1MouseYBit+8, 8, static_cast<unsigned char>(_current_input._p1_mouse_y & 0x00FF));

            _input_buffer_bits.SetBit(P1MouseLeftBit, _current_input._p1_mouse_left);
            _input_buffer_bits.SetBit(P1MouseRightBit, _current_input._p1_mouse_right);


            _input_buffer_bits.SetBitsAt(P2MouseXBit, 8, static_cast<unsigned char>((_current_input._p2_mouse_x & 0xFF00) >> 8));
            _input_buffer_bits.SetBitsAt(P2MouseXBit + 8, 8, static_cast<unsigned char>(_current_input._p2_mouse_x & 0x00FF));
            _input_buffer_bits.SetBitsAt(P2MouseYBit, 8, static_cast<unsigned char>((_current_input._p2_mouse_y & 0xFF00) >> 8));
            _input_buffer_bits.SetBitsAt(P2MouseYBit + 8, 8, static_cast<unsigned char>(_current_input._p2_mouse_y & 0x00FF));

            _input_buffer_bits.SetBit(P2MouseLeftBit, _current_input._p2_mouse_left);
            _input_buffer_bits.SetBit(P2MouseRightBit, _current_input._p2_mouse_right);
        }

        bool IsButtonPressed(int button) const {
            switch(button) {
            case 0:
                return _current_input._p1_key_up;
                break;
            case 1:
                return _current_input._p1_key_down;
                break;
            case 2:
                return _current_input._p1_key_left;
                break;
            case 3:
                return _current_input._p1_key_right;
                break;
            case 4:
                return _current_input._p1_key_a;
                break;
            case 5:
                return _current_input._p1_key_b;
                break;
            case 6:
                return _current_input._p1_key_x;
                break;
            case 7:
                return _current_input._p1_key_y;
                break;
            case 8:
                return _current_input._p1_key_start;
                break;
            case 9:
                return _current_input._p1_key_select;
                break;
            case 10:
                return _current_input._p2_key_up;
                break;
            case 11:
                return _current_input._p2_key_down;
                break;
            case 12:
                return _current_input._p2_key_left;
                break;
            case 13:
                return _current_input._p2_key_right;
                break;
            case 14:
                return _current_input._p2_key_a;
                break;
            case 15:
                return _current_input._p2_key_b;
                break;
            case 16:
                return _current_input._p2_key_x;
                break;
            case 17:
                return _current_input._p2_key_y;
                break;
            case 18:
                return _current_input._p2_key_start;
                break;
            case 19:
                return _current_input._p2_key_select;
                break;
            default:
                // Really should do some sort of error
                return false;
                break;
            }
        }

        bool IsMouseButtonPressed(int button) {
            switch(button) {
            case 0:
                return _current_input._p1_mouse_left;
                break;
            case 1:
                return _current_input._p1_mouse_right;
                break;
            case 2:
                return _current_input._p2_mouse_left;
                break;
            case 3:
                return _current_input._p2_mouse_right;
                break;
            }
        }

        short GetMousePos(int axis) {
            switch(axis) {
            case 0:
                return _current_input._p1_mouse_x;
                break;
            case 1:
                return _current_input._p1_mouse_y;
                break;
            case 2:
                return _current_input._p2_mouse_x;
                break;
            case 3:
                return _current_input._p2_mouse_y;
                break;
            }
        }
    };

}
