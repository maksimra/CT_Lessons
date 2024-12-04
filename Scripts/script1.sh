#!/bin/bash
# Написать подробные комментарии
# echo   "Кису любишь!"

# Создали переменную
KISA=Киса

# Вывели её на экран
echo $KISA

# Вывели на экран рандомное число
echo $RANDOM

# Создаём папку test и переходим в неё
mkdir test
cd  test

# Создали переменную
n=0

for ((i = 1; i <= $1; i++))
do
   mkdir level1.$i
	for ((j = 1; j <= $2; j++))
	do
		 mkdir level1.$i/level2.$j

		n=$((n+1))

		touch level1.$i/level2.$j/TextFile$n
		echo "This is file $n" > level1.$i/level2.$j/TextFile$n
		echo $n
	done
done

for f in `find . -type f`
do
  rev $f > $f
done

