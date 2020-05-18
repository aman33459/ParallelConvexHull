for((i = 1; ; ++i)); do
    echo $i
    ./a.out $i > int
    # ./a < int > out1
    # ./brute < int > out2
    # diff -w out1 out2 || break
    ./quickHull/quickhullpara < int
    ./serialHull/serialhull < int
done
