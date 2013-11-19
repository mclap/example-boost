set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)
find_package(Boost 1.53.0 REQUIRED COMPONENTS system thread regex)
if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
endif(Boost_FOUND)

set(USED_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_REGEX_LIBRARY})

add_executable(example-smtpd
	smtpd_request_handler.h
	smtpd_request_parser.cpp smtpd_request_parser.h
	smtpd_request.h
	smtpd_server.cpp smtpd_server.h
	smtpd_session.cpp smtpd_session.h
	smtpd_main.cpp
	smtpd_reply.cpp smtpd_reply.h
	)

target_link_libraries(example-smtpd ${USED_LIBS})
