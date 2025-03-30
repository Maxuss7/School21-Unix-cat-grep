SHELL := /bin/bash
CC = gcc
CCFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = s21_grep
GREP_SRC = s21_grep.c

TEST_FILES := tests_files/test_grep1.txt tests_files/test_grep2.txt tests_files/test_1_5.txt tests_files/test_data.txt tests_files/test_grep.txt tests_files/test_file.txt
PATTERN_FILE := tests_files/pattrn_file.txt
PATTERNS := qwe QWE bn prince "The prince" "qwe qwe" "a b c"
SINGLE_FLAGS := i v c l n h s o
FLAG_COMBINATIONS := iv in io vn vo no ivn ivo ino vno ivno

all: clang s21_grep 

s21_grep: 
	$(CC) $(CCFLAGS) $(GREP_SRC) -o $(TARGET)
	  
clang:
	clang-format -style=Google -i s21_grep.c

clean:
	rm -rf $(TARGET)

test:
	@mkdir -p tests_files
	
	@echo "=== TEST 1: BASIC PATTERN TESTS ==="
	@for pattern in $(PATTERNS); do \
		echo "[TEST] Basic pattern: '$$pattern'"; \
		diff -u <(./s21_grep "$$pattern" $(TEST_FILES)) <(grep "$$pattern" $(TEST_FILES)) || exit 1; \
	done
	
	@echo "=== TEST 2: SINGLE FLAG TESTS ==="
	@for flag in $(SINGLE_FLAGS); do \
		for pattern in $(PATTERNS); do \
			echo "[TEST] Flag -$$flag with pattern: '$$pattern'"; \
			diff -u <(./s21_grep -$$flag "$$pattern" $(TEST_FILES)) <(grep -$$flag "$$pattern" $(TEST_FILES)) || exit 1; \
		done; \
	done
	
	@echo "=== TEST 3: FLAG COMBINATION TESTS ==="
	@for combo in $(FLAG_COMBINATIONS); do \
		flags=`echo $$combo | sed 's/./ -&/g'`; \
		for pattern in $(PATTERNS); do \
			echo "[TEST] Flags $$flags with pattern: '$$pattern'"; \
			diff -u <(./s21_grep $$flags "$$pattern" $(TEST_FILES)) <(grep $$flags "$$pattern" $(TEST_FILES)) || exit 1; \
		done; \
	done
	
	@echo "=== TEST 4: FILE PATTERN TEST (-f flag) ==="
	@echo "[TEST] Testing -f with pattern file: $(PATTERN_FILE)";
	@diff -u <(./s21_grep -f $(PATTERN_FILE) $(TEST_FILES)) <(grep -f $(PATTERN_FILE) $(TEST_FILES)) || exit 1;
	
	@echo "=== TEST 5: ERROR HANDLING TEST ==="
	@echo "[TEST] Testing error handling with non-existent file";
	@diff -u <(./s21_grep "pattern" no_such_file.txt 2>&1) <(grep "pattern" no_such_file.txt 2>&1) || exit 1;
	
	@echo "=== TEST 6: SPECIAL COMBINATION TESTS ==="
	@echo "[TEST] Testing combination -n -o with 'prince' in test_grep.txt";
	@diff -u <(./s21_grep -n -o "prince" tests_files/test_grep.txt) <(grep -n -o "prince" tests_files/test_grep.txt) || exit 1;
	
	@echo "[TEST] Testing combination -ivn with 'PRINCE' (case insensitive)";
	@diff -u <(./s21_grep -ivn "PRINCE" tests_files/test_grep.txt) <(grep -ivn "PRINCE" tests_files/test_grep.txt) || exit 1;
	
	@echo "=== ALL TESTS PASSED SUCCESSFULLY ==="