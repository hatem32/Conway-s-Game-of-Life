#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h> 

using namespace std;

class Universe {
private:
    int rows, cols;
    vector<vector<bool>> grid; // 2D grid to store cell states

public:
    // Constructor to initialize the universe
    Universe(int r, int c) : rows(r), cols(c) {
        grid.resize(rows, vector<bool>(cols, false));
    }

    // Function to initialize the universe with random cells (alive or dead)
    void initialize() {
        srand(time(0)); // Seed for random number generation
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = rand() % 2; // Randomly set cell as alive or dead
            }
        }
        display();
    }

    // Function to reset the grid (set all cells as dead)
    void reset() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = false;
            }
        }
        display();
    }

    // Function to count alive neighbors of a given cell
    int count_neighbors(int x, int y) {
        int count = 0;
        // Check 8 surrounding cells (up, down, left, right, and diagonals)
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue; // Skip the cell itself
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                    if (grid[nx][ny]) {
                        count++;
                    }
                }
            }
        }
        return count;
    }

    // Function to generate the next state of the universe
    void next_generation() {
        vector<vector<bool>> new_grid = grid; // Copy the grid to create the new generation
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int neighbors = count_neighbors(i, j);
                if (grid[i][j]) {
                    // Live cell with fewer than 2 neighbors dies (underpopulation)
                    // or more than 3 neighbors dies (overpopulation)
                    if (neighbors < 2 || neighbors > 3) {
                        new_grid[i][j] = false;
                    }
                } else {
                    // Dead cell with exactly 3 neighbors becomes alive (reproduction)
                    if (neighbors == 3) {
                        new_grid[i][j] = true;
                    }
                }
            }
        }
        grid = new_grid; // Update the grid with the new generation
    }

    // Function to display the current state of the universe
    void display(bool showNumbers = false) {
        system("cls"); // Clear the screen (works on Windows, use "clear" on Unix-like systems)
        cout << "Game of Life - Current State" << endl;

        int cell_number = 1; // Start numbering cells
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j]) {
                    cout << "O "; // Alive cell
                } else if (showNumbers) {
                    cout << cell_number << " "; // Show cell number
                } else {
                    cout << ". "; // Dead cell
                }
                ++cell_number;
            }
            cout << endl;
        }
    }

    // Function to start the game for a certain number of generations
    void run(int generations) {
        for (int i = 0; i < generations; ++i) {
            display(); // Show current state
            next_generation(); // Generate next state
            Sleep(500); // Wait for 500ms before the next generation (Windows-specific)
        }
    }

    // Function to set a custom pattern based on user input (using cell numbers)
    void set_custom_pattern() {
        int total_cells = rows * cols;
        int cell_number;
        char choice;

        do {
            cout << "Enter cell number to make alive (1 to " << total_cells << "): ";
            cin >> cell_number;

            if (cell_number < 1 || cell_number > total_cells) {
                cout << "Invalid cell number! Please try again." << endl;
            } else {
                int x = (cell_number - 1) / cols; // Calculate row from cell number
                int y = (cell_number - 1) % cols; // Calculate column from cell number
                grid[x][y] = true; // Set the corresponding cell as alive
                cout << "Cell (" << x << ", " << y << ") set to alive." << endl;
            }

            display(); // Show updated grid
            cout << "Do you want to set another cell? (y/n): ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');
    }
};

int main() {
    int rows = 20, cols = 50;
    Universe game(rows, cols);
    game.display();

    int choice;
    do {
        cout << "Game of Life Menu:" << endl;
        cout << "1. Set Custom Live Cells by Cell Number" << endl;
        cout << "2. Initialize Random Grid" << endl;
        cout << "3. Run Simulation for 20 generations" << endl;
        cout << "4. Reset Grid (All cells dead)" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                game.set_custom_pattern();
                break;
            case 2:
                game.initialize();
                break;
            case 3:
                game.run(20); // Run for 20 generations
                break;
            case 4:
                game.reset();
                break;
            case 5:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    return 0;
}
