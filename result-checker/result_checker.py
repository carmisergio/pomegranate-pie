#!/bin/python

import sys

GIVEN_FILE_NAME = "pi_given.txt"
TO_CHECK_FILE_NAME = "pi_tocheck.txt"

def compare():
    given_file = open(GIVEN_FILE_NAME, "r").read()
    to_check = open(TO_CHECK_FILE_NAME, "r").read().replace("\n", "")
    
    n_right = 0;
    
    for index, char in enumerate(to_check):
        if char != given_file[index]:
            break
        n_right += 1
    
    if n_right == len(to_check):
        print(f"ALL OK! {n_right} digits correct")
    else:
        print(f"FAIL! {n_right} of {len(to_check)} digits correct")


    
if __name__ == "__main__":
    compare()
