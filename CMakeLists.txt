cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

project (DAQStreamer)

find_package(Protobuf)

# c++11 required
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# packetlib required
if (DEFINED ENV{PACKETLIB_ROOT_FOLDER} )
    message(STATUS "Using PacketLib from $ENV{PACKETLIB_ROOT_FOLDER}")
	include_directories($ENV{PACKETLIB_ROOT_FOLDER}/include)
	link_directories(${link_directories} $ENV{PACKETLIB_ROOT_FOLDER}/lib)
else()
    message(FATAL_ERROR "PacketLib not found, the PACKETLIB_ROOT_FOLDER env variable isn't set")
endif()

# ctatools required
if (DEFINED ENV{CTATOOLS_ROOT_FOLDER} )
    message(STATUS "Using CTATools from $ENV{CTATOOLS_ROOT_FOLDER}")
	include_directories($ENV{CTATOOLS_ROOT_FOLDER}/include)
	link_directories(${link_directories} $ENV{CTATOOLS_ROOT_FOLDER}/lib)
else()
    message(FATAL_ERROR "PacketLib not found, the CTATOOLS_ROOT_FOLDER env variable isn't set")
endif()

# ctatools source workaround (some files are not installed)
if (DEFINED ENV{CTATOOLS_SOURCE_FOLDER} )
    message(STATUS "Using CTATools from $ENV{CTATOOLS_SOURCE_FOLDER}")
	include_directories($ENV{CTATOOLS_SOURCE_FOLDER}/Core) # required for Streamer.hpp
	include_directories($ENV{CTATOOLS_SOURCE_FOLDER}) # required for Streamer.hpp
	file (GLOB DataModelProto "$ENV{CTATOOLS_SOURCE_FOLDER}/DataModel/*.proto") # generate L0 and message
	PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${DataModelProto})
else()
    message(FATAL_ERROR "PacketLib not found, the CTATOOLS_SOURCE_FOLDER env variable isn't set")
endif()

add_executable(DAQStreamer main.cpp CameraEventStreamer.cpp DebugWorker.cpp)
target_link_libraries(DAQStreamer CTAToolsCore packet protobuf)
