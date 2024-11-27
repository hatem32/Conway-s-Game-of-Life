#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <windows.h>

using namespace std;

class Universe {
private:
    int rows, cols;
    vector<vector<bool>> grid;

public:
    Universe(int r, int c) : rows(r), cols(c) {
        grid.resize(rows, vector<bool>(cols, false));
    }

    void initialize() {
        srand(time(0));
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = rand() % 2;
            }
        }
        display();
    }

    void reset() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                grid[i][j] = false;
            }
        }
        display();
    }

    int count_neighbors(int x, int y) const {
        int count = 0;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
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

    void next_generation() {
        vector<vector<bool>> new_grid = grid;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int neighbors = count_neighbors(i, j);
                if (grid[i][j]) {
                    new_grid[i][j] = neighbors == 2 || neighbors == 3;
                } else {
                    new_grid[i][j] = neighbors == 3;
                }
            }
        }
        grid = new_grid;
    }

    void display(bool showNumbers = false) const {
        system("cls");
        cout << "Game of Life - Current State\n\n";

        int cellNumber = 1;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j]) {
                    cout << "O ";
                } else if (showNumbers) {
                    cout << cellNumber << " ";
                } else {
                    cout << ". ";
                }
                ++cellNumber;
            }
            cout << endl;
        }
        cout << endl;
    }

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
                int x = (cell_number - 1) / cols;
                int y = (cell_number - 1) % cols;
                grid[x][y] = true;
                cout << "Cell (" << x << ", " << y << ") set to alive." << endl;
            }

            display();
            cout << "Do you want to set another cell? (y/n): ";
            cin >> choice;
        } while (choice == 'y' || choice == 'Y');
    }

    void load_from_file(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error opening file: " << filename << endl;
            return;
        }

        file >> rows >> cols;
        grid.assign(rows, vector<bool>(cols, false));

        string line;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                char state;
                file >> state;
                grid[i][j] = (state == '1');
            }
        }

        file.close();
        cout << "Pattern loaded successfully from file." << endl;
        display();
    }

    void run(int generations) {
        for (int i = 0; i < generations; ++i) {
            display();
            next_generation();
            Sleep(500);
        }
    }
};

int main() {
    int rows = 20, cols = 50;
    Universe game(rows, cols);

    int choice;
    do {
        cout << "Game of Life Menu:\n";
        cout << "1. Set Custom Live Cells by Cell Number\n";
        cout << "2. Initialize Random Grid\n";
        cout << "3. Run Simulation for N generations\n";
        cout << "4. Reset Grid (All cells dead)\n";
        cout << "5. Load Pattern from File\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                int sizeChoice;
                cout << "Choose a grid size:\n";
                cout << "1. 10x10\n";
                cout << "2. 20x20\n";
                cout << "3. 20x50\n";
                cout << "Enter your choice: ";
                cin >> sizeChoice;

                if (sizeChoice == 1) game = Universe(10, 10);
                else if (sizeChoice == 2) game = Universe(20, 20);
                else if (sizeChoice == 3) game = Universe(20, 50);
                else cout << "Invalid choice! Defaulting to 20x50.\n";
                game.set_custom_pattern();
                break;
            case 2: {
                int sizeChoice;
                cout << "Choose a grid size:\n";
                cout << "1. 10x10\n";
                cout << "2. 20x20\n";
                cout << "3. 20x50\n";
                cout << "Enter your choice: ";
                cin >> sizeChoice;

                if (sizeChoice == 1) game = Universe(10, 10);
                else if (sizeChoice == 2) game = Universe(20, 20);
                else if (sizeChoice == 3) game = Universe(20, 50);
                else cout << "Invalid choice! Defaulting to 20x50.\n";

                game.initialize();
                break;
            }
            case 3: {
                int generations;
                cout << "Enter number of generations: ";
                cin >> generations;
                game.run(generations);
                break;
            }
            case 4:
                game.reset();
                break;
            case 5: {
                string filename;
                cout << "Enter filename to load pattern: ";
                cin >> filename;
                game.load_from_file(filename);
                break;
            }
            case 6:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
