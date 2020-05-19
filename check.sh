echo "Going to run 100 test"
for((i = 1; i<100 ; ++i)); do
    echo $i
    ./checker.o $i > int
    # ./a < int > out1
    # ./brute < int > out2
    # diff -w out1 out2 || break
    ./parallel.o < int
    ./serial.o < int
done
