#pragma once

#include "vector3d_structure.h"
#include <cstddef>
#include <fstream>
#include <stdexcept>

class DynamicMatrix {
private:
    Vector3D** matrix;
    size_t rows;
    size_t cols;

    void allocateMemory();
    void deallocateMemory();

public:
    DynamicMatrix(size_t rows = 0, size_t cols = 0);
    ~DynamicMatrix();

    DynamicMatrix(const DynamicMatrix& other);
    DynamicMatrix& operator=(const DynamicMatrix& other);

    DynamicMatrix(DynamicMatrix&& other) noexcept;
    DynamicMatrix& operator=(DynamicMatrix&& other) noexcept;

    Vector3D& at(size_t row, size_t col);
    const Vector3D& at(size_t row, size_t col) const;

    double totalMagnitude() const;

    void deleteRow(size_t row);
    void deleteColumn(size_t col);

    void insertRow(size_t rowIndex, const Vector3D* newRow);
    void insertColumn(size_t colIndex, const Vector3D* newColumn);
    void insertSubmatrix(const DynamicMatrix& submatrix, size_t startRow, size_t startCol);

    DynamicMatrix operator+(const DynamicMatrix& other) const;
    DynamicMatrix operator-(const DynamicMatrix& other) const;
    DynamicMatrix operator*(const DynamicMatrix& other) const;
    DynamicMatrix operator*(double scalar) const;

    void deleteItem(size_t rowIndex, size_t colIndex);
    void addItem(size_t rowIndex, size_t colIndex, const Vector3D& vec);
    void addVectorAt(size_t rowIndex, size_t colIndex, const Vector3D& vec);

    bool operator==(const DynamicMatrix& other) const;
    bool operator!=(const DynamicMatrix& other) const;
    bool operator<(const DynamicMatrix& other) const;
    bool operator>(const DynamicMatrix& other) const;
    bool operator<=(const DynamicMatrix& other) const;
    bool operator>=(const DynamicMatrix& other) const;

    friend std::ostream& operator<<(std::ostream& os, const DynamicMatrix& mat);
    friend std::istream& operator>>(std::istream& is, DynamicMatrix& mat);

    // New file I/O operators
    void saveToFile(const std::string& filename) const;
    static DynamicMatrix loadFromFile(const std::string& filename);

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

    void print() const;
};
