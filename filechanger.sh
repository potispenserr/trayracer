#! /bin/bash

copyright="$(cat bw.txt)"

pwd

files=$(ls *.{cc,h,cpp})

function addCopyrightAndChangeExtension(){
    for file in $files; do
        #checks if file already has copyright header and adds it if neccesary
        if grep -E "PVS-Studio" "$file"
        then
            echo "$file already has a copyright header"
        else
            echo "$copyright" > tmp.txt
            echo "$(cat $file)" >> tmp.txt
            mv tmp.txt "$file"
        fi

        #changes .cpp files to .cc
        if [ ${file: -4} == ".cpp" ]
        then
            echo "Changing extension on $file to .cc"
            echo "$(cat $file)" >> "$(echo ${file: 0:-4}".cc")"
            rm $file
        fi
    done
}

function floatToDoubleToFloat() {
    files=$(ls *.h *.cc) 

    for file in $files; do
        echo "Printing some occurences of float in $file"
        grep "float" $file | tail

        echo "Changing all them to doubles"
        sed -i "s/float/double/g" $file

        echo "Printing some occurences of double in $file"
        grep "double" $file | tail

        echo "Changing them back to floats again"
        sed -i "s/double/float/g" $file

        echo "Printing some occurences of float in $file again"
        grep "float" $file | tail
    done
}

addCopyrightAndChangeExtension



