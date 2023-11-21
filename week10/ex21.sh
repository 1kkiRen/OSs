# 1. Check the file types in the paths /dev and /etc.
file /dev/*
file /etc/*

# 2. Count the number of directories in the folder /etc by running the
# command line.
ls -l /etc | grep ^d | wc -l

# Before compilation
file ex2.c

# After compilation
gcc ex3.c -o ex3
file ex3.c
# The difference is that the file after compilation became C source, ASCII text, while the file before compilation was C source, Unicode text, UTF-8 text.

# After changing the text
gcc ex2.c -o ex2
file ex2.c
# The difference is that the file was C source, ASCII text, and after changing the text became C source, Unicode text, UTF-8 text.

