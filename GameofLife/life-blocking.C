#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <mpi.h>
using namespace std;

/*
 * Reads the input file line by line and stores it in a 2D matrix.
 */
void read_input_file(int **life, string const &input_file_name) {
    
    // Open the input file for reading.
    ifstream input_file;
    input_file.open(input_file_name);
    if (!input_file.is_open())
        perror("Input file cannot be opened");

    string line, val;
    int x, y;
    while (getline(input_file, line)) {
        stringstream ss(line);
        
        // Read x coordinate.
        getline(ss, val, ',');
        x = stoi(val);
        
        // Read y coordinate.
        getline(ss, val);
        y = stoi(val);

        // Populate the life matrix.
        life[x][y] = 1;
    }
    input_file.close();
}

/* 
 * Writes out the final state of the 2D matrix to a csv file. 
 */
void write_output(int **result_matrix, int X_limit, int Y_limit,
                  string const &input_name, int num_of_generations) {
    
    // Open the output file for writing.
    ofstream output_file;
    string input_file_name = input_name.substr(0, input_name.length() - 5);
    output_file.open(input_file_name + "." + to_string(num_of_generations) +
                    ".csv");
    if (!output_file.is_open())
        perror("Output file cannot be opened");
    
    // Output each live cell on a new line. 
    for (int i = 0; i < X_limit; i++) {
        for (int j = 0; j < Y_limit; j++) {
            if (result_matrix[i][j] == 1) {
                output_file << i << "," << j << "\n";
            }
        }
    }
    output_file.close();
}

/*
 * Processes the life array for the specified number of iterations.
 */
void compute(int **life, int **previous_life, int X_limit, int Y_limit, int elements_per_proc, int rank, int size) {
    int neighbors = 0;
    int bottom_right, bottom_middle, bottom_left, top_right, top_middle, top_left;
    int *new_data_up = new int[X_limit+2];
    int *new_data_down = new int[X_limit+2];
    int *data = new int[X_limit+2];
    int *new_data = new int[X_limit+2];

    // Update the previous_life matrix with the current life matrix state.
    for (int i = 0; i < X_limit; i++) {
        for (int j = 0; j < Y_limit; j++) {
            previous_life[i + 1][j + 1] = life[i][j];
        }
    }


    // There's padding and edges don't wrap around so just check if it's 0 or 1!

    // For simulating each generation, calculate the number of live
    // neighbors for each cell and then determine the state of the cell in
    // the next iteration.

    //if top of the grid

    if (rank == 0) {
        printf("rank is %d", rank);

        for (int i = 1; i < X_limit + 1; i++) {
        for (int j = 1; j < Y_limit + 1; j++) {

            if (j == Y_limit) {

                printf("top of grid last row w/ col %d\n", i);

                data = previous_life[i];
                MPI_Send(&data, X_limit + 2, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
                MPI_Recv(&new_data, X_limit + 2, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                bottom_left = new_data_down[i-1];
                bottom_middle = new_data_down[i];
                bottom_right = new_data_down[i+1];

                neighbors = previous_life[i - 1][j - 1] + previous_life[i - 1][j] + bottom_left + previous_life[i][j - 1] + bottom_middle + previous_life[i + 1][j - 1] + previous_life[i + 1][j] + bottom_right;

            } else {
                neighbors = previous_life[i - 1][j - 1] + previous_life[i - 1][j] + previous_life[i - 1][j + 1] + previous_life[i][j - 1] + previous_life[i][j + 1] + previous_life[i + 1][j - 1] + previous_life[i + 1][j] + previous_life[i + 1][j + 1];
            }


            if (previous_life[i][j] == 0) {
                // A cell is born only when an unoccupied cell has 3 neighbors.
                if (neighbors == 3)
                    life[i - 1][j - 1] = 1;
            } else {
                // An occupied cell survives only if it has either 2 or 3 neighbors.
                // The cell dies out of loneliness if its neighbor count is 0 or 1.
                // The cell also dies of overpopulation if its neighbor count is 4-8.
                if (neighbors != 2 && neighbors != 3) {
                    life[i - 1][j - 1] = 0;
                }
            }
        }}
    // if bottom of the grid
    } else if (rank == (size - 1)) {

        printf("%d is the rank bottom line \n", rank);

        for (int i = 1; i < X_limit + 1; i++) {
        for (int j = 1; j < Y_limit + 1; j++) {

            if (j == 1) {
                printf("bottom of the grid row 1\n");

            

            data = previous_life[i];

            MPI_Recv(&new_data, X_limit, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Send(&data, X_limit, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);


            top_left = new_data_up[j-1];
            top_middle = new_data_up[j];
            top_right = new_data_up[j+1];

            neighbors = top_left + previous_life[i - 1][j] + previous_life[i - 1][j + 1] + top_middle + previous_life[i][j + 1] + top_right + previous_life[i + 1][j] + previous_life[i + 1][j + 1];
            } else {
            neighbors = previous_life[i - 1][j - 1] + previous_life[i - 1][j] + previous_life[i - 1][j + 1] + previous_life[i][j - 1] + previous_life[i][j + 1] + previous_life[i + 1][j - 1] + previous_life[i + 1][j] + previous_life[i + 1][j + 1];
            
            }

            
            if (previous_life[i][j] == 0) {
                // A cell is born only when an unoccupied cell has 3 neighbors.
                if (neighbors == 3)
                    life[i - 1][j - 1] = 1;
            } else {
                // An occupied cell survives only if it has either 2 or 3 neighbors.
                // The cell dies out of loneliness if its neighbor count is 0 or 1.
                // The cell also dies of overpopulation if its neighbor count is 4-8.
                if (neighbors != 2 && neighbors != 3) {
                    life[i - 1][j - 1] = 0;
                }
            }
        }}


    } else if (rank % 2 == 0) {
        printf("%d is the rank divisible by 2 \n", rank);

        for (int i = 1; i < X_limit + 1; i++) {
        for (int j = 1; j < Y_limit + 1; j++) {

            if (j == 1 || j == Y_limit) {
                printf("at an edge in col %d\n", i);
            

            data = previous_life[i];

            top_left = new_data_up[j-1];
            top_middle = new_data_up[j];
            top_right = new_data_up[j+1];
            bottom_left = new_data_down[j-1];
            bottom_middle = new_data_down[j];
            bottom_right = new_data_down[j+1];

            MPI_Recv(&new_data_up, X_limit, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&new_data_down, X_limit, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            MPI_Send(&data, X_limit, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(&data, X_limit, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);


            neighbors = top_left + previous_life[i - 1][j] + bottom_left + top_middle + bottom_middle + top_right + previous_life[i + 1][j] + bottom_right;
            } else {
                neighbors = previous_life[i - 1][j - 1] + previous_life[i - 1][j] + previous_life[i - 1][j + 1] + previous_life[i][j - 1] + previous_life[i][j + 1] + previous_life[i + 1][j - 1] + previous_life[i + 1][j] + previous_life[i + 1][j + 1];
            }

            if (previous_life[i][j] == 0) {
                // A cell is born only when an unoccupied cell has 3 neighbors.
                if (neighbors == 3)
                    life[i - 1][j - 1] = 1;
            } else {
                // An occupied cell survives only if it has either 2 or 3 neighbors.
                // The cell dies out of loneliness if its neighbor count is 0 or 1.
                // The cell also dies of overpopulation if its neighbor count is 4-8.
                if (neighbors != 2 && neighbors != 3) {
                    life[i - 1][j - 1] = 0;
                }
            }
        }}
    
    } else {
        printf("%d is the rank not divisible by 2 \n", rank);

        for (int i = 1; i < X_limit + 1; i++) {
        for (int j = 1; j < Y_limit + 1; j++) {

            if (j == 1 || j == Y_limit) {
                printf("at edge in col %d\n", i);

            
            MPI_Send(data, X_limit, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
            MPI_Send(data, X_limit, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);

            MPI_Recv(new_data_up, X_limit, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(new_data_down, X_limit, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            

            top_left = new_data_up[j-1];
            top_middle = new_data_up[j];
            top_right = new_data_up[j+1];
            bottom_left = new_data_down[j-1];
            bottom_middle = new_data_down[j];
            bottom_right = new_data_down[j+1];

            neighbors = top_left + previous_life[i - 1][j] + bottom_left + top_middle + bottom_middle + top_right + previous_life[i + 1][j] + bottom_right;

            } else {
                neighbors = previous_life[i - 1][j - 1] + previous_life[i - 1][j] + previous_life[i - 1][j + 1] + previous_life[i][j - 1] + previous_life[i][j + 1] + previous_life[i + 1][j - 1] + previous_life[i + 1][j] + previous_life[i + 1][j + 1];
            }

            

            if (previous_life[i][j] == 0) {
                // A cell is born only when an unoccupied cell has 3 neighbors.
                if (neighbors == 3)
                    life[i - 1][j - 1] = 1;
            } else {
                // An occupied cell survives only if it has either 2 or 3 neighbors.
                // The cell dies out of loneliness if its neighbor count is 0 or 1.
                // The cell also dies of overpopulation if its neighbor count is 4-8.
                if (neighbors != 2 && neighbors != 3) {
                    life[i - 1][j - 1] = 0;
                }
            }
        }}
    }
}

/**
  * The main function to execute "Game of Life" simulations on a 2D board.
  */
int main(int argc, char *argv[]) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    


    if (argc != 5)
        perror("Expected arguments: ./life <input_file> <num_of_generations> <X_limit> <Y_limit>");

    string input_file_name = argv[1];
    int num_of_generations = stoi(argv[2]);
    int X_limit = stoi(argv[3]);
    int Y_limit = stoi(argv[4]);
    int number_of_elements = X_limit * Y_limit;
    int elements_per_proc = number_of_elements / size;
    int new_Y_limit = Y_limit / size;
    float total_max, total_min, total_sum, time;


    // Initialize the board
    int **life = new int *[X_limit];
    for (int i = 0; i < X_limit; i++) {
        life[i] = new int[Y_limit];
        for (int j = 0; j < Y_limit; j++) {
            life[i][j] = 0;
        }
    }

    // Use previous_life to track the pervious state of the board.
    // Pad the previous_life matrix with 0s on all four sides by setting all
    // cells in the following rows and columns to 0:
    //  1. Row 0
    //  2. Column 0
    //  3. Row X_limit+1
    //  4. Column Y_limit+1
    int **previous_life = new int *[X_limit+2];
    for (int i = 0; i < X_limit+2; i++) {
        previous_life[i] = new int[Y_limit+2];
        for (int j = 0; j < Y_limit+2; j++) {
            previous_life[i][j] = 0;
        }
    }

    

    if (rank == 0) {
        read_input_file(life, input_file_name);
    }

/*
    1. First you will have to come up with identifying what portion of the grid each rank works on (they should be equal portions) so I would suggest come up with that scheme
    2. Next, you need to actually distribute the data from the 0th process (which reads the entire input) to the other processes so that they get the portion they are supposed to work on. You will use MPI_Scatter/Gather for these things
    3. Another alternative to 2. is to have all processes read the input file and filter out what each rank cares about 
*/

    // Each processes gets n/p elements

    int **new_block = new int *[X_limit];
    for (int i = 0; i < X_limit; i++) {
        new_block[i] = new int[new_Y_limit];
        for (int j = 0; j < new_Y_limit; j++) {
            new_block[i][j] = 0;
        }
    }


    if (rank == 0) {
        MPI_Scatter(life, Y_limit, MPI_INT, new_block, new_Y_limit, MPI_INT, 0, MPI_COMM_WORLD);
    }

    int **new_block_copy = new int *[X_limit+2];
    for (int i = 0; i < X_limit+2; i++) {
        new_block_copy[i] = new int[new_Y_limit+2];
        for (int j = 0; j < new_Y_limit+2; j++) {
            new_block_copy[i][j] = 0;
        }
    }

    double starttime, endtime;
    starttime = MPI_Wtime();

    // Go through this the input amount of generations
    for (int numg = 0; numg < num_of_generations; numg++) {
        compute(new_block, new_block_copy, X_limit, new_Y_limit, elements_per_proc, rank, size);
    }


    endtime = MPI_Wtime();
    time = endtime - starttime;

    cout << "Runtime: " << float(endtime - starttime) / CLOCKS_PER_SEC << "s for process\n";

    
    MPI_Gather(new_block, new_Y_limit, MPI_INT, life, new_Y_limit, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {

        MPI_Reduce(&time, &total_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&time, &total_min, 1, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
        MPI_Reduce(&time, &total_max, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

        write_output(life, X_limit, Y_limit, input_file_name, num_of_generations);
        cout << "TIME: Min: " << total_min << " s Avg: " << float (total_sum / number_of_elements) << " s Max: " << total_max << " s\n";
    }

    // Write out the final state to the output file.


    for (int i = 0; i < X_limit; i++) {
        delete life[i];
    }
    for (int i = 0; i < X_limit+2; i++) {
        delete previous_life[i];
    }
    delete[] life;
    delete[] previous_life;

    MPI_Finalize();
    return 0;
}

