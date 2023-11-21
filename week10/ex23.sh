# 2. Create a text file ex1.txt using gen.sh with arguments 10 and
# ex1.txt
./gen.sh 10 ex2.txt

# 3. Link ex1.txt to ex11.txt and ex12.txt
ln ex2.txt ex11.txt
ln ex2.txt ex12.txt

# 4. Compare the content of all files using diff or cat command. Is
# there a difference? Justify your answer.
diff ex2.txt ex11.txt
diff ex2.txt ex12.txt
# There is no difference, because all files have same content.

# 5. Check i-node numbers of all files and save the output to the file
# output.txt. Are they different i-node numbers? Justify your answer.
ls -i ex2.txt ex11.txt ex12.txt > output.txt
# They have same inodes because they are links.

# 6. Check the disk usage of ex1.txt file using du command.
du ex2.txt

