gcc publisher.c -o publisher
gcc subscriber.c -o subscriber

n=$1
i=1

if [[ $n -lt 1 ]] || [[ $n -gt 3 ]]
then
    echo "n must be between 1 and 3"
    exit 1
fi

while [[ $i -le $n ]] ; do
  xterm -e ./subscriber $i &
  i=$((i+1))
done

xterm -e ./publisher $n

