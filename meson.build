project('cppversus', 'cpp', version : '1.0', default_options : [ 'cpp_std=c++20' ])

# Variables tracking sources and libraries
library_path = []
project_sources = []
project_header_files = []
project_test_sources = []
project_benchmark_sources = []

includes = [
    include_directories('include')
]

cppversus_lib = library('cppversus', 
    [
        'src/cppversus.cpp'
    ], 
    include_directories : includes,
    dependencies : [
        dependency('spdlog')
    ],
    link_with : [

    ]
)


if get_option('enable-tests')
    gtest = subproject('gtest')
    subdir('tests')
endif
if get_option('enable-benchmarks')
    benchmark = subproject('google-benchmark')

    subdir('benchmarks')
endif

subdir('docs')