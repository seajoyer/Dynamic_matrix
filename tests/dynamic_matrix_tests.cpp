#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include "dynamic_matrix.h"

TEST_CASE("DynamicMatrix Construction and Basic Operations", "[DynamicMatrix]") {
    SECTION("Constructor and size") {
        DynamicMatrix matrix(3, 4);
        REQUIRE(matrix.getRows() == 3);
        REQUIRE(matrix.getCols() == 4);
    }

    SECTION("Element access and modification") {
        DynamicMatrix matrix(2, 2);
        matrix.at(0, 0) = Vector3D(1, 2, 3);
        matrix.at(0, 1) = Vector3D(4, 5, 6);
        matrix.at(1, 0) = Vector3D(7, 8, 9);
        matrix.at(1, 1) = Vector3D(10, 11, 12);

        REQUIRE(matrix.at(0, 0).x == 1);
        REQUIRE(matrix.at(0, 1).y == 5);
        REQUIRE(matrix.at(1, 0).z == 9);
        REQUIRE(matrix.at(1, 1).x == 10);

        REQUIRE_THROWS_AS(matrix.at(2, 0), std::out_of_range);
        REQUIRE_THROWS_AS(matrix.at(0, 2), std::out_of_range);
    }

    SECTION("Copy constructor and assignment operator") {
        DynamicMatrix matrix1(2, 2);
        matrix1.at(0, 0) = Vector3D(1, 2, 3);
        matrix1.at(1, 1) = Vector3D(4, 5, 6);

        DynamicMatrix matrix2(matrix1);
        REQUIRE(matrix2.at(0, 0).x == 1);
        REQUIRE(matrix2.at(1, 1).z == 6);

        DynamicMatrix matrix3(1, 1);
        matrix3 = matrix1;
        REQUIRE(matrix3.getRows() == 2);
        REQUIRE(matrix3.getCols() == 2);
        REQUIRE(matrix3.at(0, 0).y == 2);
        REQUIRE(matrix3.at(1, 1).x == 4);
    }
}

TEST_CASE("DynamicMatrix Row and Column Operations", "[DynamicMatrix]") {
    SECTION("Delete row") {
        DynamicMatrix matrix(3, 2);
        matrix.at(0, 0) = Vector3D(1, 2, 3);
        matrix.at(1, 0) = Vector3D(4, 5, 6);
        matrix.at(2, 0) = Vector3D(7, 8, 9);

        matrix.deleteRow(1);
        REQUIRE(matrix.getRows() == 2);
        REQUIRE(matrix.at(1, 0).x == 7);

        REQUIRE_THROWS_AS(matrix.deleteRow(2), std::out_of_range);
    }

    SECTION("Delete column") {
        DynamicMatrix matrix(2, 3);
        matrix.at(0, 0) = Vector3D(1, 2, 3);
        matrix.at(0, 1) = Vector3D(4, 5, 6);
        matrix.at(0, 2) = Vector3D(7, 8, 9);

        matrix.deleteColumn(1);
        REQUIRE(matrix.getCols() == 2);
        REQUIRE(matrix.at(0, 1).x == 7);

        REQUIRE_THROWS_AS(matrix.deleteColumn(2), std::out_of_range);
    }

    SECTION("Insert row") {
        DynamicMatrix matrix(2, 2);
        Vector3D newRow[2] = {Vector3D(1, 2, 3), Vector3D(4, 5, 6)};
        matrix.insertRow(0, newRow);
        matrix.insertRow(2, newRow);

        REQUIRE(matrix.getRows() == 4);
        REQUIRE(matrix.at(0, 0).x == 1);
        REQUIRE(matrix.at(0, 1).z == 6);
    }

    SECTION("Insert column") {
        DynamicMatrix matrix(2, 2);
        Vector3D newColumn[2] = {Vector3D(1, 2, 3), Vector3D(4, 5, 6)};
        matrix.insertColumn(0, newColumn);
        matrix.insertColumn(2, newColumn);

        REQUIRE(matrix.getCols() == 4);
        REQUIRE(matrix.at(1, 1).y == 0);
        REQUIRE(matrix.at(1, 2).z == 6);
    }
}

TEST_CASE("DynamicMatrix Arithmetic Operations", "[DynamicMatrix]") {
    SECTION("Matrix addition") {
        DynamicMatrix matrix1(2, 2);
        matrix1.at(0, 0) = Vector3D(1, 2, 3);
        matrix1.at(1, 1) = Vector3D(4, 5, 6);

        DynamicMatrix matrix2(2, 2);
        matrix2.at(0, 0) = Vector3D(2, 3, 4);
        matrix2.at(1, 1) = Vector3D(5, 6, 7);

        DynamicMatrix result = matrix1 + matrix2;
        REQUIRE(result.at(0, 0).x == 3);
        REQUIRE(result.at(0, 0).y == 5);
        REQUIRE(result.at(1, 1).z == 13);

        DynamicMatrix matrix3(3, 2);
        REQUIRE_THROWS_AS(matrix1 + matrix3, std::invalid_argument);
    }

    SECTION("Matrix subtraction") {
        DynamicMatrix matrix1(2, 2);
        matrix1.at(0, 0) = Vector3D(5, 6, 7);
        matrix1.at(1, 1) = Vector3D(8, 9, 10);

        DynamicMatrix matrix2(2, 2);
        matrix2.at(0, 0) = Vector3D(1, 2, 3);
        matrix2.at(1, 1) = Vector3D(4, 5, 6);

        DynamicMatrix result = matrix1 - matrix2;
        REQUIRE(result.at(0, 0).x == 4);
        REQUIRE(result.at(0, 0).y == 4);
        REQUIRE(result.at(1, 1).z == 4);

        DynamicMatrix matrix3(3, 2);
        REQUIRE_THROWS_AS(matrix1 - matrix3, std::invalid_argument);
    }

    SECTION("Matrix multiplication") {
        DynamicMatrix matrix1(2, 3);
        matrix1.at(0, 0) = Vector3D(1, 0, 0);
        matrix1.at(0, 1) = Vector3D(2, 0, 0);
        matrix1.at(0, 2) = Vector3D(3, 0, 0);
        matrix1.at(1, 0) = Vector3D(4, 0, 0);
        matrix1.at(1, 1) = Vector3D(5, 0, 0);
        matrix1.at(1, 2) = Vector3D(6, 0, 0);

        DynamicMatrix matrix2(3, 2);
        matrix2.at(0, 0) = Vector3D(7, 0, 0);
        matrix2.at(0, 1) = Vector3D(8, 0, 0);
        matrix2.at(1, 0) = Vector3D(9, 0, 0);
        matrix2.at(1, 1) = Vector3D(10, 0, 0);
        matrix2.at(2, 0) = Vector3D(11, 0, 0);
        matrix2.at(2, 1) = Vector3D(12, 0, 0);

        DynamicMatrix result = matrix1 * matrix2;
        REQUIRE(result.getRows() == 2);
        REQUIRE(result.getCols() == 2);
        REQUIRE(result.at(0, 0).x == 58);
        REQUIRE(result.at(0, 1).x == 64);
        REQUIRE(result.at(1, 0).x == 139);
        REQUIRE(result.at(1, 1).x == 154);

        DynamicMatrix matrix3(2, 2);
        REQUIRE_THROWS_AS(matrix1 * matrix3, std::invalid_argument);
    }

    SECTION("Scalar multiplication") {
        DynamicMatrix matrix(2, 2);
        matrix.at(0, 0) = Vector3D(1, 2, 3);
        matrix.at(0, 1) = Vector3D(4, 5, 6);
        matrix.at(1, 0) = Vector3D(7, 8, 9);
        matrix.at(1, 1) = Vector3D(10, 11, 12);

        DynamicMatrix result = matrix * 2;
        REQUIRE(result.at(0, 0).x == 2);
        REQUIRE(result.at(0, 1).y == 10);
        REQUIRE(result.at(1, 0).z == 18);
        REQUIRE(result.at(1, 1).x == 20);
    }
}

TEST_CASE("DynamicMatrix Submatrix Insertion", "[DynamicMatrix]") {
    SECTION("Insert submatrix") {
        DynamicMatrix matrix(4, 4);
        for (size_t i = 0; i < 4; ++i) {
            for (size_t j = 0; j < 4; ++j) {
                matrix.at(i, j) = Vector3D(i + j, i + j, i + j);
            }
        }

        DynamicMatrix submatrix(2, 2);
        submatrix.at(0, 0) = Vector3D(100, 100, 100);
        submatrix.at(0, 1) = Vector3D(200, 200, 200);
        submatrix.at(1, 0) = Vector3D(300, 300, 300);
        submatrix.at(1, 1) = Vector3D(400, 400, 400);

        matrix.insertSubmatrix(submatrix, 1, 1);

        REQUIRE(matrix.at(1, 1).x == 100);
        REQUIRE(matrix.at(1, 2).y == 200);
        REQUIRE(matrix.at(2, 1).z == 300);
        REQUIRE(matrix.at(2, 2).x == 400);
        REQUIRE(matrix.at(0, 0).x == 0);
        REQUIRE(matrix.at(3, 3).x == 6);

        REQUIRE_THROWS_AS(matrix.insertSubmatrix(submatrix, 3, 3), std::out_of_range);
    }
}
