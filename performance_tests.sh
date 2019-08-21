#!/bin/bash
ulimit -s unlimited
build=0
generate_tests=0
tests=1
while getopts ":bg:" opt; do
  case $opt in
    b)
      echo "Building project triggered." >&2
      build=1
      ;;
    g)
      echo "Generating tests triggered." >&2
      tests=$OPTARG
      generate_tests=1
      ;;
    \? ) echo "Unknown option: -$OPTARG" >&2; exit 1;;
    :  ) echo "Missing option argument for -$OPTARG" >&2;;
    *  ) echo "Unimplemented option: -$OPTARG" >&2; exit 1;;
  esac
done

shift $(($OPTIND - 1))

if [ $build -eq 1 ]
then
./build.sh
fi

if [ $generate_tests -eq 1 ]
then
printf "$tests" | ./tests/gen_performance.py > tests/in.txt
fi

build/src/Main < tests/in.txt