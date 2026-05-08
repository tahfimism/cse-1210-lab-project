# Linear Regression CLI System

## Project Overview

The **Linear Regression CLI System** is an interactive, terminal-based application written entirely in C. Developed as an open-ended lab experiment for a programming course (CSE 1210), the project demonstrates core computer science principles by implementing a fundamental machine learning algorithm from scratch.

At its core, this project exists to bridge the gap between low-level system programming and high-level mathematical optimization. It allows users to load 2D datasets, train a linear regression model using gradient descent, track error reduction over time, and visualize the results using an embedded ASCII graphing engine.

The system solves the problem of understanding how models learn without relying on abstract, black-box libraries like scikit-learn or TensorFlow. It is tailored for students, educators, and developers who want a transparent view into memory management, algorithm implementation, and mathematical modeling in pure C.

## System Capabilities / Features

- **Interactive CLI Console:** A continuous menu loop that robustly handles user commands and invalid inputs, managing the complete state of the application.
- **Custom Gradient Descent Engine:** Calculates partial derivatives of the Mean Squared Error (MSE) cost function to iteratively optimize the slope ($m$) and intercept ($c$) of the linear equation $y = mx + c$.
- **ASCII Coordinate Plotter:** Renders data points, axes, and the fitted regression line directly in the terminal by mapping continuous floating-point values into a discrete 2D grid space.
- **Cost History Tracking:** Records the MSE every 100 iterations during the training phase, allowing users to verify that the gradient descent is converging properly.
- **Persistent Storage:** Reads training datasets from text files (`data.txt`) and allows exporting/importing of trained model weights (`model.txt`) across different application sessions.

## Technical Architecture

The architecture is contained within a single file (`main.c`) utilizing standard C paradigms to achieve a modular and state-driven design.

- **Frontend/UI:** Text-based menu loop powered by `scanf`, `switch-case` statements, and standard output formatting.
- **State Management:** Handled via local variables in `main()` mapped to system states (e.g., `model_ready`, `dataset` arrays). Cost history is stored globally (`cost_history`, `cost_history_count`) to track the optimization lifecycle.
- **Data Layer:** Uses custom `struct DataPoint` to encapsulate Cartesian coordinates.
- **Processing Layer:** Heavily utilizes function modularity and pointer-based memory manipulation (Call by Reference) to handle multi-variable updates during gradient calculation.

### File Structure
```text
.
├── main.c           # Core application logic, math, and rendering engine
├── data.txt         # Input dataset (X, Y space-separated floats)
├── model.txt        # Output artifact (stores trained m and c parameters)
├── report.md        # Technical analysis and execution trace reports
└── requirements.md  # Original academic constraints and project requirements
```

## Internal Logic & Implementation

### Optimization Pipeline
The optimization relies on standard Batch Gradient Descent. The `gradient_descent` function loops for a fixed number of iterations. Inside the loop, it calls `compute_gradient`, passing pointers `&grad_m` and `&grad_c`. The gradients are calculated by summing the errors $( \hat{y} - y ) * x$ for the slope and $( \hat{y} - y )$ for the intercept across the entire dataset. The weights are then shifted in the opposite direction of the gradient, scaled by the learning rate (`alpha`).

### ASCII Rendering Engine
The `plot_graph` function creates a `char arr[SIZE][SIZE]` buffer. Because terminal outputs print top-to-bottom but Cartesian graphs go bottom-to-top, the system dynamically inverts the Y-axis using `row = SIZE - 1 - (int)(y + 0.5)`.
- Data points are injected into the array as `*`.
- The regression line is calculated across the X-axis domain, rounded to integers, and plotted as `X`.
- Intersections are cleverly marked with `#`.

### Input Handling
The system employs robust file parsing using `fscanf`, verifying the return value to ensure exactly two floats are read per line. It avoids buffer overflows by capping reads against a hardcoded `SIZE` macro constraint.

## Tech Stack

- **Language:** C (C99+ Standard)
- **Standard Libraries:**
  - `<stdio.h>` for file I/O, console streams.
  - `<stdlib.h>` for basic type operations.
- **Compiler:** GCC / Clang / MSVC agnostic.
- **Why C?:** Implementing a mathematical modeling system in C enforces an understanding of pointers, memory layout, and manual data structures, fulfilling the requirement of the underlying lab assignment while proving that higher-level abstraction isn't strictly necessary for ML concepts.

## User Experience / Product Flow

1. **Initialization:** The user launches the executable and is greeted with a 7-option main menu.
2. **Data Loading:** The user selects `1` to parse `data.txt`. The system validates the file and prints a preview of the first 5 records.
3. **Training:** The user selects `3`. The gradient descent loop runs, the global state marks `model_ready = 1`, and the system automatically invokes the plotting engine to show the fitted line overlaid on the data.
4. **Analysis:** The user selects `6` to inspect the Cost History table, verifying that the MSE dropped from its initial value to its final optimized state.
5. **Persistence:** The user selects `4` to save the weights, creating or overwriting `model.txt`.
6. **Exit:** The user selects `7` to terminate the process safely.

## Engineering Decisions

- **Static Memory Allocation over Dynamic:** To prioritize simplicity and avoid memory leaks, `dataset` arrays and plotting grids use fixed static sizes defined by `#define SIZE 30`. This limits the dataset bounds but removes the complexity of managing heap memory (`malloc`/`free`).
- **Call by Reference (Pointers):** C lacks the ability to return multiple values from functions. Pointers are heavily utilized in `compute_gradient` and `load_data_from_file` to mutate caller variables efficiently without wrapping outputs in ad-hoc structs.
- **Robust Input Buffer Clearing:** Menu inputs are prone to infinite loops if a string is provided instead of an integer. The loop `while(getchar() != '\n');` is intentionally included to flush `stdin` whenever `scanf` fails.
- **Separation of Concerns:** The code is meticulously separated. Math functions do not handle I/O, and file parsing functions do not contain business logic.

## Current Status

- **Fully Operational:** The system successfully parses data, trains the model, tracks cost, and visualizes the results.
- **Hardcoded Hyperparameters:** The learning rate ($\alpha = 0.001$) and iterations ($1000$) are statically compiled.
- **Fixed Scaling:** The ASCII graph expects input data bounded roughly within $0 \leq X, Y < 30$. Values exceeding these bounds are safely truncated via out-of-bounds checks, but they do not scale the grid dynamically.
- **Paths:** Filenames (`data.txt`, `model.txt`) are hardcoded into the `main()` function scope.

## Future Improvements

- **Dynamic Memory and Scaling:** Implement `malloc` to handle datasets of any length. Introduce a scaling function to map arbitrary floating-point domains (e.g., coordinates in the thousands) down to the 30x30 console grid representation.
- **Configurable Hyperparameters:** Expose the learning rate, iteration count, and input file paths as CLI arguments or menu prompts.
- **Prediction Tool:** Add an 8th menu option allowing users to input a custom $X$ value and have the system predict $Y$ based on the trained model.
- **Multivariate Regression:** Expand the `DataPoint` struct into an array of features to support multivariable regression.

## Conclusion

The Linear Regression CLI System is a highly educational and technically sound implementation of machine learning fundamentals written in raw C. By stripping away heavy dependencies, the project serves as an excellent demonstration of memory management, file I/O, algorithm optimization, and text-based graphics plotting. It perfectly meets the scope of a deep exploratory lab experiment while remaining robust and extensible.