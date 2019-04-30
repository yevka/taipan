cmake_minimum_required(VERSION 3.10.1 FATAL_ERROR)

# Define variables
set(TEST_RUNNER test_runner)

set(TEST_SRC_LIST
        "test_fen.cpp"
        "test_make_unmakemove.cpp"
        "test_move_man.cpp"
        "test_mailbox2fen.cpp"
        "test_perft.cpp"

        "test_runner.cpp"
        )

# Configure test target
add_executable(${TEST_RUNNER} ${TEST_SRC_LIST} ${SRC_LIST})

# Run tests
add_test(
        NAME ${TEST_RUNNER}
        COMMAND ./${TEST_RUNNER} -d yes
)
