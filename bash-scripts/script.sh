#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Мало аргументов"
elif [ `ls| grep .${1}$| wc -w` -ne 0 ]
    then
        for x in ./*"$1"
        do
            if [ -f $x ]
            then
                mv $x ${x%.*}.$2
            fi
        done
    else
        echo "Нет файлов с таким расширением в текущей директории"
fi
