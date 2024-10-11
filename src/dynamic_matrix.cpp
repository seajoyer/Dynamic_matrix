#include "dynamic_matrix.h"
#include "vector3d_structure.h"
#include <cstddef>
#include <stdexcept>
#include <cstring>

void DynamicMatrix::allocateMemory() {
    matrix = new Vector3D*[rows];
    for (size_t i = 0; i < rows; ++i)
        matrix[i] = new Vector3D[cols];
}

void DynamicMatrix::deallocateMemory() {
    for (size_t i = 0; i < rows; ++i)
        delete[] matrix[i];

    delete[] matrix;
}

DynamicMatrix::DynamicMatrix(size_t rows, size_t cols) : rows(rows), cols(cols) {
    allocateMemory();
}

DynamicMatrix::~DynamicMatrix() {
    deallocateMemory();
}

DynamicMatrix::DynamicMatrix(const DynamicMatrix& other) : rows(other.rows), cols(other.cols) {
    allocateMemory();
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            matrix[i][j] = other.matrix[i][j];
}

DynamicMatrix& DynamicMatrix::operator=(const DynamicMatrix& other) {
    if (this != &other) {
        deallocateMemory();
        rows = other.rows;
        cols = other.cols;
        allocateMemory();
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                matrix[i][j] = other.matrix[i][j];
    }
    return *this;
}

DynamicMatrix::DynamicMatrix(DynamicMatrix&& other) noexcept
    : matrix(other.matrix), rows(other.rows), cols(other.cols) {
    other.matrix = nullptr;
    other.rows = 0;
    other.cols = 0;
}

DynamicMatrix& DynamicMatrix::operator=(DynamicMatrix&& other) noexcept {
    if (this != &other) {
        deallocateMemory();
        matrix = other.matrix;
        rows = other.rows;
        cols = other.cols;
        other.matrix = nullptr;
        other.rows = 0;
        other.cols = 0;
    }
    return *this;
}

Vector3D& DynamicMatrix::at(size_t row, size_t col) {
    if (row >= rows || col >= cols)
        throw std::out_of_range("Matrix index out of range");
    return matrix[row][col];
}

const Vector3D& DynamicMatrix::at(size_t row, size_t col) const {
    if (row >= rows || col >= cols)
        throw std::out_of_range("Matrix index out of range");
    return matrix[row][col];
}

void DynamicMatrix::deleteRow(size_t row) {
    if (row >= rows)
        throw std::out_of_range("Row index out of range");

    Vector3D** newMatrix = new Vector3D*[rows - 1];

    for (size_t i = 0, newI = 0; i < rows; ++i) {
        if (i != row)
            newMatrix[newI++] = matrix[i];
        else
            delete[] matrix[i];
    }
    delete[] matrix;
    matrix = newMatrix;
    --rows;
}

void DynamicMatrix::deleteColumn(size_t col) {
    if (col >= cols)
        throw std::out_of_range("Column index out of range");

    for (size_t i = 0; i < rows; ++i) {
        Vector3D* newRow = new Vector3D[cols - 1];
        for (size_t j = 0, newJ = 0; j < cols; ++j)
            if (j != col)
                newRow[newJ++] = matrix[i][j];

        delete[] matrix[i];
        matrix[i] = newRow;
    }
    --cols;
}

void DynamicMatrix::insertRow(size_t rowIndex, const Vector3D* newRow) {
    if (rowIndex > rows)
        throw std::out_of_range("Row index out of range");

    Vector3D** newMatrix = new Vector3D*[rows + 1];

    for (size_t i = 0; i < rowIndex; ++i)
        newMatrix[i] = matrix[i];

    newMatrix[rowIndex] = new Vector3D[cols];
    for (size_t j = 0; j < cols; ++j)
        newMatrix[rowIndex][j] = newRow[j];

    for (size_t i = rowIndex; i < rows; ++i)
        newMatrix[i + 1] = matrix[i];

    delete[] matrix;
    matrix = newMatrix;
    ++rows;
}

void DynamicMatrix::insertColumn(size_t colIndex, const Vector3D* newColumn) {
    if (colIndex > cols)
        throw std::out_of_range("Column index out of range");

    for (size_t i = 0; i < rows; ++i) {

        Vector3D* newRow = new Vector3D[cols + 1];

        for (size_t j = 0; j < colIndex; ++j)
            newRow[j] = matrix[i][j];

        newRow[colIndex] = newColumn[i];
        for (size_t j = colIndex; j < cols; ++j)
            newRow[j + 1] = matrix[i][j];

        delete[] matrix[i];
        matrix[i] = newRow;
    }
    ++cols;
}

void DynamicMatrix::insertSubmatrix(const DynamicMatrix& submatrix, size_t startRow, size_t startCol) {
    if (startRow + submatrix.rows > rows || startCol + submatrix.cols > cols)
        throw std::out_of_range("Submatrix doesn't fit in the current matrix");

    for (size_t row = 0; row < submatrix.rows; ++row)
        for (size_t col = 0; col < submatrix.cols; ++col)
          matrix[startRow + row][startCol + col] = submatrix.matrix[row][col];
}

DynamicMatrix DynamicMatrix::operator+(const DynamicMatrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("Matrix dimensions don't match for addition");

    DynamicMatrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];

    return result;
}

DynamicMatrix DynamicMatrix::operator-(const DynamicMatrix& other) const {
    if (rows != other.rows || cols != other.cols)
        throw std::invalid_argument("Matrix dimensions don't match for subtraction");

    DynamicMatrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            result.matrix[i][j] = matrix[i][j] - other.matrix[i][j];

    return result;
}

DynamicMatrix DynamicMatrix::operator*(const DynamicMatrix& other) const {
    if (cols != other.rows)
        throw std::invalid_argument("Matrix dimensions don't match for multiplication");

    DynamicMatrix result(rows, other.cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < other.cols; ++j) {
            Vector3D sum;
            for (size_t k = 0; k < cols; ++k)
                sum = sum + matrix[i][k] * other.matrix[k][j].x;
            result.matrix[i][j] = sum;
        }

    return result;
}

DynamicMatrix DynamicMatrix::operator*(double scalar) const {
    DynamicMatrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            result.matrix[i][j] = matrix[i][j] * scalar;

    return result;
}

void DynamicMatrix::print() const {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j)
            std::cout << matrix[i][j] << " ";

        std::cout << std::endl;
    }
}

void DynamicMatrix::deleteItem(size_t rowIndex, size_t colIndex) {
    if (rowIndex >= rows || colIndex >= cols) {
        throw std::out_of_range("Invalid index for deletion");
    }
    matrix[rowIndex][colIndex] = Vector3D();
}

void DynamicMatrix::addItem(size_t rowIndex, size_t colIndex, const Vector3D& vec) {
    if (rowIndex >= rows || colIndex >= cols) {
        throw std::out_of_range("Invalid index for addition");
    }
    matrix[rowIndex][colIndex] = vec;  // Insert the vector at the given position
}

void DynamicMatrix::addVectorAt(size_t rowIndex, size_t colIndex, const Vector3D& vec) {
    if (rowIndex >= rows || colIndex >= cols) {
        throw std::out_of_range("Invalid index for vector addition");
    }
    matrix[rowIndex][colIndex] = matrix[rowIndex][colIndex] + vec;  // Add vector
}

bool DynamicMatrix::operator==(const DynamicMatrix& other) const {
    if (rows != other.rows || cols != other.cols) return false;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (!(matrix[i][j] == other.matrix[i][j])) return false;
        }
    }
    return true;
}

bool DynamicMatrix::operator!=(const DynamicMatrix& other) const {
    return !(*this == other);
}

bool DynamicMatrix::operator<(const DynamicMatrix& other) const {
    return this->totalMagnitude() < other.totalMagnitude();
}

bool DynamicMatrix::operator>(const DynamicMatrix& other) const {
    return this->totalMagnitude() > other.totalMagnitude();
}

bool DynamicMatrix::operator<=(const DynamicMatrix& other) const {
    return !(*this > other);
}

bool DynamicMatrix::operator>=(const DynamicMatrix& other) const {
    return !(*this < other);
}

// Helper function to calculate total magnitude of vectors
double DynamicMatrix::totalMagnitude() const {
    double sum = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            sum += matrix[i][j].lenght();  // Use the length method of Vector3D
        }
    }
    return sum;
}

std::ostream& operator<<(std::ostream& os, const DynamicMatrix& mat) {
    for (size_t i = 0; i < mat.rows; ++i) {
        for (size_t j = 0; j < mat.cols; ++j) {
            os << mat.matrix[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}

std::istream& operator>>(std::istream& is, DynamicMatrix& mat) {
    for (size_t i = 0; i < mat.rows; ++i) {
        for (size_t j = 0; j < mat.cols; ++j) {
            is >> mat.matrix[i][j].x >> mat.matrix[i][j].y >> mat.matrix[i][j].z;
        }
    }
    return is;
}

void DynamicMatrix::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open file for writing");
    }

    file.write(reinterpret_cast<const char*>(&rows), sizeof(rows));
    file.write(reinterpret_cast<const char*>(&cols), sizeof(cols));

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            file.write(reinterpret_cast<const char*>(&matrix[i][j]), sizeof(Vector3D));
        }
    }
}

DynamicMatrix DynamicMatrix::loadFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open file for reading");
    }

    size_t rows, cols;
    file.read(reinterpret_cast<char*>(&rows), sizeof(rows));
    file.read(reinterpret_cast<char*>(&cols), sizeof(cols));

    DynamicMatrix result(rows, cols);

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            file.read(reinterpret_cast<char*>(&result.matrix[i][j]), sizeof(Vector3D));
        }
    }

    return result;
}
