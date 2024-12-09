check_include_file_cxx(stdint.h HAVE_STDINT_H)
if(HAVE_STDINT_H)
    add_definitions(-DHAVE_STDINT_H)
endif()

set(examples_as_tests_sources)
if(${ENABLE_EXAMPLES})
    set(examples_as_tests_sources
        #test/dgr-examples-test-suite.cc
        )
endif()

build_lib(
    LIBNAME ddr
    SOURCE_FILES
        model/dgr-tags.cc
        model/dgr-header.cc
        model/neighbor-status-database.cc
        model/dgr-udp-application.cc
        model/dgr-tcp-application.cc
        model/dgr-sink.cc
        model/ipv4-dgr-routing-table-entry.cc
        model/ipv4-dgr-routing.cc
        model/dgr-router-interface.cc
        model/dgr-route-manager.cc
        model/dgr-route-manager-impl.cc
        model/dgr-candidate-queue.cc
        model/dgrv2-queue-disc.cc
        helper/ipv4-dgr-routing-helper.cc
        helper/dgr-application-helper.cc
        helper/dgr-tcp-application-helper.cc
        helper/dgr-sink-helper.cc
    HEADER_FILES
        model/dgr-tags.h
        model/dgr-header.h
        model/neighbor-status-database.h
        model/dgr-udp-application.h
        model/dgr-tcp-application.h
        model/dgr-sink.h
        model/ipv4-dgr-routing-table-entry.h
        model/ipv4-dgr-routing.h
        model/dgr-router-interface.h
        model/dgr-route-manager.h
        model/dgr-route-manager-impl.h
        model/dgr-candidate-queue.h
        model/dgrv2-queue-disc.h
        helper/ipv4-dgr-routing-helper.h
        helper/dgr-application-helper.h
        helper/dgr-tcp-application-helper.h
        helper/dgr-sink-helper.h
    LIBRARIES_TO_LINK ${libcore}
    TEST_SOURCES
        # test/dgr-udp-application-test.cc
        # test/dgr-routing-table-test.cc
        # test/dgr-virtual-queue-test.cc
        # test/dgr-geant-topo-test.cc
        # test/dgr-header-test.cc
        # test/dgr-tcp-application-test.cc
        # 'test/dsr-tcp-applciation.cc
        # 'test/dsr-routing-test-suite.cc
        ${examples_as_tests_sources}
)
