add_library(bwt bwt.c)

add_executable(bwt_test bwt_test.c)
target_link_libraries(bwt_test bwt)
add_test(bwt_test bwt_test)
