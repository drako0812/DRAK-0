#include "bit_array.hpp"

namespace drak {
    
    class System {
        BitArray<10*1024*1024> _memory;
        
        chaiscript::ChaiScript _scriptEngine;
    public:
        void Initialize() {
            // Initialize standard library
        }
    
        void Update() {
            // Call user's Update function
            
        }
    };
    
}
