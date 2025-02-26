#include <gtest/gtest.h>
#include <util/coding.h>

TEST(test_varInt, test_varInt32) {
    uint8_t buf[5] = {0};
    auto    res    = dbx::util::encodeVarInt32(buf, 0x12345678);
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res.value(), 5);
    ASSERT_EQ(buf[0], 0x78);
    ASSERT_EQ(buf[1], 0x96);
    ASSERT_EQ(buf[2], 0x34);
    ASSERT_EQ(buf[3], 0x12);
    ASSERT_EQ(buf[4], 0x00);
    EXPECT_EQ(dbx::util::decodeVarInt32(buf).value().first, 0x12345678);
}

TEST(test_varInt, test_varInt64) {
    uint8_t buf[10] = {0};
    auto    res     = dbx::util::encodeVarInt64(buf, 0x123456789ABCDEF0);
    ASSERT_TRUE(res.has_value());
    ASSERT_EQ(res.value(), 10);
    ASSERT_EQ(buf[0], 0xF0);
    ASSERT_EQ(buf[1], 0xEF);
    ASSERT_EQ(buf[2], 0xCD);
    ASSERT_EQ(buf[3], 0xAB);
    ASSERT_EQ(buf[4], 0x89);
    ASSERT_EQ(buf[5], 0x67);
    ASSERT_EQ(buf[6], 0x45);
    ASSERT_EQ(buf[7], 0x23);
    ASSERT_EQ(buf[8], 0x01);
    ASSERT_EQ(buf[9], 0x00);
    EXPECT_EQ(dbx::util::decodeVarInt64(buf).value().first, 0x123456789ABCDEF0);
}