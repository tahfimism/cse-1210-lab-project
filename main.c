#include <stdio.h>
#include <stdlib.h>

#define SIZE 30
#define HISTORY_SIZE 11  // Cost recorded every 100 steps over 1000 iterations

// Define a structure to store a data point (x, y)
typedef struct {
    float x;
    float y;
} DataPoint;

// Enum for menu options to improve code readability
typedef enum {
    LOAD_DATA = 1,
    DRAW_PLOT,
    RUN_MODEL,
    SAVE_MODEL,
    LOAD_MODEL,
    SHOW_COST_HISTORY,
    EXIT
} MenuOption;

// Global arrays to store cost history during training
float cost_history[HISTORY_SIZE];
int cost_history_count = 0;

/**
 * Computes the Mean Squared Error (MSE) cost
 * dataset: array of data points
 * n: number of points
 * m, c: model parameters (slope and intercept)
 */
float compute_cost(DataPoint dataset[], int n, float m, float c)
{
    float sum = 0;
    for(int i = 0; i < n; i++){
        float x = dataset[i].x;
        float y = dataset[i].y;

        float y_hat = m*x + c; // predicted y
        float error = y_hat - y;

        sum += error * error;
    }

    return sum / (2 * n);
}

/**
 * Computes gradients of m and c for optimization
 * Uses call-by-reference (*grad_m, *grad_c) to return multiple values
 */
void compute_gradient(DataPoint dataset[], int n, float m, float c, float *grad_m, float *grad_c)
{
    float sum_for_m = 0;
    float sum_for_c = 0;
    for(int i = 0; i < n; i++){
        float x = dataset[i].x;
        float y = dataset[i].y;
        float y_hat = m * x + c;

        sum_for_m += (y_hat - y) * x;
        sum_for_c += (y_hat - y);
    }

    *grad_m = sum_for_m / n;
    *grad_c = sum_for_c / n;
}

/**
 * Performs gradient descent to find optimal m and c
 * Also records cost every 100 iterations into cost_history
 */
void gradient_descent(DataPoint dataset[], int n, float *m, float *c, float alpha, int iterations)
{
    cost_history_count = 0;
    // Record initial cost
    cost_history[cost_history_count++] = compute_cost(dataset, n, *m, *c);

    for(int loop = 0; loop < iterations; loop++)
    {
        float grad_m, grad_c;
        compute_gradient(dataset, n, *m, *c, &grad_m, &grad_c);

        // Update parameters using the gradients
        *m = *m - alpha * grad_m;
        *c = *c - alpha * grad_c;

        // Record cost every 100 iterations
        if ((loop + 1) % 100 == 0) {
            cost_history[cost_history_count++] = compute_cost(dataset, n, *m, *c);
        }
    }
}

// Displays a small portion of the loaded data for verification
void print_data_subset(DataPoint dataset[], int n, int count)
{
    int limit = count > n ? n : count;
    printf("\n--- Data Preview (First %d rows) ---\n", limit);
    for(int i = 0; i < limit; i++) {
        printf("Row %d: x = %.2f, y = %.2f\n", i+1, dataset[i].x, dataset[i].y);
    }
    printf("Total rows: %d\n", n);
    printf("------------------------------------\n");
}

/**
 * File I/O: Loads data from a text file using fscanf
 * filename: string containing the file path
 * n: updated via call-by-reference
 */
void load_data_from_file(char *filename, DataPoint dataset[], int *n) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open %s\n", filename);
        return;
    }

    int count = 0;
    while (count < SIZE && fscanf(file, "%f %f", &dataset[count].x, &dataset[count].y) == 2) {
        count++;
    }

    *n = count;
    fclose(file);
    printf("Success: Loaded %d points from %s.\n", *n, filename);
}

// File I/O: Saves model parameters (m, c) to a file
void save_model_to_file(char *filename, float m, float c) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error: Could not save model.\n");
        return;
    }
    fprintf(file, "%f %f\n", m, c);
    fclose(file);
    printf("Model saved to %s.\n", filename);
}

// File I/O: Loads model parameters from a file
void load_model_from_file(char *filename, float *m, float *c) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not load model.\n");
        return;
    }
    if (fscanf(file, "%f %f", m, c) == 2) {
        printf("Model parameters loaded from %s.\n", filename);
    }
    fclose(file);
}

// Draws the X and Y axes on the ASCII grid
void draw_axes(char arr[SIZE][SIZE], int r) {
    for(int j = 0; j < r; j++) arr[r-1][j] = '-'; // X-axis
    for(int i = 0; i < r; i++) arr[i][0] = '|';   // Y-axis
    arr[r-1][0] = '+'; // Origin
}

// Plots the line y = mx + c on the ASCII grid
void draw_line(char arr[SIZE][SIZE], int r, float m, float c) {
    for(int x = 0; x < r; x++) {
        float y_f = m * x + c;
        int y = (int)(y_f + 0.5); // Round to nearest integer

        int row = r - 1 - y;
        int col = x;

        if(row >= 0 && row < r && col >= 0 && col < r) {
            // Mark with 'X' or '#' if it overlaps with a data point
            if(arr[row][col] == '.') arr[row][col] = 'X';
            else if(arr[row][col] == '*') arr[row][col] = '#';
        }
    }
}

/**
 * Main plotting function
 * model_loaded: boolean to decide if we should draw the line
 */
void plot_graph(DataPoint dataset[], int n, float m, float c, int model_loaded) {
    int r = SIZE;
    char arr[SIZE][SIZE];

    // Initialize grid with dots (background)
    for(int i = 0; i < r; i++)
        for(int j = 0; j < r; j++)
            arr[i][j] = '.';

    // Plot dataset points as '*'
    for(int i = 0; i < n; i++) {
        int col = (int)(dataset[i].x + 0.5);
        int row = r - 1 - (int)(dataset[i].y + 0.5);

        if(row >= 0 && row < r && col >= 0 && col < r)
            arr[row][col] = '*';
    }

    // Draw axes and model line
    draw_axes(arr, r);
    if (model_loaded) draw_line(arr, r, m, c);

    // Print the final grid to terminal
    printf("\nASCII Plot (* = Data, X = Line, # = Both):\n");
    for(int i = 0; i < r; i++) {
        printf("| ");
        for(int j = 0; j < r; j++) printf("%c ", arr[i][j]);
        printf("\n");
    }

    // Bottom border
    printf("  ");
    for(int i = 0; i < 2*r; i++) printf("_");
    printf("\n");

    // Display model info below the plot
    if (model_loaded) {
        printf("\n[Model Information]\n");
        printf("Equation: y = %.4fx + %.4f\n", m, c);
        if (n > 0) {
            printf("Mean Squared Error: %.4f\n", compute_cost(dataset, n, m, c));
        } else {
            printf("Mean Squared Error: N/A (No data loaded)\n");
        }
        printf("------------------------------------\n");
    }
}


int main() {
    DataPoint dataset[SIZE];
    int n = 0; 
    
    // Model configuration
    float m = 0, c = 0;
    float alpha = 0.001; // Learning rate
    int iterations = 1000;
    
    int model_ready = 0; 
    char data_filename[50] = "data.txt";
    char model_filename[50] = "model.txt";

    int choice;
    MenuOption option;

    printf("--- Linear Regression Lab System ---\n");

    // Menu loop
    while(1) {
        printf("\n--- Main Menu ---\n");
        printf("1. Load Data\n");
        printf("2. Draw Plot\n");
        printf("3. Run Model (Train)\n");
        printf("4. Save Model\n");
        printf("5. Load Model\n");
        printf("6. Show Cost History\n");
        printf("7. Exit\n");
        printf("Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Please enter a valid number.\n");
            while(getchar() != '\n'); // Clear buffer
            continue;
        }

        option = (MenuOption)choice;

        switch(option) {
            case LOAD_DATA:
                load_data_from_file(data_filename, dataset, &n);
                if (n > 0) print_data_subset(dataset, n, 5);
                break;

            case DRAW_PLOT:
                if (n == 0 && !model_ready) printf("Nothing to plot yet.\n");
                else plot_graph(dataset, n, m, c, model_ready);
                break;

            case RUN_MODEL:
                if (n == 0) {
                    printf("Load data first!\n");
                    break;
                }
                m = 0; c = 0; // Reset
                gradient_descent(dataset, n, &m, &c, alpha, iterations);
                model_ready = 1;
                
                printf("\nTraining complete!\n");
                plot_graph(dataset, n, m, c, model_ready);
                break;

            case SAVE_MODEL:
                if (model_ready) save_model_to_file(model_filename, m, c);
                else printf("Train the model first!\n");
                break;

            case LOAD_MODEL:
                load_model_from_file(model_filename, &m, &c);
                model_ready = 1;
                plot_graph(dataset, n, m, c, model_ready);
                break;

            case SHOW_COST_HISTORY:
                if (cost_history_count == 0) {
                    printf("No cost history yet. Run the model first!\n");
                } else {
                    printf("\n--- Cost History (MSE) ---\n");
                    printf("Step %4d: %.6f\n", 0, cost_history[0]);
                    for (int i = 1; i < cost_history_count; i++) {
                        printf("Step %4d: %.6f\n", i * 100, cost_history[i]);
                    }
                    printf("--------------------------\n");
                }
                break;

            case EXIT:
                printf("Goodbye!\n");
                return 0;

            default:
                printf("Invalid option.\n");
        }
    }

    return 0;
}
