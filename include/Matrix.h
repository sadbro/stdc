#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <assert.h>

typedef struct {

    size_t rows;
    size_t cols;
    size_t stride;
    int* start;

} Matrix;

Matrix MATRIX(size_t rows, size_t cols, float _default){

    Matrix m;
    m.rows = rows;
    m.cols = cols;
    m.start = malloc(sizeof(int) * rows * cols);

    for(size_t i=0; i<rows*cols; i++){ m.start[i] = _default; }

    assert(m.start != NULL);
    return m;
}

#define MAT_AT(m, i, j) (m).start[(i)*(m).cols + (j)]

Matrix matrix_identity(size_t order){

    Matrix m = MATRIX(order, order, 0);
    for(size_t i=0; i<m.rows; i++){

        for(size_t j=0; j<m.cols; j++){

            if(i == j){ MAT_AT(m, i, j) += 1; }
        }
    }

    return m;
}

#define fill(matrix, value) randomise(matrix, value, value)

Matrix row_loc(Matrix m, size_t row_start, size_t row_end){

    assert( row_end <= m.rows );
    assert( row_end > row_start );

    size_t nrows = row_end - row_start;

    return (Matrix){

        .rows = nrows,
        .cols = m.cols,
        .start = &MAT_AT(m, row_start, 0)
    };
}

Matrix col_loc(Matrix m, size_t col_start, size_t col_end){

    assert( col_end <= m.cols );
    assert( col_end > col_start );

    size_t ncols = col_end - col_start;

    Matrix result = MATRIX(m.rows, ncols, 0.0f);

    for (size_t i=0; i<result.rows; i++){

        for (size_t j=0; j<result.cols; j++){

            MAT_AT(result, i, j) = MAT_AT(m, i, col_start + j);
        }
    }

    return result;
}

void matrix_sum(Matrix dst, Matrix src){

    assert((dst.rows == src.rows) && (dst.cols == src.cols));

    for(size_t i=0; i<dst.rows; i++){

        for(size_t j=0; j<dst.cols; j++){

            MAT_AT(dst, i, j) += MAT_AT(src, i, j);
        }
    }
}

void matrix_dot(Matrix dst, Matrix a, Matrix b){

    assert(a.cols == b.rows);
    assert((dst.rows == a.rows) && (dst.cols == b.cols));
    size_t n = a.cols;

    for(size_t i=0; i<dst.rows; i++){

        for(size_t j=0; j<dst.cols; j++){

            MAT_AT(dst, i, j) = 0.0f;
            for(size_t k=0; k<n; k++){

                MAT_AT(dst, i, j) += MAT_AT(a, i, k) * MAT_AT(b, k, j);
            }
        }
    }
}

void matrix_copy(Matrix dst, Matrix src){

    assert( (dst.cols == src.cols) && (dst.rows == src.rows) );

    for (size_t i=0; i<dst.rows; i++){

        for (size_t j=0; j<dst.cols; j++){

            MAT_AT(dst, i, j) = MAT_AT(src, i, j);
        }
    }
}

int _(float a){ return (int)a; }

void matrix_print_replace(Matrix m, const char* delimiter, int (*replacer)(float)){

    for (size_t i = 0; i < m.rows; ++i) {

        for (size_t j = 0; j < m.cols -1; ++j) {

            printf("%d%s", replacer(MAT_AT(m, i, j)), delimiter);
        }
        printf("%d\n", replacer(MAT_AT(m, i, m.cols -1)));
    }
}

void matrix_print(Matrix m, const char* delimiter){ matrix_print_replace(m, delimiter, _); }

void matrix_fprint_replace(FILE* stream, Matrix m, const char* delimiter, int (*replacer)(float)){

    for (size_t i = 0; i < m.rows; ++i) {

        for (size_t j = 0; j < m.cols -1; ++j) {

            fprintf(stream, "%d%s", replacer(MAT_AT(m, i, j)), delimiter);
        }
        fprintf(stream, "%d\n", replacer(MAT_AT(m, i, m.cols -1)));
    }
}

void matrix_fprint(FILE* stream, Matrix m, const char* delimiter){ matrix_fprint_replace(stream, m, delimiter, _); }

#endif // MATRIX_H