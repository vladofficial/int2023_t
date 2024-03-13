#pragma once
#include <cinttypes>
#include <iostream>

struct int2023_t {
    uint8_t value[253] = {};
    static const uint8_t kNumberOfDigits = 253;
    static const int16_t kNumSystemBase = 256;
    static const uint8_t kMaxDigitValue = 255;
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t from_int(int32_t i);

int2023_t operator-(const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

bool check_negative(int2023_t& num);

int2023_t operator<<(const int2023_t&lhs, int &rhs);

int2023_t operator>>(const int2023_t&lhs, int &rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

bool operator>(const int2023_t& lhs, const int2023_t& rhs);

bool operator<(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

int2023_t from_string(const char* buff);

