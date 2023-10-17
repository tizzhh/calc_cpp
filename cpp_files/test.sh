#!/bin/bash

# Define the input strings
input_strings=(
  "(2/(3+2)*5-7.5^3)mod3*sqrt(5)" # -6.4287
  "2+3*5"
  "-5*(-(-5-sin(-5^(-1))))" # -24.00665
  "-1^2"
  "2.5+3-5.5"
  "5mod2"
  "sin(sin(1))*4" #2.9824
  "+6-7*(+4)" # -22
  "2^2^3" # 256
)

errors=(
  "4*sin(sin(~))"
  "(4*)"
  "*4"
  "(.)/(.)"
  "*()"
  "2.5.6*3"
  "4*"
  "4*)"
  "4*!"
  "!*4"
  "*)"
  "))(("
  "(4*"
  "4*+2"
  "4--2"
  "sin(1)4"
)

# Run the main program for each input string
echo -e "\nTESTS\n"
for input_str in "${input_strings[@]}";
do
  output_str=$(./a.out "$input_str")
  echo "Input: $input_str"
  echo "Output: $output_str"
  echo -e "\n"
done

echo "------------------------"

expected_error_count=${#errors[@]}
returned_error_count=0

echo -e "\nERRORS\n"
for input_str in "${errors[@]}";
do
  output_str=$(./a.out "$input_str")
  echo "Input: $input_str"
  if [[ "$output_str" == "ERROR, please provide correct input" ]]; then
    echo -e "\e[32mOutput: $output_str\e[0m"
    returned_error_count=$((returned_error_count + 1))
  else
    echo -e "\e[31mOutput: $output_str\e[0m"
  fi
  echo -e "\n"
done

echo -e "\nExpected error lines: $expected_error_count"
echo -e "Returned error lines: $returned_error_count"
