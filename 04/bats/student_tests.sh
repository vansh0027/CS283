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

#!/usr/bin/env bats


@test "Built-in command: exit" {
    run ./dsh <<EOF                
exit
EOF

    [ "$status" -eq 0 ]
    [[ "$output" == *"cmd loop returned 0"* ]]
}


@test "External command: Check pwd" {
    run ./dsh <<EOF                
pwd
EOF

    current_dir=$(pwd)
    [[ "$output" == *"$current_dir"* ]]
    [ "$status" -eq 0 ]
}

@test "Built-in command: Check cd to valid directory" {
    mkdir -p /tmp/dsh_test_dir
    run ./dsh <<EOF
cd /tmp/dsh_test_dir
pwd
EOF

    [[ "$output" == *"/tmp/dsh_test_dir"* ]]
    [ "$status" -eq 0 ]
}




@test "Built-in command: Check cd to invalid directory" {
    run ./dsh <<EOF
cd /no/such/dir
EOF

    [[ "$output" == *"cd failed"* ]]
    [ "$status" -eq 0 ]
}


@test "External command with arguments: Check echo" {
    run ./dsh <<EOF
echo hello world
EOF

    [[ "$output" == *"hello world"* ]]
    [ "$status" -eq 0 ]
}


@test "Check handling empty input" {
    run ./dsh <<EOF
                
EOF

    [[ "$output" == *"warning: no commands provided"* ]]
    [ "$status" -eq 0 ]
}


@test "Check handling multiple spaces and quotes" {
    run ./dsh <<EOF
echo    "multiple   spaces"  test
EOF

    [[ "$output" == *"multiple   spaces test"* ]]
    [ "$status" -eq 0 ]
}


@test "Whitespace-only input" {
    run ./dsh <<EOF
            
EOF
    [[ "$output" == *"warning: no commands provided"* ]]
    [ "$status" -eq 0 ]
}

@test "External command: special characters in arguments" {
    run ./dsh <<EOF
echo "special!@#$%^&*()chars"
EOF
    [[ "$output" == *"special!@#$%^&*()chars"* ]]
    [ "$status" -eq 0 ]
}

@test "External command: quoted empty argument" {
    run ./dsh <<EOF
echo ""
EOF
    [[ "$output" == *""* ]]
    [ "$status" -eq 0 ]
}


@test "External command: Check invalid command" {
    run ./dsh <<EOF
nonexistent_command
EOF

    [[ "$output" == *"execute command error"* ]]
    [ "$status" -eq 0 ]
}


@test "Check Multiple commands in sequence" {
    run ./dsh <<EOF
echo first
pwd
EOF

    [[ "$output" == *"first"* ]]
    [[ "$output" == *"$(pwd)"* ]]
    [ "$status" -eq 0 ]
}
