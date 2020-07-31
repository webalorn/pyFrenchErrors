mkdir -p local_tests
python3 $1 2> local_tests/py_errors > local_tests/pyOut
./pyfe $1 local_tests/py_errors local_tests/out.json python fr