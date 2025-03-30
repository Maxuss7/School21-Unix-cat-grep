# 🚀 s21_cat & s21_grep - Unix Utilities

**My implementations of cat and grep tools in C**

![C](https://img.shields.io/badge/C-11-blue)
![Linux](https://img.shields.io/badge/Linux-compatible-brightgreen)

---

## 📋 s21_cat Features

| # | Option               | Description |
|---|----------------------|-------------|
| 1 | `-b` (`--number-nonblank`) | Numbers only non-empty lines |
| 2 | `-e` (implies `-v`)<br>`-E` (GNU style) | Shows `$` at line ends + displays non-printable chars<br>`-E` shows `$` without non-printables |
| 3 | `-n` (`--number`)    | Numbers all output lines |
| 4 | `-s` (`--squeeze-blank`) | Compresses multiple adjacent blank lines |
| 5 | `-t` (implies `-v`)<br>`-T` (GNU style) | Shows tabs as `^I` + non-printables<br>`-T` shows tabs without non-printables |


---



## 🔍 s21_grep Features

| # | Option | Description                          |
|---|--------|--------------------------------------|
| 1 | `-e`   | Specifies search pattern            |
| 2 | `-i`   | Case-insensitive search             |
| 3 | `-v`   | Inverts match results               |
| 4 | `-c`   | Shows match count only              |
| 5 | `-l`   | Shows matching files only           |
| 6 | `-n`   | Prefixes output with line numbers   |
| 7 | `-h`   | Hides filename prefixes             |
| 8 | `-s`   | Suppresses error messages           |
| 9 | `-f`   | Reads patterns from file            |
| 10| `-o`   | Shows only matched parts            |

---

## 🛠 Installation & Building

### Get the source code

```bash
git clone https://github.com/Maxuss7/School21-Unix-cat-grep.git
cd School21-Unix-cat-grep
```

### Build for s21_grep

```bash
cd cat
make
```

### Build s21_grep
```bash
cd grep
make
```

## Testing (Requires GNU coreutils (cat, grep) for comparison tests)

```bash
make test
```

## 💻 Usage Examples 

### Basic s21_cat usage
```bash
# 1. View file contents
./s21_cat filename.txt

# 2. Show line numbers (-n)
./s21_cat -n main.c

# 3. Display non-printable characters (-v) 
./s21_cat -v binary_file.bin


# 4. Number non-empty lines (-b)
./s21_cat -b /var/log/syslog
```

### Basic s21_grep usage
```bash
# 1. Basic search
./s21_grep "search_term" *.txt

# 2. Case-insensitive search (-i)
./s21_grep -i "error" /var/log/*.log

# 3. Count matches (-c)
./s21_grep -c "TODO" *.c

# 4. Show matching files only (-l)
./s21_grep -l "pattern" project_files/*
```

## 🧹 Cleaning Project Files (removes binaries)

```bash
make clean
```
