#include "gtest/gtest.h"
#include "BitArray.h"


TEST(BitArrayTest, DefaultConstructor) {
    BitArray ba;
    EXPECT_EQ(ba.size(), 0);
    EXPECT_TRUE(ba.empty());
}

TEST(BitArrayTest, ParameterizedConstructor) {
    BitArray ba1(5, 0x0AUL); //01010
    EXPECT_EQ(ba1.size(), 5);
    EXPECT_EQ(ba1.to_string(), "01010");
    
    BitArray ba2(0, 0xFFUL); //11111111
    EXPECT_EQ(ba2.size(), 0);
    EXPECT_TRUE(ba2.empty());
}

TEST(BitArrayTest, InvalidConstructor) {
    EXPECT_THROW(BitArray(-1, 0x00UL), std::invalid_argument);
}


TEST(BitArrayTest, CopyConstructor) {
    BitArray original(4, 0x0AUL); //1010
    BitArray copy(original);
    EXPECT_EQ(copy.size(), original.size());
    EXPECT_EQ(copy.to_string(), original.to_string());
}

TEST(BitArrayTest, AssignmentOperator) {
    BitArray a(3, 0x05UL);//101
    BitArray b;
    b = a;
    EXPECT_EQ(b.size(), 3);
    EXPECT_EQ(b.to_string(), "101");
    
    b = b;
    EXPECT_EQ(b.to_string(), "101");

    BitArray c(5, 0x0AUL);//01010
    BitArray d(2, 0x01UL);//01
    d = c;
    EXPECT_EQ(d.size(), 5);
    EXPECT_EQ(d.to_string(), "01010");
}

TEST(BitArrayTest, Swap) {
    BitArray a(3, 0x01UL);//001
    BitArray b(4, 0x0AUL);//1010
    a.swap(b);
    
    EXPECT_EQ(a.size(), 4);
    EXPECT_EQ(a.to_string(), "1010");
    EXPECT_EQ(b.size(), 3);
    EXPECT_EQ(b.to_string(), "001");
}

TEST(BitArrayTest, Resize) {
    BitArray ba(2, 0x01UL);//01
    ba.resize(5, true);//11101
    EXPECT_EQ(ba.size(), 5);
    EXPECT_EQ(ba.to_string(), "11101");
    
    ba.resize(3, false);//101
    EXPECT_EQ(ba.size(), 3);
    EXPECT_EQ(ba.to_string(), "101");
}

TEST(BitArrayTest, InvalidResize) {
    BitArray ba;
    EXPECT_THROW(ba.resize(-1, true), std::invalid_argument);
}

TEST(BitArrayTest, Clear) {
    BitArray ba(5, 0x1FUL);//11111
    ba.clear();
    EXPECT_EQ(ba.size(), 0);
    EXPECT_TRUE(ba.empty());
}

TEST(BitArrayTest, PushBack) {
    BitArray ba;
    ba.push_back(true);
    ba.push_back(false);
    ba.push_back(true);
    EXPECT_EQ(ba.size(), 3);
    EXPECT_EQ(ba.to_string(), "101");
}

TEST(BitArrayTest, AndAssignment) {
    BitArray a(3, 0x05UL);//101
    BitArray b(3, 0x03UL);//011
    a &= b; //001
    EXPECT_EQ(a.to_string(), "001");
}

TEST(BitArrayTest, OrAssignment) {
    BitArray a(3, 0x05UL);//101
    BitArray b(3, 0x03UL);//011
    a |= b;//111
    EXPECT_EQ(a.to_string(), "111");
}

TEST(BitArrayTest, XorAssignment) {
    BitArray a(3, 0x05UL); // 101
    BitArray b(3, 0x03UL); // 011
    a ^= b;                // 110
    EXPECT_EQ(a.to_string(), "110");
}

TEST(BitArrayTest, OrAndXorAssigInvalidSize) {
    BitArray a(2, 0x01UL);
    BitArray b(3, 0x01UL);
    EXPECT_THROW(a &= b, std::invalid_argument);
    EXPECT_THROW(a |= b, std::invalid_argument);
    EXPECT_THROW(a ^= b, std::invalid_argument);
}

TEST(BitArrayTest, LeftShiftAssignment) {
    BitArray ba(4, 0x01UL);//0001
    ba <<= 2; //0100
    EXPECT_EQ(ba.to_string(), "0100");
    
    BitArray ba2(3, 0x07UL); //111
    ba2 <<= 3;//000
    EXPECT_EQ(ba2.to_string(), "000");
}

TEST(BitArrayTest, RightShiftAssignment) {
    BitArray ba(4, 0x08UL);//1000
    ba >>= 2;//0010
    EXPECT_EQ(ba.to_string(), "0010");
    
    BitArray ba2(3, 0x07UL); //111
    ba2 >>= 3;//000
    EXPECT_EQ(ba2.to_string(), "000");
}

TEST(BitArrayTest, InvalidShift) {
    BitArray ba(3, 0x01UL);
    EXPECT_THROW(ba <<= -1, std::invalid_argument);
    EXPECT_THROW(ba >>= -1, std::invalid_argument);
}

TEST(BitArrayTest, LeftShift) {
    BitArray ba(4, 0x01UL); //0001
    BitArray result = ba << 1; //0010
    EXPECT_EQ(result.to_string(), "0010");
    EXPECT_EQ(ba.to_string(), "0001");
}

TEST(BitArrayTest, RightShift) {
    BitArray ba(4, 0x08UL); //1000
    BitArray result = ba >> 1; //0100
    EXPECT_EQ(result.to_string(), "0100");
    EXPECT_EQ(ba.to_string(), "1000");
}

TEST(BitArrayTest, SetOneBit) {
    BitArray ba(3, 0x00UL); //000
    ba.set(1, true); //010
    EXPECT_EQ(ba.to_string(), "010");
    
    ba.set(0, false);
    EXPECT_EQ(ba.to_string(), "010");
}

TEST(BitArrayTest, SetAllBits) {
    BitArray ba(4, 0x00UL); //0000
    ba.set(); //1111
    EXPECT_EQ(ba.to_string(), "1111");
}

TEST(BitArrayTest, ResetOneBit) {
    BitArray ba(3, 0x07UL); //111
    ba.reset(1); //101
    EXPECT_EQ(ba.to_string(), "101");
}

TEST(BitArrayTest, ResetAllBits) {
    BitArray ba(4, 0x0FUL); //1111
    ba.reset();//0000
    EXPECT_EQ(ba.to_string(), "0000");
}

TEST(BitArrayTest, InvalidSetReset) {
    BitArray ba(3, 0x01UL);
    EXPECT_THROW(ba.set(-1, true), std::invalid_argument);
    EXPECT_THROW(ba.set(5, true), std::invalid_argument);
    EXPECT_THROW(ba.reset(-1), std::invalid_argument);
}

TEST(BitArrayTest, Any) {
    BitArray empty;
    EXPECT_FALSE(empty.any());
    
    BitArray zeros(3, 0x00UL); //000
    EXPECT_FALSE(zeros.any());
    
    BitArray ones(3, 0x07UL); //111
    EXPECT_TRUE(ones.any());
    
    BitArray mixed(3, 0x01UL); //001
    EXPECT_TRUE(mixed.any());
}

TEST(BitArrayTest, None) {
    BitArray empty;
    EXPECT_TRUE(empty.none());
    
    BitArray zeros(3, 0x00UL);
    EXPECT_TRUE(zeros.none());
    
    BitArray ones(3, 0x07UL);
    EXPECT_FALSE(ones.none());
    
    BitArray mixed(3, 0x01UL);
    EXPECT_FALSE(mixed.none());
}

TEST(BitArrayTest, Count) {
    BitArray empty;
    EXPECT_EQ(empty.count(), 0);
    
    BitArray zeros(4, 0x00UL);
    EXPECT_EQ(zeros.count(), 0);
    
    BitArray ones(4, 0x0FUL);
    EXPECT_EQ(ones.count(), 4);
    
    BitArray mixed(4, 0x0AUL); //1010
    EXPECT_EQ(mixed.count(), 2);
}

TEST(BitArrayTest, NotOperator) {
    BitArray ba(4, 0x0AUL); //1010
    BitArray inv = ~ba;//0101
    EXPECT_EQ(inv.to_string(), "0101");
    
    BitArray empty;
    BitArray inv_empty = ~empty;
    EXPECT_EQ(inv_empty.size(), 0);
    EXPECT_TRUE(inv_empty.empty());
}

TEST(BitArrayTest, IndexOperator) {
    BitArray ba(4, 0x05UL); //0101
    EXPECT_TRUE(ba[0]);
    EXPECT_FALSE(ba[1]);
    EXPECT_TRUE(ba[2]);
    EXPECT_FALSE(ba[3]);
}

TEST(BitArrayTest, InvalidIndex) {
    BitArray ba(3, 0x01UL);
    EXPECT_THROW(ba[-1], std::invalid_argument);
    EXPECT_THROW(ba[3], std::invalid_argument);
    EXPECT_THROW(ba[10], std::invalid_argument);
}

TEST(BitArrayTest, SizeAndEmpty) {
    BitArray empty;
    EXPECT_EQ(empty.size(), 0);
    EXPECT_TRUE(empty.empty());
    
    BitArray non_empty(1, 0x00UL);
    EXPECT_EQ(non_empty.size(), 1);
    EXPECT_FALSE(non_empty.empty());
}

TEST(BitArrayTest, ToString) {
    BitArray empty;
    EXPECT_EQ(empty.to_string(), "");
    
    BitArray single_zero(1, 0x00UL);
    EXPECT_EQ(single_zero.to_string(), "0");
    
    BitArray single_one(1, 0x01UL);
    EXPECT_EQ(single_one.to_string(), "1");
    
    BitArray mixed(4, 0x0AUL); //1010
    EXPECT_EQ(mixed.to_string(), "1010");
    
    BitArray all_ones(5, 0x1FUL);
    EXPECT_EQ(all_ones.to_string(), "11111");
}

TEST(BitArrayTest, Equal) {
    BitArray a(3, 0x05UL); //101
    BitArray b(3, 0x05UL); //101
    BitArray c(3, 0x03UL); //011
    BitArray d(2, 0x01UL); //01
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
    EXPECT_FALSE(a == d);
}

TEST(BitArrayTest, NotEqual) {
    BitArray a(3, 0x05UL); //101
    BitArray b(3, 0x05UL); //101
    BitArray c(3, 0x03UL); //011
    
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);
}

TEST(BitArrayTest, AndOperator) {
    BitArray a(3, 0x05UL); //101
    BitArray b(3, 0x03UL); //011
    BitArray result = a & b; //001
    EXPECT_EQ(result.to_string(), "001");
}

TEST(BitArrayTest, OrOperator) {
    BitArray a(3, 0x05UL); //101
    BitArray b(3, 0x03UL); //011
    BitArray result = a | b; //111
    EXPECT_EQ(result.to_string(), "111");
}

TEST(BitArrayTest, XorOperator) {
    BitArray a(3, 0x05UL); //101
    BitArray b(3, 0x03UL); //011
    BitArray result = a ^ b; //110
    EXPECT_EQ(result.to_string(), "110");
}

TEST(BitArrayTest, AndOrXorInvalidSize) {
    BitArray a(2, 0x01UL);
    BitArray b(3, 0x01UL);
    EXPECT_THROW(a & b, std::invalid_argument);
    EXPECT_THROW(a | b, std::invalid_argument);
    EXPECT_THROW(a ^ b, std::invalid_argument);
}

TEST(BitArrayTest, LargeSize) {
    const int large_size = 70;
    BitArray ba(large_size, 0x00UL);

    ba.set(0, true);
    ba.set(65, true);
    
    EXPECT_TRUE(ba[0]);
    EXPECT_TRUE(ba[65]);
    EXPECT_EQ(ba.count(), 2);

    std::string expected(large_size, '0');
    expected[large_size - 1] = '1';
    expected[large_size - 66] = '1';
    EXPECT_EQ(ba.to_string(), expected);
}
