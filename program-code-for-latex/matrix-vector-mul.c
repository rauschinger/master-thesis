// uses GrB_mxv with GrB_PLUS_TIMES_SEMIRING_FP64 semiring to compute the result

// to compile and run use:
// gcc -o matrix-vector-mul matrix-vector-mul.c -I/path/to/graphblas/include -L/path/to/graphblas/lib -lgraphblas
// ./matrix-vector-mul

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GraphBLAS.h>

// Custom function to print a GraphBLAS matrix
void printMatrix(GrB_Matrix matrix) {
    GrB_Index nrows, ncols;
    GrB_Matrix_nrows(&nrows, matrix);
    GrB_Matrix_ncols(&ncols, matrix);

    for (GrB_Index i = 0; i < nrows; i++) {
        for (GrB_Index j = 0; j < ncols; j++) {
            double value;
            GrB_Matrix_extractElement_FP64(&value, matrix, i, j);
            printf("%f ", value);
        }
        printf("\n");
    }
}

// Custom function to fill a matrix with random values
void fillMatrixRandom(GrB_Matrix matrix, double range) {
    GrB_Index nrows, ncols;
    GrB_Matrix_nrows(&nrows, matrix);
    GrB_Matrix_ncols(&ncols, matrix);

    for (GrB_Index i = 0; i < nrows; i++) {
        for (GrB_Index j = 0; j < ncols; j++) {
            GrB_Matrix_setElement_FP64(matrix, ((double)rand() / RAND_MAX) * range, i, j);
        }
    }
}

// Custom function to print a GraphBLAS vector
void printVector(GrB_Vector vector, const char* name) {
    GrB_Index size;
    GrB_Vector_size(&size, vector);

    printf("%s:\n[", name);

    for (GrB_Index i = 0; i < size; i++) {
        double value;
        GrB_Vector_extractElement_FP64(&value, vector, i);
        printf("%f", value);

        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

// Function to fill a vector with random values
void fillVectorRandom(GrB_Vector vector, double range) {
    GrB_Index size;
    GrB_Vector_size(&size, vector);

    for (GrB_Index i = 0; i < size; i++) {
        GrB_Vector_setElement_FP64(vector, ((double)rand() / RAND_MAX) * range, i);
    }
}

int main() {
    // Initialize GraphBLAS
    GrB_init(GrB_BLOCKING);

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Create matrix A and vectors x,y
    GrB_Matrix A;
    GrB_Vector x;
    GrB_Vector y;
    GrB_Matrix_new(&A, GrB_FP64, 3, 3);
    GrB_Vector_new(&x, GrB_FP64, 3);
    GrB_Vector_new(&y, GrB_FP64, 3);

    // Fill matrix A and vector x
    fillMatrixRandom(A, 10.0);
    fillVectorRandom(x, 10.0);

    // Print matrix A and vector x
    printf("\nMatrix A:\n");
    printMatrix(A);
    printf("\n");
    printVector(x, "Vector x");
    printf("\n");

    // Perform matrix-vector multiplication: y = A * x
    GrB_mxv(y, GrB_NULL, GrB_NULL, GrB_PLUS_TIMES_SEMIRING_FP64, A, x, GrB_NULL);

    // Print the result vector y
    printVector(y, "Result Vector y");

    // Free matrices and finish
    GrB_Matrix_free(&A);
    GrB_Vector_free(&x);
    GrB_Vector_free(&y);
    GrB_finalize();

    return 0;
}

