cmake_minimum_required(VERSION 3.5)

project(ipfilter)

#==============================
# work with version determination...
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
#
# Make a version file containing the current version from git.
#
include(GetGitRevisionDescription)
git_describe(VERSION --tags --dirty=-dirty)

#parse the version information into pieces.
string(REGEX REPLACE "^v([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string(REGEX REPLACE "^v[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${VERSION}")
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.$ENV{TRAVIS_BUILD_NUMBER}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/version.cpp.in
                ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)
set(version_file "${CMAKE_CURRENT_BINARY_DIR}/version.cpp")
#================================
# end version part
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(ipfilter ${CMAKE_CURRENT_SOURCE_DIR}/ipFilter.cpp )
add_executable(constexpr ${CMAKE_CURRENT_SOURCE_DIR}/constexpr.cpp )

target_link_libraries(constexpr ${Boost_LIBRARIES})

set_target_properties(ipfilter constexpr PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)



install(TARGETS ipfilter DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION ${VERSION_SHORT})
set(CPACK_PACKAGE_CONTACT xxx@xxx.xx)

include(CPack)
