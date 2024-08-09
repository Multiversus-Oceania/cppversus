api_init_test_sources = [
    'api_init_test.cpp'
]

api_init_test = executable('api_init_test', 
    [ api_init_test_sources, test_main ], 
    include_directories : includes,
    dependencies : [
        test_deps
    ], 
    link_with : [
        cppversus_lib, test_dep_libs
    ]
)

test('API Init Test', api_init_test)
project_benchmark_sources += files(api_init_test_sources)