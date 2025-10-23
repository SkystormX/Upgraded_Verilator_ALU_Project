#include "VALU.h"                 // Verilator-generated ALU model
#include "verilated.h"            // Verilator simulation core
#include "verilated_vcd_c.h"      // For waveform tracing
#include <iostream>
#include <iomanip>
#include <cstdlib>                // rand()
#include <ctime>                  // time()

int main(int argc, char **argv) {
    // Initialises Verilator environment
    Verilated::commandArgs(argc, argv);

    // Creates an instance of the ALU module
    VALU* top = new VALU;

    // Enables the waveform tracing
    Verilated::traceEverOn(true);           // Enables tracing globally
    VerilatedVcdC* tfp = new VerilatedVcdC; // Creates VCD trace object
    top->trace(tfp, 99);                    // Records all internal signals
    tfp->open("alu_wave.vcd");              // Outputs the VCD file name

    // random number generator
    srand(time(0));

    std::cout << "A\tB\topcode\tResult\tExpected\tMatch\n";
    std::cout << "-----------------------------------------------\n";

    // Runs multiple randomised test cases
    int sim_time = 0; // Tracks simulated time for waveform dumping
    for (int i = 0; i < 10; i++) {
        uint8_t A = rand() % 256;
        uint8_t B = rand() % 256;

        for (int op = 0; op < 8; op++) {
            // Applies inputs to ALU
            top->A = A;
            top->B = B;
            top->opcode = op;

            // Evaluates combinational logic
            top->eval();

            // Computes expected result in software
            uint8_t expected = 0;
            switch (op) {
                case 0: expected = A + B; break;     // ADD
                case 1: expected = A - B; break;     // SUB
                case 2: expected = A & B; break;     // AND
                case 3: expected = A | B; break;     // OR
                case 4: expected = A ^ B; break;     // XOR
                case 5: expected = ~(A | B); break;  // NOR
                case 6: expected = A << 1; break;    // SHIFT LEFT
                case 7: expected = A >> 1; break;    // SHIFT RIGHT
            }

            // Compares ALU result with expected
            bool match = (expected == top->Result);

            // Prints results
            std::cout << std::dec
                      << (int)A << "\t"
                      << (int)B << "\t"
                      << op << "\t"
                      << (int)top->Result << "\t"
                      << (int)expected << "\t"
                      << (match ? "✔️" : "❌") << "\n";

            // Dumps current signals into VCD file
            tfp->dump(sim_time);
            sim_time += 10; // Increments time for next operation
        }
    }

    // Closes the waveform file
    tfp->close();


    delete top;
    return 0;
}
