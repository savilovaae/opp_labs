#include "parallel_for_program.h"

void p_fill_initial_approximation(double *approximation, size_t N) {
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        approximation[i] = 0;
    }
}

void p_fill_matrix_vector(double **matrix, double *vector, size_t N) {
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = (i == j) ? 2 : 1;
        }
        vector[i] = (double) N + 1;
    }
}

bool p_is_solved(const double **matrix, const double *vector, double *curr_approximation, size_t N) {
    double numerator_sqrt = p_get_vector_sqrt(vector, N);
    double *denominator = malloc(sizeof(denominator) * N);
    p_multiplication_matrix_vector(matrix, curr_approximation, denominator, N);
    p_subtracting_vectors(denominator, vector, N);
    double denominator_sqrt = p_get_vector_sqrt(denominator, N);
    free(denominator);
    return denominator_sqrt / numerator_sqrt < Epsilon;
}

void p_multiplication_matrix_vector(const double **matrix, const double *vector, double *res, size_t N) {
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        res[i] = 0;
    }
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            res[i] += matrix[i][j] * vector[j];
        }
    }
}

void p_subtracting_vectors(double *curr, const double *vector, size_t N) {
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        curr[i] -= vector[i];
    }
}

void p_multiplication_tau_vector(const double *vector, double *result, size_t N) {
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        result[i] = vector[i] * Tau;
    }
}

double p_get_vector_sqrt(const double *vector, size_t N) {
    double ans = 0;
#pragma omp parallel for reduction(+: ans)
    for (int i = 0; i < N; i++) {
        ans += vector[i] * vector[i];
    }
    return (double) pow(ans, 0.5);
}


void p_preparation_perfomance_free(size_t N) {
    double **matrix = malloc(sizeof(*matrix) * N);
    for (int i = 0; i < N; i++) {
        matrix[i] = malloc(sizeof(matrix[i]) * N);
    }
    double *vector = malloc(sizeof(vector) * N);
    p_fill_matrix_vector(matrix, vector, N);
    double *initial_approximation = malloc(sizeof(vector) * N);
    p_fill_initial_approximation(initial_approximation, N);
    p_solve_equations((const double **) matrix, vector, initial_approximation, N);
    //p_print_result(initial_approximation, N);
    for (int i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(initial_approximation);
    free(matrix);
    free(vector);
}

void p_solve_equations(const double **matrix, const double *vector, double *initial_approximation, size_t N) {
    do {
        double *tmp_vect = malloc(sizeof(tmp_vect) * N);
        p_multiplication_matrix_vector(matrix, initial_approximation, tmp_vect, N);
        p_subtracting_vectors(tmp_vect, vector, N);
        double *tmp_curr = malloc(sizeof(tmp_curr) * N);
        p_multiplication_tau_vector(tmp_vect, tmp_curr, N);
        p_subtracting_vectors(initial_approximation, tmp_curr, N);
        free(tmp_curr);
        free(tmp_vect);
    } while (!p_is_solved(matrix, vector, initial_approximation, N));
}


void p_print_result(double *res, size_t N) {
    for (int i = 0; i < N; i++) {
        printf("res[%d] = %f\n", i, res[i]);
    }
}