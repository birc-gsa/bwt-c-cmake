add_library(bwt bwt.c sa.c)

add_executable(bwt_test bwt_test.c)
target_link_libraries(bwt_test bwt)
add_test(bwt_test bwt_test)

add_executable(sa_test sa_test.c)
target_link_libraries(sa_test bwt)
add_test(sa_test sa_test)
