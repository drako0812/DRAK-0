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

#include <cstring>
#include <cassert>
#include <bitset>

namespace drak {

    template <unsigned int Bytes>
    class BitArray {
        gsl::span<unsigned char> _data;
        std::unique_ptr<std::array<unsigned char, Bytes>> _raw;

    public:
        BitArray(gsl::span<unsigned char> & source_memory = gsl::span<unsigned char>()) {
            if(source_memory.empty()) {
                _raw = std::make_unique<std::array<unsigned char, Bytes>>();
                _data = gsl::span<unsigned char>{*_raw};
                memset(_data.data(), 0, Bytes);
            } else {
                _raw = nullptr;
                _data = source_memory;
            }
        }

        BitArray(BitArray const& bits) {
            if(bits._raw != nullptr) {
                _raw = std::make_unique<std::array<unsigned char, Bytes>>();
                memcpy(_raw->data(), bits._raw->data(), Bytes);
                _data = bits._data;
            } else {
                _raw = nullptr;
                _data = bits._data;
            }
        }

        BitArray operator =(BitArray const& bits) {
            if(bits._raw != nullptr) {
                _raw = std::make_unique<std::array<unsigned char, Bytes>>();
                memcpy(_raw->data(), bits._raw->data(), Bytes);
                _data = bits._data;
            } else {
                _raw = nullptr;
                _data = bits._data;
            }
            return *this;
        }

        void SetAll() {
            memset(_data.data(), 0xFF, Bytes);
        }

        void ClearAll() {
            memset(_data.data(), 0x00, Bytes);
        }

        void SetBit(unsigned int idx) {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: SetBit given an out-of-bounds index");
#endif
            //_data[idx / 8] |= 1 << (idx % 8);
            _data[idx >> 3] |= 1 << (idx & 7);
        }

        void ClearBit(unsigned int idx) {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: ClearBit given an out-of-bounds index");
#endif
            _data[idx / 8] &= ~(1 << (idx % 8));
        }

        void SetBit(unsigned int idx, bool val) {
            if(val) {
                SetBit(idx);
            } else {
                ClearBit(idx);
            }
        }

        bool TestBit(unsigned int idx) const {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: TestBit given an out-of-bounds index");
#endif
            return ((_data[idx / 8] & (1 << (idx % 8))) != 0);
        }

        unsigned char GetBitsAt(unsigned int idx, unsigned char num_bits) const {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: GetBitsAt given an out-of-bounds index");
            assert((num_bits <= 8) && "ERROR: GetBitsAt given an out-of-bounds number of bits");
#endif
            std::bitset<8> new_bits;
            for(int i = 0; i < num_bits; i++) {
                new_bits[i] = TestBit(idx + i);
            }
            new_bits >>= num_bits - 8;
            return static_cast<unsigned char>(new_bits.to_ulong());
        }

        void SetBitsAt(unsigned int idx, unsigned char num_bits, unsigned char value) {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: SetBitsAt given an out-of-bounds index");
            assert((num_bits <= 8) && "ERROR: SetBitsAt given an out-of-bounds number of bits");
#endif
            std::bitset<8> new_bits = value;
            for(int i = idx + num_bits - 1, j = 7; i > idx; i--, j--) {
                //new_bits[i] = TestBit(idx + i);
                if(new_bits[j]) {
                    SetBit(i);
                } else {
                    ClearBit(i);
                }
            }
        }

        // Optimization for 6-bit screenbuffer
        unsigned char Get6BitsAt(unsigned int idx) const {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: Get6BitsAt given an out-of-bounds index");
#endif
            unsigned char b0, b1, b2, b3, b4, b5;
            b0 = TestBit(idx + 5) ? 1 : 0;
            b1 = (TestBit(idx + 4) ? 1 : 0) << 1;
            b2 = (TestBit(idx + 3) ? 1 : 0) << 2;
            b3 = (TestBit(idx + 2) ? 1 : 0) << 3;
            b4 = (TestBit(idx + 1) ? 1 : 0) << 4;
            b5 = (TestBit(idx) ? 1 : 0) << 5;
            return b0 | b1 | b2 | b3 | b4 | b5;
        }

        // Optimization for 6-bit screenbuffer
        void Set6BitsAt(unsigned int idx, unsigned char value) {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: Set6BitsAt given an out-of-bounds index");
#endif
            bool b0, b1, b2, b3, b4, b5;
            b0 = (0b000001 & value) == 1;
            b1 = ((0b000010 & value) >> 1) == 1;
            b2 = ((0b000100 & value) >> 2) == 1;
            b3 = ((0b001000 & value) >> 3) == 1;
            b4 = ((0b010000 & value) >> 4) == 1;
            b5 = ((0b100000 & value) >> 5) == 1;
            SetBit(idx, b5);
            SetBit(idx + 1, b4);
            SetBit(idx + 2, b3);
            SetBit(idx + 3, b2);
            SetBit(idx + 4, b1);
            SetBit(idx + 5, b0);
        }

        // Optimization for 13-bit sprite index
        unsigned short Get13BitsAt(unsigned int idx) const {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: Get13BitsAt given an out-of-bounds index");
#endif
            unsigned char b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12;
            b0 = TestBit(idx + 12) ? 1 : 0;
            b1 = (TestBit(idx + 11) ? 1 : 0) << 1;
            b2 = (TestBit(idx + 10) ? 1 : 0) << 2;
            b3 = (TestBit(idx + 9) ? 1 : 0) << 3;
            b4 = (TestBit(idx + 8) ? 1 : 0) << 4;
            b5 = (TestBit(idx + 7) ? 1 : 0) << 5;
            b6 = (TestBit(idx + 6) ? 1 : 0) << 6;
            b7 = (TestBit(idx + 5) ? 1 : 0) << 7;
            b8 = (TestBit(idx + 4) ? 1 : 0) << 8;
            b9 = (TestBit(idx + 3) ? 1 : 0) << 9;
            b10 = (TestBit(idx + 2) ? 1 : 0) << 10;
            b11 = (TestBit(idx + 1) ? 1 : 0) << 11;
            b12 = (TestBit(idx) ? 1 : 0) << 12;
            return b0 | b1 | b2 | b3 | b4 | b5 | b6 | b7 | b8 | b9 | b10 | b11 | b12;
        }

        // Optimization for 13-bit sprite index
        void Set13BitsAt(unsigned int idx, unsigned short value) {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: Set13BitsAt given an out-of-bounds index");
#endif
            bool b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12;
            b0 = ( 0b0000000000001 & value) == 1;
            b1 = ((0b0000000000010 & value) >> 1) == 1;
            b2 = ((0b0000000000100 & value) >> 2) == 1;
            b3 = ((0b0000000001000 & value) >> 3) == 1;
            b4 = ((0b0000000010000 & value) >> 4) == 1;
            b5 = ((0b0000000100000 & value) >> 5) == 1;
            b6 = ((0b0000001000000 & value) >> 6) == 1;
            b7 = ((0b0000010000000 & value) >> 7) == 1;
            b8 = ((0b0000100000000 & value) >> 8) == 1;
            b9 = ((0b0001000000000 & value) >> 9) == 1;
            b10 = ((0b0010000000000 & value) >> 10) == 1;
            b11 = ((0b0100000000000 & value) >> 11) == 1;
            b12 = ((0b1000000000000 & value) >> 12) == 1;
            SetBit(idx, b12);
            SetBit(idx + 1, b11);
            SetBit(idx + 2, b10);
            SetBit(idx + 3, b9);
            SetBit(idx + 4, b8);
            SetBit(idx + 5, b7);
            SetBit(idx + 6, b6);
            SetBit(idx + 7, b5);
            SetBit(idx + 8, b4);
            SetBit(idx + 9, b3);
            SetBit(idx + 10, b2);
            SetBit(idx + 11, b1);
            SetBit(idx + 12, b0);
        }
    };

}
