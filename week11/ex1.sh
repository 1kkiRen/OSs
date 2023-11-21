# Create a 50MB file
dd if=/dev/zero of=lofs.img bs=1M count=50

# Set up a loop device for the image file
sudo losetup /dev/loop0 lofs.img

# Create an ext4 filesystem on the loop device
sudo mkfs.ext4 /dev/loop0

# Create a directory to mount the loop device
sudo mkdir lofsdisk

# Mount the loop device on the created directory
sudo mount /dev/loop0 lofsdisk

# Create two files with content in the mounted filesystem
sudo echo "Dmitrii" > lofsdisk/file1
sudo echo "Kuzmin" > lofsdisk/file2

# Define a function to get the dynamic libraries required by a given executable
get_libs() {
    # Use ldd to get the dynamic libraries required by the executable
    ldd $1 | grep '/[^ ]*' -o
}

# Loop through a list of commands (bash, cat, echo, ls)
for cmd in bash cat echo ls
do
    # Get the dynamic libraries required by the command
    libs=$(get_libs /bin/$cmd)

    # Copy the command and its dynamic libraries to the mounted filesystem
    sudo cp --parents /bin/$cmd ./lofsdisk/
    for lib in $libs
    do
        sudo cp --parents $lib ./lofsdisk/
    done
done

# Compile a C program (ex1.c) and copy the executable to the mounted filesystem
gcc ex1.c -o ex1
sudo cp ./ex1 ./lofsdisk/

# Use chroot to change the root directory to the mounted filesystem and execute a command
sudo chroot ./lofsdisk/ /bin/bash -c "/ex1" > ex1.txt

# Execute the compiled C program and append the output to ex1.txt
sudo ./ex1 >> ex1.txt
