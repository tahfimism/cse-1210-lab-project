#include <stdio.h>

#define SIZE 30

// Step 1: Define a data type for a single data point
typedef struct {
    float x;
    float y;
} DataPoint;


// Computes mean squared error cost
float compute_cost(DataPoint dataset[], int n, float m, float c)
{
    float sum = 0;
    for(int i = 0; i < n; i++){
        float x = dataset[i].x;
        float y = dataset[i].y;

        float y_hat = m*x + c; // predicted y

        float cost_for_each_dt = y_hat - y;

        sum += cost_for_each_dt * cost_for_each_dt;
    }

    float cost = sum/(2*n);
    return cost;

}


// Computes gradients of m and c for one step
void compute_gradient(DataPoint dataset[], int n, float m, float c, float *grad_m, float *grad_c)
{
    float sum_for_m = 0;
    float sum_for_c = 0;
    for(int i = 0; i < n; i++){

        float x = dataset[i].x;
        float y = dataset[i].y;

        float y_hat = m*x + c;

        sum_for_m += (y_hat - y)*x;
        sum_for_c += (y_hat - y);
    }

    *grad_m = sum_for_m/n;
    *grad_c = sum_for_c/n;
}



// Performs gradient descent to optimize m and c
void gradient_descent(DataPoint dataset[], int n, float *m, float *c, float alpha, int iterations)
{

    for(int loop=0; loop < iterations; loop++)
    {

        float grad_m, grad_c;

        // compute current grad
        compute_gradient(dataset, n, *m, *c, &grad_m, &grad_c);

        // Update parameters
        *m = *m - alpha * grad_m;
        *c = *c - alpha * grad_c;
    }
}

void print_data(DataPoint dataset[])
{
    printf("Dataset:\n");
    for(int i = 0; i < 10; i++) {
        printf("Point %d: x = %.2f, y = %.2f\n", i+1, dataset[i].x, dataset[i].y);
    }
}



void draw_axes(char arr[SIZE][SIZE], int r);
void draw_line(char arr[SIZE][SIZE], int r, float m, float c);
void plot_graph();

int main() {
    // Step 2: Create an array of 10 data points
    DataPoint dataset[10] = {
        {1.0, 2.0},
        {2.0, 2.5},
        {3.0, 3.5},
        {4.0, 4.0},
        {5.0, 4.5},
        {6.0, 5.0},
        {7.0, 5.5},
        {8.0, 6.0},
        {9.0, 6.5},
        {10.0, 7.0}
    };

    // Print the dataset
    print_data(dataset);

    // Gradient descent parameters
    float m = 0, c = 0;
    float alpha = 0.001;
    int iterations = 1000;
    int n = 10;

    // Train regression line
    gradient_descent(dataset, n, &m, &c, alpha, iterations);



    float input_x = 4.7;
    float prediction = m*input_x + c;


    // -------------------------
    // PLOTTING
    // -------------------------
    int r = SIZE;
    char arr[SIZE][SIZE];

    // Initialize grid with dots
    for(int i = 0; i < r; i++)
        for(int j = 0; j < r; j++)
            arr[i][j] = '.';

    // Draw axes
    // draw_axes(arr, r);

    // Draw dataset points
    for(int i = 0; i < n; i++) {
        int col = (int)(dataset[i].x + 0.5);
        int row = r - 1 - (int)(dataset[i].y + 0.5); // 1st quadrant

        if(row >= 0 && row < r && col >= 0 && col < r)
            arr[row][col] = '*';
    }

    // Draw regression line
    draw_line(arr, r, m, c);

    // Print the grid
    printf("\nGraph:\n");
    for(int i = 0; i < r; i++) {
        printf("| ");
        for(int j = 0; j < r; j++) {
            printf("%c ", arr[i][j]);
        }
        printf("\n");
    }

    // Print bottom border
    printf("  ");
    for(int i = 0; i < 2*r; i++)
        printf("_");
    printf("\n");



    // print vales
    printf("\nLearned line: y = %.4fx + %.4f\n", m, c);
    printf("Prediction for x = %.2f: %.4f\n", input_x, prediction);


    return 0;
}





// Draw axes (1st quadrant)
void draw_axes(char arr[SIZE][SIZE], int r) {
    // X-axis (bottom row)
    for(int j = 0; j < r; j++)
        arr[r-1][j] = '-';

    // Y-axis (left column)
    for(int i = 0; i < r; i++)
        arr[i][0] = '|';

    arr[r-1][0] = '+'; // origin
}




// Draw single line y = mx + c
void draw_line(char arr[SIZE][SIZE], int r, float m, float c) {
    for(int x = 0; x < r; x++) {
        float y_f = m * x + c;
        int y = (int)(y_f + 0.5);  // rounding

        int row = r - 1 - y;
        int col = x;

        if(row >= 0 && row < r && col >= 0 && col < r) {
            if(arr[row][col] == '.')
                arr[row][col] = 'X';
            if(arr[row][col] == '*')
                arr[row][col] = '#';
        }
    }
}

// The whole graph plotting system
void plot_graph() {
    int r = SIZE;
    char arr[SIZE][SIZE];

    // Initialize grid
    for(int i = 0; i < r; i++)
        for(int j = 0; j < r; j++)
            arr[i][j] = '.';

    // Draw axes
    draw_axes(arr, r);

    // Draw lines (example lines; you can call draw_line() later with regression output)
    draw_line(arr, r, 1, 0);    // y = x
    draw_line(arr, r, 0.5, 2);  // y = 0.5x + 2
    draw_line(arr, r, 2, 0);    // y = 2x

    // Print the grid
    printf("\n");
    for(int i = 0; i < r; i++){
        printf("| ");
        for(int j = 0; j < r; j++){
            printf("%c ", arr[i][j]);
        }
        printf("\n");
    }

    // Print bottom border
    printf("  ");
    for(int i = 0; i < 2*r; i++){
        printf("_");
    }
    printf("\n");
}
