#!/bin/bash
ulimit -s unlimited
build=0
generate_tests=0
profile=0
tests=1
while getopts ":bpg:" opt; do
  case $opt in
    b)
      echo "Building project triggered." >&2
      build=1
      ;;
    p)
      echo "Profiling triggered." >&2
      profile=1
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
printf "$tests" | ./gen.py > in.txt
printf "$tests" | ./gen_correctness_tests.py > in_correctness.txt
fi

cmake-build-debug/Subset_Sum_Solver < in.txt
#echo "Correctness tests:"
#cmake-build-debug/Subset_Sum_Solver < in_correctness.txt

if [ $profile -eq 1 ]
then
gprof cmake-build-debug/Subset_Sum_Solver < in.txt > gprof.log
fi
