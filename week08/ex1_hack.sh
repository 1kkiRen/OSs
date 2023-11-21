gcc ex1.c -o ex1

./ex1 &

sleep 1

pid=$(cat /tmp/ex1.pid)

echo "Process ID: $pid"

addr=$(cat /proc/$pid/maps | grep heap | awk '{print $1}')

start=$(echo $addr | cut -d'-' -f1)
end=$(echo $addr | cut -d'-' -f2)

start="0x$start"
end="0x$end"

echo "Address: $addr"

pass=$(sudo xxd -s $start -l $end /proc/$pid/mem | grep "pass:")
pass=$(echo $pass | cut -d':' -f3 | cut -c1-8)

echo "Password: $pass"

sudo kill -9 $pid

rm ex1