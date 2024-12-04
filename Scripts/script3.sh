#! /bin/bash
# Написать подробные комментарии

mkdir "Directory"
cd "Directory"

for ((i = 1; i <= $1; i++))
do
    # Создаём папку
    mkdir "Folder$i"
    for ((j = 1; j <= $2; j++))
    do
        # Создаем файл с именем "File j" внутри папки "Folder i"
	    touch "Folder$i/File$j"
	    for ((k = 1; k <= $3; k++))
	    do
	        echo You have come to the File number $j of the Folder number $i >> "Folder$i/File$j"
        done
    done
done


i=1

# Выполнится 4 раза
while [ $i -lt 5 ]
do
    # Переходим в директорию
    cd Folder$i
    # Перезаписываем файл содержимым в обратном порядке
    echo $(rev File$i) > File$i
    # Инкрементируем i
    i=$(( $i + 1 ))
done
