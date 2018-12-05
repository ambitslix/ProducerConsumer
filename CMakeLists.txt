cmake_minimum_required (VERSION 2.8)

project (ProducerConsumer)

add_executable (ProducerConsumer ProducerConsumer.cpp)

target_link_libraries (ProducerConsumer pthread)

install (TARGETS ProducerConsumer DESTINATION bin)

