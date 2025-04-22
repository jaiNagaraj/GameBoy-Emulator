#include "cpu.hpp"

class GheithBoy {
    public:
        void run_gb();
        void load_rom();
    private:
        CPU* cpu;
};