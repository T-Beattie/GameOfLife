#include "grid.h"

#include "ansi_escapes.h"

int main(int argc, char* argv[]) {
    std::cout << "Game of Life\n";
    std::cout << "Press the Enter key to display each generation\n";

    // Wait for user to press the return key
    std::cin.get();

    // Enable ANSI escape codes on Windows
    setupConsole();

    // Grid for the first generation
    grid current_generation;

    // Populate the cells at random
    current_generation.randomize();

    while (true) {
        // Draw the current generation
        current_generation.draw();

        // Wait for user to press the return key
        std::cin.get();

        // Grid for the next generation
        grid next_generation;

        // Populate the cells in the next generation
        calculate(current_generation, next_generation);

        // Update to the next generation
        current_generation.update(next_generation);
    }

    // Move cursor to bottom of screen
    std::cout << "\x1b[" << 0 << ";" << rowmax - 1 << "H";

    // Restore console on Windows
    restoreConsole();
}
