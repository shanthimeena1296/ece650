#include <iostream>
#include <sstream>
#include <vector>

#include <cmath>

std::string prime_factor(unsigned v) {
  std::stringstream out;

  // YOUR CODE GOES HERE
  unsigned int value = v;
  unsigned int count = 0; 

  for (unsigned int i = 2; i <= v / 2; i++)
  {
    while (value % i == 0) 
      {
        value = value/i;
        count ++;
      }
      if (count > 0) 
      {
        out << i;
        if (count > 1)
        {
          out << '^' << count;
        }
        if (value > 1)
        {
           out << " x ";
        }
      }
      count = 0;
  }
  if (out.str().empty()) {
      out << v;
  }

  return out.str();
}

int main() {

  auto input = 9;
  auto output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 3^2

  input = 2;
  output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 2

  input = 4;
  output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 2^2

  input = 240;
  output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 2^4 x 3 x 5
  
  input = 60;
  output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 2^2 x 3 x 5

  input = 8320;
  output = prime_factor(input);

  std::cout << "Prime factorization of " << input << " is " << output
            << std::endl;
  // expected output: 2^7 x 5 x 13
  return 0;
}
