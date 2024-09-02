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
void compute(int **life, int **previous_life, int X_limit, int Y_limit) {
    int neighbors = 0;

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

    for (int i = 1; i <= X_limit; i++) {
        for (int j = 1; j <= Y_limit; j++) {

            // calculate number of live neighbors for current cell
            neighbors = previous_life[((i - 1) * (Y_limit + 2))][j - 1] +
                        previous_life[(i * (Y_limit + 2))][j - 1] +
                        previous_life[((i + 1) * (Y_limit + 2))][j - 1] +
                        previous_life[((i - 1) * (Y_limit + 2))][j] +
                        previous_life[((i + 1) * (Y_limit + 2))][j] +
                        previous_life[((i - 1) * (Y_limit + 2))][j + 1] +
                        previous_life[(i * (Y_limit + 2))][j + 1] +
                        previous_life[((i + 1) * (Y_limit + 2))][j + 1];

            // cell becomes occupied if empty with three neighbors         
            if (previous_life[(i * (Y_limit + 2))][j] == 0) {
                if (neighbors == 3)
                    life[((i - 1) * Y_limit)][j - 1] = 1;

            // occupied cells that don't have 2-3 neighbors die
            } else {
                if (neighbors < 2 || neighbors > 3) {
                    life[((i - 1) * Y_limit)][j - 1] = 0;
                }
            }
        }
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
    MPI_Request send_req[2], recv_req[2];


    if (argc != 5) {
        perror("Expected arguments: ./life <input_file> <num_of_generations> <X_limit> <Y_limit>");
    }
    string input_file_name = argv[1];
    int num_of_generations = stoi(argv[2]);
    int X_limit = stoi(argv[3]);
    int Y_limit = stoi(argv[4]);
    int number_of_elements = X_limit * Y_limit;
    int elements_per_proc = number_of_elements / size;
    int rows_per_process = Y_limit / size;
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

    
    read_input_file(life, input_file_name);


/*
    1. First you will have to come up with identifying what portion of the grid each rank works on (they should be equal portions) so I would suggest come up with that scheme
    2. Next, you need to actually distribute the data from the 0th process (which reads the entire input) to the other processes so that they get the portion they are supposed to work on. You will use MPI_Scatter/Gather for these things
    3. Another alternative to 2. is to have all processes read the input file and filter out what each rank cares about 
*/

    // Each processes gets n/p elements

    int **curr_life = new int *[X_limit];
    for (int i = 0; i < Y_limit; i++) {
        curr_life[i] = new int[rows_per_process];
        for (int j = 0; j < rows_per_process; j++) {
            curr_life[i][j] = 0;
        }
    }
    

    MPI_Scatter(life, elements_per_proc, MPI_INT, curr_life, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);


    double starttime, endtime;
    starttime = MPI_Wtime();

    for (int i = 0; i < num_of_generations; i++) {
        if (rank > 0) {
            MPI_Isend(curr_life[0], Y_limit, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &send_req[0]);
            MPI_Irecv(previous_life[1], Y_limit, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &recv_req[1]);
            MPI_Wait(&recv_req[1], MPI_STATUS_IGNORE);
        }
        if (rank < size - 1) {
            MPI_Irecv(previous_life[(rows_per_process + 1) * (Y_limit + 2) + 1], Y_limit, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &recv_req[0]);
            MPI_Isend(curr_life[(rows_per_process - 1) * Y_limit], Y_limit, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &send_req[1]);
            MPI_Wait(&recv_req[0], MPI_STATUS_IGNORE);
        }
        compute(curr_life, previous_life, rows_per_process, Y_limit);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    endtime = MPI_Wtime();
    time = endtime - starttime;

    MPI_Gather(curr_life, elements_per_proc, MPI_INT, life, elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Reduce(&time, &total_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
        MPI_Reduce(&time, &total_min, 1, MPI_FLOAT, MPI_MIN, 0, MPI_COMM_WORLD);
        MPI_Reduce(&time, &total_max, 1, MPI_FLOAT, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {


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

