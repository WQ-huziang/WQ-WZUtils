#!/bin/bash

pro_list = ()

# read the project root path
read -t 30 -p "Please input your project path (not inclue /) : " propath
echo "Your project path is $propath"

function initPro() {
  mkdir -p $propath/inc/WZUtils
  mkdir -p $propath/lib
}

function copyDir() {
  echo "Copying $1"

  # copy project
  cp -rf ./$1 $propath/inc/WZUtils/$1
  cd $propath/inc/WZUtils/$1

  # get lib and inc
  if [ -d "inc" && -d "lib" ]
  then
    mv lib/* $propath/lib/
    rm -rf !(inc)
    mv inc/* ./
    rm -rf inc
  fi

  cd -
}

function getName() {
  j = 0
  for file in `ls ./`
  do
    if [ -d ./$file ]
    then
      pro_list[j] = $file
      j = `expr $j + 1`
      copyDir $file
    fi
  done   
}

initPro
getName
