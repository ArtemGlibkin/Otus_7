cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(bayan VERSION ${PROJECT_VESRION})

configure_file(include/version.h.in include/version.h)

add_executable(bayan  src/main.cpp)
#add_executable(bayan _test tests/BulkTest.cpp)

set_target_properties(bayan   PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

#find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(Boost_USE_STATIC_LIBS        ON)  # only find static libs
set(Boost_USE_DEBUG_LIBS        OFF)  # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)

find_package(Boost COMPONENTS program_options coroutine regex REQUIRED)

target_include_directories(bayan 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(bayan  PRIVATE ${GTEST_INCLUDE_DIR} ${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(bayan PRIVATE Threads::Threads)
target_link_libraries(bayan PRIVATE ${Boost_LIBRARIES})

if(MSVC)
	target_compile_options(bayan  PRIVATE /MT)
	#target_compile_options(bayan_test PRIVATE /MT)
endif()

install(TARGETS bayan  RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT artem-glibkin@yandex.ru)

include(CPack)
enable_testing()

#gtest_add_tests(TARGET bulk_test
#				TEST_LIST SimpleTest BracketsTest EnclosedBracketsTest
#)