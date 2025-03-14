#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}
@test "Checking if dsh runs" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Execute ls command" {
    run ./dsh <<EOF
ls
exit
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ dshlib\.c ]]
}

@test "Change directory using cd" {
    run ./dsh <<EOF
cd ..
pwd
exit
EOF
    echo "PWD Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" == *"/CS283"* ]]
}

@test "Execute pipeline ls | grep .c" {
    run ./dsh <<EOF
ls | grep ".c"
exit
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ dshlib\.c ]]
}

@test "Execute multi-stage pipeline" {
    run ./dsh <<EOF
ls | grep ".c" | wc -l
exit
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ [0-9]+ ]]
}

@test "Execute output redirection" {
    run ./dsh <<EOF
echo "hello, world" > testfile.txt
ls
cat testfile.txt
exit
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ hello,\ world ]]
    rm -f testfile.txt
}

@test "Execute append redirection" {
    run ./dsh <<EOF
echo "hello, world" > testfile.txt
echo "this is line 2" >> testfile.txt
ls
cat testfile.txt
exit
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ hello,\ world ]]
    [[ "$output" =~ this\ is\ line\ 2 ]]
    rm -f testfile.txt
}

@test "Handle empty input" {
    run ./dsh <<EOF

exit
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
}

@test "Input redirection test" {
    echo "input line" > input.txt
    run ./dsh <<EOF
cat < input.txt
exit
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ input\ line ]]
    rm -f input.txt
}

@test "Built-in command cd without arguments" {
    run ./dsh <<EOF
cd
pwd
exit
EOF
    echo "PWD Output: $output"
    [ "$status" -eq 0 ]
}

@test "Complex command with multiple pipes" {
    run ./dsh <<EOF
echo -e 'apple\nbanana\ncarrot' | grep a | grep p | sort
exit
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ apple ]]
}

@test "Input redirection with <" {
    echo "input line" > input.txt
    run ./dsh <<EOF
cat < input.txt
exit
EOF
    echo "Full Output: '$output'"
    [ "$status" -eq 0 ]
    grep -q "input line" input.txt
    rm -f input.txt
}

@test "Output redirection with >" {
    run ./dsh <<EOF
echo "test output" > out.txt
exit
EOF
    cat out.txt > file_output.txt
    echo "Full Output: '$output'"
    [ "$status" -eq 0 ]
    grep -q "test output" file_output.txt
    rm -f out.txt file_output.txt
}

@test "Append redirection with >>" {
    run ./dsh <<EOF
echo "first line" > append.txt
echo "second line" >> append.txt
exit
EOF
    cat append.txt > file_output.txt
    echo "Full Output: '$output'"
    [ "$status" -eq 0 ]
    grep -q "first line" file_output.txt
    grep -q "second line" file_output.txt
    rm -f append.txt file_output.txt
}

@test "Background execution test" {
    run ./dsh <<EOF
sleep 1 &
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Exit command works" {
    run ./dsh <<EOF
exit
EOF
    [ "$status" -eq 0 ]
}

@test "Check multiple commands execution" {
    run ./dsh <<EOF
echo "first"
echo "second"
exit
EOF
    echo "Output: $output"
    [ "$status" -eq 0 ]
    [[ "$output" =~ first ]]
    [[ "$output" =~ second ]]
}

@test "Multiple redirections and pipes" {
    echo "This is a test" > input.txt
    run ./dsh <<EOF
cat < input.txt | grep "test" | wc -l > output.txt
exit
EOF
    cat output.txt > final_output.txt
    echo "Captured Output: '$output'"
    [ "$status" -eq 0 ]
    grep -q "1" final_output.txt
    rm -f input.txt output.txt final_output.txt
}