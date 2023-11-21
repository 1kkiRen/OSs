# 1. Check the inode of the program ex1. Determine the number of
# blocks that the file has and the size of each block. Print also the
# total size in bytes and permissions of the file.
ls -i ex1
stat ex1

# 2. Copy the program ex1 to ex2 and check the number of links for
# the file ex2. Check if they have same i-node numbers. Justify your
# answer.
cp ex1 ex2
ls -i ex2
stat ex1
# ex1 has 1 hardlink

stat ex2
# They have different inode numbers, because they are different files, but with same filling.

# 3. Identify the files who have 3 links in the path /etc by running the
# following command line:
stat -c "%h - %n" /etc/* | grep ^3

