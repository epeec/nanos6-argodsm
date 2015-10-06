ACLOCAL_AMFLAGS = -I ../../m4
AM_CXXFLAGS = -I$(srcdir)/../../src -I$(srcdir)/..

#
# Tests
#

alltests = \
	noop.debug.test noop.test \
	single-nest.debug.test single-nest.test \
	fibonacci.debug.test fibonacci.test \
	cpu-activation.debug.test cpu-activation.test \
	critical.debug.test critical.test

check_PROGRAMS =
TESTS =
if TEST_WITH_MCC
check_PROGRAMS += $(alltests)
TESTS += $(alltests)
endif


shutdown_common_sources = \
	tests/infrastructure/ProgramLifecycle.cpp

test_common_debug_libs = ../../libnanos6-debug.la ../../libcheck-shutdown-debug.la
test_common_libs = ../../libnanos6.la ../../libcheck-shutdown.la
test_common_ldflags = -no-install

noop_debug_test_SOURCES = noop/noop.cpp
noop_debug_test_CXXFLAGS = $(DEBUG_CXXFLAGS) $(AM_CXXFLAGS)
noop_debug_test_LDADD = $(test_common_debug_libs)
noop_debug_test_LDFLAGS = $(test_common_ldflags)

noop_test_SOURCES = noop/noop.cpp
noop_test_CPPFLAGS = -DNDEBUG
noop_test_CXXFLAGS = $(OPT_CXXFLAGS) $(AM_CXXFLAGS)
noop_test_LDADD = $(test_common_libs)
noop_test_LDFLAGS = $(test_common_ldflags)

single_nest_debug_test_SOURCES = single-nest/single-nest.cpp
single_nest_debug_test_CXXFLAGS = $(DEBUG_CXXFLAGS) $(AM_CXXFLAGS)
single_nest_debug_test_LDADD = $(test_common_debug_libs)
single_nest_debug_test_LDFLAGS = $(test_common_ldflags)

single_nest_test_SOURCES = single-nest/single-nest.cpp
single_nest_test_CPPFLAGS = -DNDEBUG
single_nest_test_CXXFLAGS = $(OPT_CXXFLAGS) $(AM_CXXFLAGS)
single_nest_test_LDADD = $(test_common_libs)
single_nest_test_LDFLAGS = $(test_common_ldflags)

fibonacci_debug_test_SOURCES = fibonacci/fibonacci.cpp
fibonacci_debug_test_CXXFLAGS = $(DEBUG_CXXFLAGS) $(AM_CXXFLAGS)
fibonacci_debug_test_LDADD = $(test_common_debug_libs)
fibonacci_debug_test_LDFLAGS = $(test_common_ldflags)

fibonacci_test_SOURCES = fibonacci/fibonacci.cpp
fibonacci_test_CPPFLAGS = -DNDEBUG
fibonacci_test_CXXFLAGS = $(OPT_CXXFLAGS) $(AM_CXXFLAGS)
fibonacci_test_LDADD = $(test_common_libs)
fibonacci_test_LDFLAGS = $(test_common_ldflags)

cpu_activation_debug_test_SOURCES = cpu-activation/cpu-activation.cpp
cpu_activation_debug_test_CXXFLAGS = $(DEBUG_CXXFLAGS) $(AM_CXXFLAGS) -I$(srcdir)/../../src/instrument/null
cpu_activation_debug_test_LDADD = $(test_common_debug_libs) -lpthread
cpu_activation_debug_test_LDFLAGS = $(test_common_ldflags)

cpu_activation_test_SOURCES = cpu-activation/cpu-activation.cpp
cpu_activation_test_CPPFLAGS = -DNDEBUG
cpu_activation_test_CXXFLAGS = $(OPT_CXXFLAGS) $(AM_CXXFLAGS) -I$(srcdir)/../../src/instrument/null
cpu_activation_test_LDADD = $(test_common_libs) -lpthread
cpu_activation_test_LDFLAGS = $(test_common_ldflags)

critical_debug_test_SOURCES = critical/critical.cpp
critical_debug_test_CXXFLAGS = $(DEBUG_CXXFLAGS) $(AM_CXXFLAGS) -I$(srcdir)/../../src/instrument/null
critical_debug_test_LDADD = $(test_common_debug_libs) -lpthread
critical_debug_test_LDFLAGS = $(test_common_ldflags)

critical_test_SOURCES = critical/critical.cpp
critical_test_CPPFLAGS = -DNDEBUG
critical_test_CXXFLAGS = $(OPT_CXXFLAGS) $(AM_CXXFLAGS) -I$(srcdir)/../../src/instrument/null
critical_test_LDADD = $(test_common_libs) -lpthread
critical_test_LDFLAGS = $(test_common_ldflags)


TEST_LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/../../tests/infrastructure/tap-driver.sh



DISTCLEANFILES = $(LOCAL_MCC_CONFIG)


libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status libtool

