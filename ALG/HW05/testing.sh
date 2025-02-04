#!/bin/bash

#printf "Building...\n"
#make

printf "Testing...\n"

for TESTNUMBER in "01" "02" "03" "04" "05" "06" "07" "08" "09" "10"; do
    printf "Test %s\n" $TESTNUMBER
    printf "Output: %s, Expected: %s\n" "$(./main < in/pub$TESTNUMBER.in)" "$(cat out/pub$TESTNUMBER.out)"
done

ARCHIVENAME="solution.zip"
zip -0 "$ARCHIVENAME" main.c
printf "Solution saved to %s\n" "$ARCHIVENAME"
