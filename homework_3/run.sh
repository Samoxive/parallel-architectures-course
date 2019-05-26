echo "---SINGLE THREADED---"
for i in 1*.txt; do
	echo "---$i---";
	./main $i;
done

echo "---CUDA---"
for i in 1*.txt; do
	echo "---$i---";
	./par_main2 $i;
done

