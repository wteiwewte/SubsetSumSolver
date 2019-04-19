#!/bin/bash
generate_tests=0
loops=1
while getopts ":g:" opt; do
  case $opt in
    g)
      echo "generating tests was triggered. Number of tests set to: $OPTARG" >&2
      loops=$OPTARG
      generate_tests=1
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    :)
      echo "generating tests was triggered. Number of tests set to default: 1" >&2
      loops=1
      generate_tests=1
      ;;
  esac
done

/home/jan/Desktop/clion-2018.3.2/bin/cmake/linux/bin/cmake --build /home/jan/Desktop/Subset_Sum_Solver/cmake-build-debug --target Subset_Sum_Solver -- -j 2

while [ $loops -gt 0 ] ; do
	if [ $generate_tests -eq 1 ]
	then
		./gen.py > in.txt
	fi
	cmake-build-debug/Subset_Sum_Solver < in.txt
	let loops-=1
done
