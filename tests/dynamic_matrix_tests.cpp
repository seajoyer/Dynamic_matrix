#define CATCH_CONFIG_MAIN
#include <catch/catch.hpp>
#include "dynamic_matrix.h"

TEST_CASE("DynamicMatrix: Edge Cases", "[DynamicMatrix]") {
    SECTION("Empty matrix construction") {
        DynamicMatrix emptyMatrix;
        CHECK(emptyMatrix.getRows() == 0);
        CHECK(emptyMatrix.getCols() == 0);
    }

    SECTION("Single element matrix") {
        DynamicMatrix singleMatrix(1, 1);
        singleMatrix.at(0, 0) = Vector3D(1, 2, 3);
        CHECK(singleMatrix.at(0, 0).x == 1);
        CHECK(singleMatrix.at(0, 0).y == 2);
        CHECK(singleMatrix.at(0, 0).z == 3);
    }
}

TEST_CASE("DynamicMatrix Construction and Basic Operations", "[DynamicMatrix]") {
    SECTION("Constructor and size") {
        DynamicMatrix matrix(3, 4);
        CHECK(matrix.getRows() == 3);
        CHECK(matrix.getCols() == 4);
    }

    SECTION("Element access and modification") {
        DynamicMatrix matrix(2, 2);
        matrix.at(0, 0) = Vector3D(1, 2, 3);
        matrix.at(0, 1) = Vector3D(4, 5, 6);
        matrix.at(1, 0) = Vector3D(7, 8, 9);
        matrix.at(1, 1) = Vector3D(10, 11, 12);

        CHECK(matrix.at(0, 0).x == 1);
        CHECK(matrix.at(0, 1).y == 5);
        CHECK(matrix.at(1, 0).z == 9);
        CHECK(matrix.at(1, 1).x == 10);

        CHECK_THROWS_AS(matrix.at(2, 0), const std::out_of_range&);
        CHECK_THROWS_AS(matrix.at(0, 2), const std::out_of_range&);
    }

    SECTION("Copy constructor and assignment operator") {
        DynamicMatrix matrix1(2, 2);
        matrix1.at(0, 0) = Vector3D(1, 2, 3);
        matrix1.at(1, 1) = Vector3D(4, 5, 6);

        DynamicMatrix matrix2(matrix1);
        CHECK(matrix2.at(0, 0).x == 1);
        CHECK(matrix2.at(1, 1).z == 6);

        DynamicMatrix matrix3(1, 1);
        matrix3 = matrix1;
        CHECK(matrix3.getRows() == 2);
        CHECK(matrix3.getCols() == 2);
        CHECK(matrix3.at(0, 0).y == 2);
        CHECK(matrix3.at(1, 1).x == 4);
    }
}

TEST_CASE("DynamicMatrix Row and Column Operations", "[DynamicMatrix]") {
    SECTION("Delete row") {
        DynamicMatrix matrix(3, 2);
        matrix.at(0, 0) = Vector3D(1, 2, 3);
        matrix.at(1, 0) = Vector3D(4, 5, 6);
        matrix.at(2, 0) = Vector3D(7, 8, 9);

        matrix.deleteRow(1);
        CHECK(matrix.getRows() == 2);
        CHECK(matrix.at(1, 0).x == 7);

        CHECK_THROWS_AS(matrix.deleteRow(2), const std::out_of_range&);
    }

    SECTION("Delete column") {
        DynamicMatrix matrix(2, 3);
        matrix.at(0, 0) = Vector3D(1, 2, 3);
        matrix.at(0, 1) = Vector3D(4, 5, 6);
        matrix.at(0, 2) = Vector3D(7, 8, 9);

        matrix.deleteColumn(1);
        CHECK(matrix.getCols() == 2);
        CHECK(matrix.at(0, 1).x == 7);

        CHECK_THROWS_AS(matrix.deleteColumn(2), const std::out_of_range&);
    }

    SECTION("Insert row") {
        DynamicMatrix matrix(2, 2);
        Vector3D newRow[2] = {Vector3D(1, 2, 3), Vector3D(4, 5, 6)};
        matrix.insertRow(0, newRow);
        matrix.insertRow(2, newRow);

        CHECK(matrix.getRows() == 4);
        CHECK(matrix.at(0, 0).x == 1);
        CHECK(matrix.at(0, 1).z == 6);
    }

    SECTION("Insert column") {
        DynamicMatrix matrix(2, 2);
        Vector3D newColumn[2] = {Vector3D(1, 2, 3), Vector3D(4, 5, 6)};
        matrix.insertColumn(0, newColumn);
        matrix.insertColumn(2, newColumn);

        CHECK(matrix.getCols() == 4);
        CHECK(matrix.at(1, 1).y == 0);
        CHECK(matrix.at(1, 2).z == 6);
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
        CHECK(result.at(0, 0).x == 3);
        CHECK(result.at(0, 0).y == 5);
        CHECK(result.at(1, 1).z == 13);

        DynamicMatrix matrix3(3, 2);
        CHECK_THROWS_AS(matrix1 + matrix3, const std::invalid_argument&);
    }

    SECTION("Matrix subtraction") {
        DynamicMatrix matrix1(2, 2);
        matrix1.at(0, 0) = Vector3D(5, 6, 7);
        matrix1.at(1, 1) = Vector3D(8, 9, 10);

        DynamicMatrix matrix2(2, 2);
        matrix2.at(0, 0) = Vector3D(1, 2, 3);
        matrix2.at(1, 1) = Vector3D(4, 5, 6);

        DynamicMatrix result = matrix1 - matrix2;
        CHECK(result.at(0, 0).x == 4);
        CHECK(result.at(0, 0).y == 4);
        CHECK(result.at(1, 1).z == 4);

        DynamicMatrix matrix3(3, 2);
        CHECK_THROWS_AS(matrix1 - matrix3, const std::invalid_argument&);
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
        CHECK(result.getRows() == 2);
        CHECK(result.getCols() == 2);
        CHECK(result.at(0, 0).x == 58);
        CHECK(result.at(0, 1).x == 64);
        CHECK(result.at(1, 0).x == 139);
        CHECK(result.at(1, 1).x == 154);

        DynamicMatrix matrix3(2, 2);
        CHECK_THROWS_AS(matrix1 * matrix3, const std::invalid_argument&);
    }

    SECTION("Scalar multiplication") {
        DynamicMatrix matrix(2, 2);
        matrix.at(0, 0) = Vector3D(1, 2, 3);
        matrix.at(0, 1) = Vector3D(4, 5, 6);
        matrix.at(1, 0) = Vector3D(7, 8, 9);
        matrix.at(1, 1) = Vector3D(10, 11, 12);

        DynamicMatrix result = matrix * 2;
        CHECK(result.at(0, 0).x == 2);
        CHECK(result.at(0, 1).y == 10);
        CHECK(result.at(1, 0).z == 18);
        CHECK(result.at(1, 1).x == 20);
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

        CHECK(matrix.at(1, 1).x == 100);
        CHECK(matrix.at(1, 2).y == 200);
        CHECK(matrix.at(2, 1).z == 300);
        CHECK(matrix.at(2, 2).x == 400);
        CHECK(matrix.at(0, 0).x == 0);
        CHECK(matrix.at(3, 3).x == 6);

        CHECK_THROWS_AS(matrix.insertSubmatrix(submatrix, 3, 3), const std::out_of_range&);
    }
}

TEST_CASE("DynamicMatrix: addItem and deleteItem") {
    DynamicMatrix matrix(3, 3);
    Vector3D vec(1.0, 2.0, 3.0);

    matrix.addItem(1, 1, vec);
    REQUIRE(matrix.at(1, 1).x == 1.0);
    REQUIRE(matrix.at(1, 1).y == 2.0);
    REQUIRE(matrix.at(1, 1).z == 3.0);

    matrix.deleteItem(1, 1);
    REQUIRE(matrix.at(1, 1).x == 0.0);
}

TEST_CASE("DynamicMatrix: addVectorAt") {
    DynamicMatrix matrix(3, 3);
    Vector3D vec1(1.0, 1.0, 1.0);
    Vector3D vec2(2.0, 2.0, 2.0);

    matrix.addItem(1, 1, vec1);
    matrix.addVectorAt(1, 1, vec2);

    REQUIRE(matrix.at(1, 1).x == 3.0);
    REQUIRE(matrix.at(1, 1).y == 3.0);
    REQUIRE(matrix.at(1, 1).z == 3.0);
}

TEST_CASE("DynamicMatrix: Comparison Operators") {
    DynamicMatrix matrix1(2, 2);
    DynamicMatrix matrix2(2, 2);

    matrix1.addItem(0, 0, Vector3D(1.0, 2.0, 3.0));
    matrix2.addItem(0, 0, Vector3D(1.0, 2.0, 3.0));

    REQUIRE(matrix1 == matrix2);

    matrix2.addItem(0, 0, Vector3D(4.0, 5.0, 6.0));
    REQUIRE(matrix1 != matrix2);

    DynamicMatrix matrix3(2, 2);
    matrix3.addItem(0, 0, Vector3D(0.5, 1.0, 1.5));
    REQUIRE(matrix3 < matrix1);
    REQUIRE(matrix2 > matrix1);
    REQUIRE(matrix1 <= matrix1);
    REQUIRE(matrix1 >= matrix3);
}

TEST_CASE("DynamicMatrix: Move Constructor and Assignment Operator", "[DynamicMatrix]") {
    DynamicMatrix matrix1(2, 2);
    matrix1.at(0, 0) = Vector3D(1, 2, 3);
    matrix1.at(1, 1) = Vector3D(4, 5, 6);

    DynamicMatrix matrix2(std::move(matrix1));
    CHECK(matrix2.at(0, 0).x == 1);
    CHECK(matrix2.at(1, 1).z == 6);
    CHECK(matrix1.getRows() == 0);
    CHECK(matrix1.getCols() == 0);

    DynamicMatrix matrix3(1, 1);
    matrix3 = std::move(matrix2);
    CHECK(matrix3.getRows() == 2);
    CHECK(matrix3.getCols() == 2);
    CHECK(matrix3.at(0, 0).y == 2);
    CHECK(matrix3.at(1, 1).x == 4);
    CHECK(matrix2.getRows() == 0);
    CHECK(matrix2.getCols() == 0);
}

TEST_CASE("DynamicMatrix: File I/O Operations", "[DynamicMatrix]") {
    DynamicMatrix matrix(2, 2);
    matrix.at(0, 0) = Vector3D(1, 2, 3);
    matrix.at(0, 1) = Vector3D(4, 5, 6);
    matrix.at(1, 0) = Vector3D(7, 8, 9);
    matrix.at(1, 1) = Vector3D(10, 11, 12);

    SECTION("Save to file and load from file") {
        std::string filename = "test_matrix.bin";
        matrix.saveToFile(filename);

        DynamicMatrix loaded = DynamicMatrix::loadFromFile(filename);
        CHECK(loaded == matrix);

        // Clean up the test file
        std::remove(filename.c_str());
    }

    SECTION("Save to file - file open failure") {
        std::string invalidFilename = "/invalid/path/test_matrix.bin";
        CHECK_THROWS_AS(matrix.saveToFile(invalidFilename), const std::runtime_error&);
    }

    SECTION("Load from file - file open failure") {
        std::string nonExistentFile = "non_existent_file.bin";
        CHECK_THROWS_AS(DynamicMatrix::loadFromFile(nonExistentFile), const std::runtime_error&);
    }
}
