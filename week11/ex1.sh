dd if=/dev/zero of=lofs.img bs=1M count=50

sudo losetup /dev/loop0 lofs.img

sudo mkfs.ext4 /dev/loop0

sudo mkdir lofsdisk
sudo mount /dev/loop0 lofsdisk

sudo echo "Dmitrii" > lofsdisk/file1
sudo echo "Kuzmin" > lofsdisk/file2


get_libs() {
    ldd $1 | grep '/[^ ]*' -o
}

for cmd in bash cat echo ls
do
    libs=$(get_libs /bin/$cmd)
    sudo cp --parents /bin/$cmd ./lofsdisk/
    for lib in $libs
    do
        sudo cp --parents $lib ./lofsdisk/
    done
done


gcc ex1.c -o ex1
sudo cp ./ex1 ./lofsdisk/

sudo chroot ./lofsdisk/ /bin/bash -c "/ex1" > ex1.txt

sudo echo "---------------------" >> ex1.txt
sudo ./ex1 >> ex1.txt
