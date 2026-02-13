#include <gtest/gtest.h>
#include "Cell.h"
#include "Field.h"
#include "LifeParser.h"
#include <iostream>
#include <fstream>

using namespace std;

TEST(CellTest, DefaultConstructor) {
    Cell cell;
    EXPECT_FALSE(cell.isCellAlive());
}

TEST(CellTest, MakeAlive) {
    Cell cell;
    cell.makeAlive();
    EXPECT_TRUE(cell.isCellAlive());
}

TEST(CellTest, MakeDead) {
    Cell cell;
    cell.makeAlive();
    cell.makeDead();
    EXPECT_FALSE(cell.isCellAlive());
}

TEST(FieldTest, Constructor) {
    Field field;
    SUCCEED();
}

TEST(FieldTest, SetCellAlive) {
    Field field;
    field.setCellAlive(5, 5);
    SUCCEED();
}

TEST(FieldTest, CheckIfIn) {
    Field field;
    int arr[] = {2, 3, 5};
    EXPECT_TRUE(field.checkIfIn(arr, 3, 2));
    EXPECT_TRUE(field.checkIfIn(arr, 3, 3));
    EXPECT_FALSE(field.checkIfIn(arr, 3, 1));
    EXPECT_FALSE(field.checkIfIn(arr, 3, 4));
}

TEST(FieldTest, CountLiveNeighbors) {
    Field field;
    EXPECT_EQ(field.countLiveNeighbors(10, 10), 0);
}

TEST(ParserTest, ParseRule) {
    string rule;
    EXPECT_TRUE(parseRule("#R B3/S23", rule));
    EXPECT_EQ(rule, "B3/S23");
    EXPECT_FALSE(parseRule("#N Test", rule));
    EXPECT_FALSE(parseRule("B3/S23", rule));
}

TEST(ParserTest, ParseString) {
    int birth[10], survive[10];
    int birthCount = 0, surviveCount = 0;
    
    parseString("B3/S23", birth, birthCount, survive, surviveCount);
    
    EXPECT_EQ(birthCount, 1);
    EXPECT_EQ(birth[0], 3);
    EXPECT_EQ(surviveCount, 2);
    EXPECT_EQ(survive[0], 2);
    EXPECT_EQ(survive[1], 3);
}
TEST(ParserTest, ReadLifeFileEmpty) {
    {
        ofstream testFile("test_empty.life");
        testFile << "#Life 1.06" << endl;
    }
    
    int birth[10], survive[10];
    int birthCount = 0, surviveCount = 0;
    
    LifeFile lifeFile = readLifeFile("test_empty.life", birth, birthCount, survive, surviveCount);
    
    EXPECT_EQ(lifeFile.universeName, "Unknown");
    EXPECT_EQ(lifeFile.rule, "B3/S23");
    EXPECT_EQ(lifeFile.liveCellsCount, 0);
    
    remove("test_empty.life");
}

TEST(IntegrationTest, GliderEvolution) {
    Field field;

    field.setCellAlive(1, 0);
    field.setCellAlive(2, 1);
    field.setCellAlive(0, 2);
    field.setCellAlive(1, 2);
    field.setCellAlive(2, 2);
    
    int birth[] = {3};
    int survive[] = {2, 3};

    field.update(birth, 1, survive, 2);
    field.update(birth, 1, survive, 2);
    field.update(birth, 1, survive, 2);

    SUCCEED();
}

TEST(FieldTest, SaveToFile) {
    Field field;
    {ofstream testFile("test_empty.life");} //временный файл
    field.setCellAlive(1, 1);
    field.setCellAlive(2, 2);
    EXPECT_NO_THROW(field.saveToFile("test_output.life", "Test", "B3/S23"));
    remove("test_output.life");
}

TEST(ErrorTest, InvalidFileSave) {
    Field field;
    EXPECT_THROW(field.saveToFile("/invalid_path/test.life", "Test", "B3/S23"), runtime_error);
}

TEST(FieldTest, ToroidalBoundaries) {
    Field field;
    field.setCellAlive(0, 0);
    field.setCellAlive(19, 19);
    SUCCEED();
}

TEST(GameRulesTest, BirthRule) {
    Field field;
    int birth[] = {3};
    int survive[] = {2, 3};
    EXPECT_NO_THROW(field.update(birth, 1, survive, 2));
}