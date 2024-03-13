#include "number.h"
#include<cstring> 

int2023_t operator+ (const int2023_t& lhs, const int2023_t& rhs) {
  int temp = 0;
  int2023_t result;
  for (uint8_t digit = 0; digit < int2023_t::kNumberOfDigits; ++digit) {
    temp = lhs.value[digit] + rhs.value[digit];
    if (temp + result.value[digit] > int2023_t::kMaxDigitValue) {
      result.value[digit] = (temp + result.value[digit]) % int2023_t::kNumSystemBase;
      result.value[digit + 1] = ++result.value[digit + 1];
    } else {
      result.value[digit] += temp;
    }
  }

  return result;
}

int2023_t from_int (int32_t i) {
  int2023_t num;
  bool is_negative = (i < 0);
  int64_t new_i = static_cast<int64_t>(i);
  if(i < 0) {
    new_i = -new_i;
  }
  int digit_iterator = 0;
  while (new_i > 0) {
    num.value[digit_iterator] = new_i % int2023_t::kNumSystemBase;
    new_i /= int2023_t::kNumSystemBase;
    ++digit_iterator;
  }
  if (is_negative) {
    int2023_t one;
    one.value[0] = 1;
    for (uint8_t digit = 0; digit < int2023_t::kNumberOfDigits; ++digit) {
      num.value[digit] = int2023_t::kMaxDigitValue  - num.value[digit];
    }
    num = num + one;
  }

  return num;
}

int2023_t operator- (const int2023_t& rhs){
  int2023_t num = rhs;
  for (uint8_t digit = 0; digit < int2023_t::kNumberOfDigits; ++digit) {
    num.value[digit] = int2023_t::kMaxDigitValue  - rhs.value[digit];
  }
  num = num + from_int(1);

  return num;
}

int2023_t operator- (const int2023_t& lhs, const int2023_t& rhs) {
  return lhs + (-rhs);
}

bool check_negative (const int2023_t& num) {
  return (num.value[int2023_t::kNumberOfDigits - 1] > 127);
}

int2023_t operator<< (const int2023_t&lhs, int &rhs) {
  int2023_t num = lhs;
  for(int digit = int2023_t::kNumberOfDigits - 1; digit  >= rhs; --digit) {
    num.value[digit] = lhs.value[digit - rhs];
  }
  for(int digit = 0; digit < rhs; ++digit) {
    num.value[digit] = 0;
  }
  return num;
}

int2023_t operator>> (const int2023_t&lhs, int &rhs) {
  int2023_t num = lhs;
  for(uint8_t digit = 0; digit < rhs; ++digit) {
    num.value[digit] = 0;
  }
  for(int digit = 0; digit < int2023_t::kNumberOfDigits - 1 - rhs; ++digit) {
    num.value[digit] = num.value[digit + rhs];
  }
  return num;
}

int2023_t operator* (const int2023_t& lhs, const int2023_t& rhs) {
  int2023_t sum;
  int one = 1;
  bool is_negative = false;
  int2023_t rhs_copy = rhs;
  int2023_t shifted_lhs = lhs;
  if (check_negative(lhs) && !check_negative(rhs)) {
    is_negative = true;
    shifted_lhs = -lhs;
  }
  else if (!check_negative(lhs) && check_negative(rhs)) {
    rhs_copy = -rhs;
    is_negative = true;
  }
  else if (check_negative(lhs) && check_negative(rhs)) {
    shifted_lhs = -lhs;
    rhs_copy = -rhs;
    is_negative = false;
  }
  for (int rhs_digits = 0; rhs_digits < int2023_t::kNumberOfDigits; ++rhs_digits) {
    for (int rhs_value = rhs_copy.value[rhs_digits]; rhs_value > 0; --rhs_value) {
      sum = sum + shifted_lhs;
    }
    shifted_lhs = shifted_lhs << one;
  }

  return is_negative ? -sum : sum;
}

bool operator> (const int2023_t& lhs, const int2023_t& rhs) {
  if (check_negative(lhs) && !check_negative(rhs)) {
    return false;
  }
  if (check_negative(rhs) && !check_negative(lhs)) {
    
    return true;
  }
  if (check_negative(rhs) && check_negative(lhs)){
    for (int i = int2023_t::kNumberOfDigits - 1; i >= 0; --i) {
      if (lhs.value[i] == rhs.value[i]) {
        continue;
      }

      return (lhs.value[i] > rhs.value[i]);
    }
  }
  for (int digit = int2023_t::kNumberOfDigits; digit >= 0; --digit) {
    if (lhs.value[digit] == rhs.value[digit]) {
      continue;
    }
    return (lhs.value[digit] > rhs.value[digit]);
  }

  return false;
}

bool operator< (const int2023_t& lhs, const int2023_t& rhs) {
  return (rhs > lhs);
}

std::ostream& operator<< (std::ostream& stream, const int2023_t& value) {
  int2023_t outputed_value = value;
  if (check_negative(value)){
    outputed_value = -value;
    stream << "-0x";
  }
  else{
    stream << "0x";
  }
  bool non_zero_digit_reached = false;
  for (int digit = int2023_t::kNumberOfDigits - 1; digit >= 0; --digit) {
    if (outputed_value.value[digit] == 0 && non_zero_digit_reached) {
      stream << "00";
    }
    else if (outputed_value.value[digit] / 16 > 0) {
      non_zero_digit_reached = true;
      if (outputed_value.value[digit] % 16 > 9 && outputed_value.value[digit] / 16 > 9) {
        stream << static_cast<uint8_t>('A' + outputed_value.value[digit] / 16 - 10);
        stream << static_cast<uint8_t>('A' + outputed_value.value[digit] % 16 - 10);
      }
      else if (outputed_value.value[digit] % 16 > 9 && outputed_value.value[digit] / 16 <= 9) {
        stream << static_cast<uint8_t>('0' + outputed_value.value[digit] / 16);
        stream << static_cast<uint8_t>('A' + outputed_value.value[digit] % 16 - 10) ;
      }
      else if (outputed_value.value[digit] % 16 <= 9 && outputed_value.value[digit] / 16 > 9) {
        stream << static_cast<uint8_t>('A' + outputed_value.value[digit] / 16 - 10);
        stream << static_cast<uint8_t>('0' + outputed_value.value[digit] % 16);
      } else {
        stream << static_cast<uint8_t>('0' + outputed_value.value[digit] / 16);
        stream << static_cast<uint8_t>('0' + outputed_value.value[digit] % 16);
      }
    } else {
      non_zero_digit_reached = true;
      if (outputed_value.value[digit] > 9) {
        stream << static_cast<uint8_t>('A' + outputed_value.value[digit] - 10);
      } else {
        stream << static_cast<uint8_t>('0' + outputed_value.value[digit]);
      }
    }
  }
}

bool operator== (const int2023_t& lhs, const int2023_t& rhs) {
  for (int i = 0; i < int2023_t::kNumberOfDigits; ++i) {
    if (lhs.value[i] != rhs.value[i]) {
      return false;
    }
  }

  return true;
}

int2023_t operator/ (const int2023_t& lhs, const int2023_t& rhs) {
  bool is_negative = false;
  int2023_t rhs_copy = rhs;
  int2023_t lhs_copy = lhs;
  if(check_negative(lhs) && !check_negative(rhs)) {
    is_negative = true;
    lhs_copy = -lhs;
  }
  else if(!check_negative(lhs) && check_negative(rhs)) {
    rhs_copy = -rhs;
    is_negative = true;
  }
  else if(check_negative(lhs) && check_negative(rhs)) {
    lhs_copy = -lhs;
    rhs_copy = -rhs;
    is_negative = false;
  }
  bool lflag = false;
  bool rflag = false;
  int ldigits = 0;
  int rdigits = 0;
  int2023_t ans;
  for(int digit = int2023_t::kNumberOfDigits - 1; digit >= 0; --digit) {
    if(rhs_copy.value[digit] != 0 && !rflag) {
      rflag = true;
      rdigits = digit + 1;
    }
    if(lhs_copy.value[digit] != 0 && !lflag) {
      lflag = true;
      ldigits = digit + 1;
    }
    if(rflag && lflag) {
      break;
    }
  }
  if(ldigits < rdigits) {
    return from_int(0);
  }
  int shift = ldigits - rdigits;
  rhs_copy = rhs_copy << shift;
  int one = 1;
  int2023_t one_2023 = from_int(1);
  if(rhs_copy > lhs_copy) {
    rhs_copy = rhs_copy >> one;
    --shift;
  }
  for(int32_t i = shift; i >= 0; --i) {
    int2023_t rhs_copy_negative = -rhs_copy;
    int2023_t ans_temp;
    while(lhs_copy > rhs_copy || lhs_copy == rhs_copy) {
      lhs_copy = lhs_copy + rhs_copy_negative;
      ans_temp = ans_temp + one_2023;
    }
    ans = ans + (ans_temp << i);
    rhs_copy = rhs_copy >> one;
  }

  return is_negative ? -ans : ans;
}

bool operator!= (const int2023_t& lhs, const int2023_t& rhs) {
  return !(lhs == rhs);
}

int2023_t from_string (const char* buff) {
  std::size_t length = std::strlen(buff);
  bool is_negative = (buff[0] == '-');
  int2023_t ans;
  int2023_t ten = from_int(10);
  for (std::size_t digit = 0 + is_negative; digit < length; ++digit) {
    ans = ans * ten + from_int(buff[digit] - '0');
  }

  return is_negative ? -ans : ans;
}