#! /bin/bash

mv tags/* . > /dev/null 2>&1
find . -name "*.c" -o -name "*.h" > files
split -n 8 -d files 2025_Will_Rich_

for file in $(ls 2025_Will_Rich_*); do
	ctags -L ${file} -f tag_${file} > /dev/null 2>&1 &
#cscope -b -q -k -f csdb_${file} -i ${file} > /dev/null 2>&1 &
done

count=1
while [[ $(jobs -p | wc -l) -gt 1 ]]; do
	a=$(jobs -p | wc -l | awk '{$1=$1;print}')
	b=$(jobs -l | grep 'tag' | wc -l | awk '{$1=$1;print}')
	c=$(jobs -l | grep 'cscope' | wc -l | awk '{$1=$1;print}')
	printf "${a} Jobs(tags:${b} cscope:${c}) Generating Tags For ${count}s  ...\n"
    sleep 1
    ((count++))
done
printf "\n\n"

rm -rf files 2025_Will_Rich_0*
