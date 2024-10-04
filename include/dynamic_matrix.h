#pragma once

#include "vector3d_structure.h"
#include <cstddef>

class DynamicMatrix {
private:
    Vector3D** matrix;

    size_t rows;
    size_t cols;

    void   allocateMemory();
    void deallocateMemory();

public:
    DynamicMatrix(size_t rows, size_t cols);
    ~DynamicMatrix();

    DynamicMatrix(const DynamicMatrix& other);
    DynamicMatrix& operator=(const DynamicMatrix& other);

    Vector3D& at(size_t row, size_t col);
    const Vector3D& at(size_t row, size_t col) const;

    void deleteRow(size_t row);
    void deleteColumn(size_t col);

    void insertRow(size_t rowIndex, const Vector3D* newRow);
    void insertColumn(size_t colIndex, const Vector3D* newColumn);
    void insertSubmatrix(const DynamicMatrix& submatrix, size_t startRow, size_t startCol);

    DynamicMatrix operator+(const DynamicMatrix& other) const;
    DynamicMatrix operator-(const DynamicMatrix& other) const;
    DynamicMatrix operator*(const DynamicMatrix& other) const;
    DynamicMatrix operator*(double scalar) const;

    size_t getRows() { return rows; }
    size_t getCols() { return cols; }

    void print() const;
};
