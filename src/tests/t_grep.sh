#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
TEST_FILE="text.txt"
echo "" > log.txt

for var in -v -c -l -n -h -o
do
  for var2 in -v -c -l -n -h -o
  do
      for var3 in -v -c -l -n -h -o
      do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
          TEST1=" $var $var2 $var3 g text.txt"
          echo "$TEST1"
          ../grep/s21_grep $TEST1 > s21_grep.txt
          grep $TEST1 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

          TEST2="$var $var2 $var3 p text.txt"
          echo "$TEST2"
          ../grep/s21_grep $TEST2 > s21_grep.txt
          grep $TEST2 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST2" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

          TEST3="-e for -e ^int $var $var2 $var3 text.txt"
          echo "$TEST3"
          ../grep/s21_grep $TEST3 > s21_grep.txt
          grep $TEST3 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST3" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

          TEST4="-e for -e ^int $var $var2 $var3 text.txt"
          echo "$TEST4"
          ../grep/s21_grep $TEST4 > s21_grep.txt
          grep $TEST4 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST4" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

          TEST5="-e regex -e ^print $var $var2 $var3 -f file1.txt text.txt"
          echo "$TEST5"
          ../grep/s21_grep $TEST5 > s21_grep.txt
          grep $TEST5 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST5" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

          TEST6="-e while -e void $var $var2 $var3 -f file2.txt text.txt"
          echo "$TEST6"
          ../grep/s21_grep $TEST6 > s21_grep.txt
          grep $TEST6 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST6" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

        fi
      done
  done
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"