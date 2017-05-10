#!./roundup

function create_task_file {
    local job_file=`mktemp`
    echo "#!/bin/bash" > $job_file
    echo "" >> $job_file
    echo "#PBS -l walltime=00:$1:00,nodes=1:ppn=$2" >> $job_file
    echo "#PBS -N $3" >> $job_file
    echo "#PBS -q batch" >> $job_file
    echo "$job_file"
}

function run_task {
    local task_id=`qsub $1`
    local task_id=`basename $task_id .localhost`
    local task_name=`cat $1 | grep "#PBS -N" | awk '{print $3}'`

    local task_out="$task_name.o$task_id"
    local task_err="$task_name.e$task_id"

    while [ ! -f $task_out ]
    do
        inotifywait -qqt 2 -e create -e moved_to "$(dirname $task_out)"
    done
    echo $task_out
}

function run_test {
    local task_name="integral_test"

    local task_file=`create_task_file "01" "$1" $task_name`
    echo "" >> $task_file
    echo "cd \$PBS_O_WORKDIR" >> $task_file
    echo "module add openmpi-x86_64" >> $task_file
    echo "mpirun --hostfile \$PBS_NODEFILE ./tint $2" >> $task_file
    
    local out=`run_task $task_file`
    cat $out
    rm -f integral_test.o*
    rm -f integral_test.e*
}


module add openmpi-x86_64
gcc   -O2 -o int int.c -lm
mpicc -O2 -o tint tint.c -lm

describe "Test integral MPI realization"


it_check_N10P1() {
    test `./int 10` "=" `run_test 1 10`
}

it_check_N10P2() {
    test `./int 10` "=" `run_test 2 10`
}

it_check_N10P3() {
    test `./int 10` "=" `run_test 3 10`
}

it_check_N10P7() {
    test `./int 10` "=" `run_test 7 10`
}

it_check_N10P9() {
    test `./int 10` "=" `run_test 9 10`
}

it_check_N100P2() {
    test `./int 100` "=" `run_test 2 100`
}

it_check_N100P7() {
    test `./int 100` "=" `run_test 7 100`
}
