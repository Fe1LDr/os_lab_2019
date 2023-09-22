for i in {1..149};
do
    echo $(od -A n -t d -N 1 /dev/urandom) >> numbers.txt
done