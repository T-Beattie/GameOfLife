#include "grid.h"

void grid::create(int row, int column) {
    cells[row][column].create();
}

void grid::draw() {
    std::cout << "\x1b[2J";

    for (int row = 0; row < rowmax; ++row) {
        for (int column = 0; column < colmax; ++column) {
            cells[row][column].draw(row, column);
        }
    }
}

void grid::randomize() {
    const int factor = 5;
    const int cutoff = RAND_MAX / factor;
    time_t now;
    time(&now);
    srand(now); // will seed value to be random, comment this out to test

    for (int row = 1; row < rowmax; ++row) {
        for (int column = 1; column < colmax; ++column) {
            if (rand() / cutoff == 0) {
                create(row, column);
            }
        }
    }
}


bool grid::will_survive(int row, int column) {
    if (!cells[row][column].is_alive()) {
        // There is no cell at this position!
        return false;
    }

    // Find the number of alive neighbours for this cell
    int neighbours = cells[row - 1][column - 1].is_alive() +
        cells[row - 1][column].is_alive() +
        cells[row - 1][column + 1].is_alive() +
        cells[row][column - 1].is_alive() +
        cells[row][column + 1].is_alive() +
        cells[row + 1][column - 1].is_alive() +
        cells[row + 1][column].is_alive() +
        cells[row + 1][column + 1].is_alive();

    if (neighbours < min_neighbours || neighbours > max_neighbours) {
        // Cell has died
        return false;
    }

    // If we get here, the cell has survived
    return true;
}

// Will a cell be born at (row, column) in the next generation?
bool grid::will_create(int row, int column) {
    if (cells[row][column].is_alive()) {
        // There already is a cell at this position!
        return false;
    }

    // Find the number of alive parents for this cell
    int parents = cells[row - 1][column - 1].is_alive() +
        cells[row - 1][column].is_alive() +
        cells[row - 1][column + 1].is_alive() +
        cells[row][column - 1].is_alive() +
        cells[row][column + 1].is_alive() +
        cells[row + 1][column - 1].is_alive() +
        cells[row + 1][column].is_alive() +
        cells[row + 1][column + 1].is_alive();

    if (parents < min_parents || parents > max_parents) {
        // Cannot create a cell here
        return false;
    }

    // If we get here, a new cell can be born
    return true;
}

// Update to the next generation
void grid::update(const grid& next) {
    for (int row = 1; row < rowmax; ++row) {
        for (int column = 1; column < colmax; ++column) {
            cells[row][column] = next.cells[row][column];
        }
    }
}

// By default, all cells in the next generation are initially unpopulated
// Calculate which live cells survive to the next generation
// and unpopulated cells are populated in the next generation
void calculate(grid& old_generation, grid& new_generation) {
    for (int row = 1; row < rowmax; ++row) {
        for (int column = 1; column < colmax; ++column) {
            // Will this live cell survive to the next generation?
            if (old_generation.will_survive(row, column)) {
                new_generation.create(row, column);
            }
            // Will this unpopulated cell be populated in the next generation?
            else if (old_generation.will_create(row, column)) {
                new_generation.create(row, column);
            }
        }
    }
}
