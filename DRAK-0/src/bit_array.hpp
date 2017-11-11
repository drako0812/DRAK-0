#pragma once

#include <cstring>
#include <cassert>
#include <bitset>

namespace drak {

    template <unsigned int Bytes>
    class BitArray {
        unsigned char _data[Bytes];

    public:
        BitArray() {
            memset(_data, 0x00, Bytes);
        }

        void SetAll() {
            memset(_data, 0xFF, Bytes);
        }

        void ClearAll() {
            memset(_data, 0x00, Bytes);
        }

        void SetBit(unsigned int idx) {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: SetBit given an out-of-bounds index");
#endif
            _data[idx / 8] |= 1 << (idx % 8);
        }

        void ClearBit(unsigned int idx) {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: ClearBit given an out-of-bounds index");
#endif
            _data[idx / 8] &= ~(1 << (idx % 8));
        }

        bool TestBit(unsigned int idx) {
#ifdef _DEBUG
            assert(((idx / 8) < Bytes) && "ERROR: TestBit given an out-of-bounds index");
#endif
            return ((_data[idx / 8] & (1 << (idx % 8))) != 0);
        }

        unsigned char GetBitsAt(unsigned int idx, unsigned char num_bits) {
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
    };

}
