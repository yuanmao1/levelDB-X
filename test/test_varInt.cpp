#include <gtest/gtest.h>
#include <util/coding.h>

TEST(test_varInt, test_varInt32) {
    uint8_t buf[5] = {0};
    auto    res    = dbx::util::encodeVarInt32(buf, 0x12345678);
    EXPECT_EQ(dbx::util::decodeVarInt32(buf).value().first, 0x12345678);
}

TEST(test_varInt, test_varInt64) {
    uint8_t buf[10] = {0};
    auto    res     = dbx::util::encodeVarInt64(buf, 0x123456789ABCDEF0);
    EXPECT_EQ(dbx::util::decodeVarInt64(buf).value().first, 0x123456789ABCDEF0);
}