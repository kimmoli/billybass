foo1=$(echo $3 | cut -d "." -f 1)
foo2=$(echo $3 | cut -d "." -f 1,2)
test -h $1/$2.so || cp $1/$2.so.$3 $1/$2.so
test -h $1/$2.so.$foo1 || cp $1/$2.so.$3 $1/$2.so.$foo1
test -h $1/$2.so.$foo2 || cp $1/$2.so.$3 $1/$2.so.$foo2

