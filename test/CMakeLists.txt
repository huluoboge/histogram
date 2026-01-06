


add_executable(test_histogram test_histogram.cpp)
target_link_libraries(test_histogram histogram GTest::GTest GTest::Main)
add_executable(test_merge test_merge.cpp)
target_link_libraries(test_merge histogram )
