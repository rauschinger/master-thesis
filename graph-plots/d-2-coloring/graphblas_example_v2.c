// to compile and run use:
// gcc -o graphblas_example_v2 graphblas_example_v2.c -I/path/to/graphblas/include -L/path/to/graphblas/lib -lgraphblas
// ./graphblas_example_v2


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <GraphBLAS.h>

#define MATRIX_SIZE 12

// Function to create and initialize a GraphBLAS matrix
GrB_Matrix createAdjacencyMatrix() {
    GrB_Matrix adjacency_matrix;
    GrB_Matrix_new(&adjacency_matrix, GrB_INT32, MATRIX_SIZE, MATRIX_SIZE);

    // Define the adjacency matrix data
    int32_t data[MATRIX_SIZE][MATRIX_SIZE] = {
      {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1},
      {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1},
      {0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0},
      {0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0},
      {1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0},
      {0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0},
      {1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
      {1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
      {0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    // Populate the adjacency matrix
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        for (int j = 0; j < MATRIX_SIZE; ++j) {
            GrB_Matrix_setElement_INT32(adjacency_matrix, data[i][j], i, j);
        }
    }

    return adjacency_matrix;
}

// Function to set diagonal entries of a GraphBLAS matrix to zero
void setDiagonalToZero(GrB_Matrix matrix) {
    GrB_Index nrows, ncols;
    GrB_Matrix_nrows(&nrows, matrix);
    GrB_Matrix_ncols(&ncols, matrix);

    for (GrB_Index i = 0; i < nrows; ++i) {
        GrB_Matrix_setElement_INT32(matrix, 0, i, i);
    }
}

// Custom function to print a GraphBLAS matrix
void printMatrix(GrB_Matrix matrix) {
    GrB_Index nrows, ncols;
    GrB_Matrix_nrows(&nrows, matrix);
    GrB_Matrix_ncols(&ncols, matrix);

    for (GrB_Index i = 0; i < nrows; ++i) {
        for (GrB_Index j = 0; j < ncols; ++j) {
            int32_t value;
            GrB_Matrix_extractElement_INT32(&value, matrix, i, j);
            printf("%d ", value);
        }
        printf("\n");
    }
}

// Function to manipulate the adjacency matrix and return the manipulated matrix
GrB_Matrix matrixManipulation(GrB_Matrix adjacency_matrix) {
    // only for d2 coloring approach
    GrB_Matrix adjacency_matrix_square;
    GrB_Matrix_new(&adjacency_matrix_square, GrB_INT32, MATRIX_SIZE, MATRIX_SIZE); 

    // Perform matrix multiplication: adjacency_matrix_square = adjacency_matrix * adjacency_matrix
    GrB_mxm(adjacency_matrix_square, GrB_NULL, GrB_NULL, GxB_PLUS_TIMES_INT32, adjacency_matrix, adjacency_matrix, GrB_NULL);

    // Set diagonal entries to zero
    setDiagonalToZero(adjacency_matrix_square);

    // Apply the condition: set entries greater than 1 to 1
    GrB_apply(adjacency_matrix_square, GrB_NULL, GrB_NULL, GrB_MIN_INT32, 1, adjacency_matrix_square, GrB_NULL);

    // Return the manipulated matrix
    return adjacency_matrix_square;
}

// Custom function to generate distinct random integer values
int generateDistinctRandom() {
    static bool initialized = false;
    if (!initialized) {
        srand(time(NULL));
        initialized = true;
    }
    
    static bool usedValues[100] = {false};  // Assuming a range of values from 1 to 100

    int random_value;
    do {
        random_value = rand() % 100 + 1;  // Adjust the range as needed
    } while (usedValues[random_value]);

    usedValues[random_value] = true;

    return random_value;
}


// Custom function to print a GraphBLAS vector
void printVector(GrB_Vector vector) {
    GrB_Index size;
    GrB_Vector_size(&size, vector);

    printf("\n");

    for (GrB_Index i = 0; i < size; i++) {
        int32_t value;
        GrB_Vector_extractElement_INT32(&value, vector, i);
        printf("%d ", value);
    }
}


int main() {
    // Initialize GraphBLAS
    GrB_init(GrB_BLOCKING);

    // Create and print the adjacency matrix
    GrB_Matrix adjacency_matrix = createAdjacencyMatrix();
    printMatrix(adjacency_matrix);

    // Call the matrix manipulation function and get the manipulated matrix
    GrB_Matrix manipulated_matrix = matrixManipulation(adjacency_matrix);

    // Create empty vectors, scalar and auxiliary variable
    GrB_Vector C;
    GrB_Vector weight;
    GrB_Vector frontier;
    GrB_Vector max;
    GrB_Scalar succ;
    GrB_Vector_new(&C, GrB_INT32, MATRIX_SIZE);
    GrB_Vector_new(&weight, GrB_INT32, MATRIX_SIZE);
    GrB_Vector_new(&frontier, GrB_INT32, MATRIX_SIZE);
    GrB_Vector_new(&max, GrB_INT32, MATRIX_SIZE);
    GrB_Scalar_new(&succ, GrB_INT32);
    int32_t Intsucc;

    // see GraphBLAS API p. 167 for right set of input variables
    // Initialize colors to 0
    GrB_assign(C, GrB_NULL, GrB_NULL, 0, GrB_ALL, MATRIX_SIZE, GrB_NULL);

    // Assign distinct random integer as weight to each vertex
    for (GrB_Index i = 0; i < MATRIX_SIZE; i++) {
        int random_value = generateDistinctRandom();
        GrB_Vector_setElement_INT32(weight, random_value, i);
    }

    for( GrB_Index color = 1; color < MATRIX_SIZE; color++) {

      // find maximum of neighbors
      GrB_vxm(max, GrB_NULL, GrB_NULL, GrB_MAX_TIMES_SEMIRING_INT32, weight, manipulated_matrix, GrB_NULL);

      GrB_eWiseAdd(frontier, GrB_NULL, GrB_NULL, GrB_GT_INT32, weight, max, GrB_NULL);

      // Find all largest nodes that are uncolored
      GrB_reduce(succ, GrB_NULL, GrB_PLUS_INT32, frontier, GrB_NULL);

      // gets integer value from GrB_Scalar Type succ to be able to check the following break condition
      GrB_Scalar_extractElement_INT32(&Intsucc, succ);

      // stop when frontier is empty
      if (Intsucc == 0) {
         break;
      }

      // Assign new color
      GrB_assign(C, frontier, GrB_NULL, color, GrB_ALL, MATRIX_SIZE, GrB_NULL);

      // Get rid of colored nodes in candidate list
      GrB_assign(weight, frontier, GrB_NULL, 0, GrB_ALL, MATRIX_SIZE, GrB_NULL);
    }
    
    printVector(C);
    printf("\n");



    // Free resources
    GrB_Matrix_free(&adjacency_matrix);
    GrB_Matrix_free(&manipulated_matrix);
    GrB_Vector_free(&C);
    GrB_Vector_free(&weight);
    GrB_Vector_free(&frontier);
    GrB_Vector_free(&max);
    GrB_Scalar_free(&succ);
    GrB_finalize();

    return 0;
}

