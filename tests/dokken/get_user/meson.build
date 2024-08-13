get_user_test_sources = [
    'get_user_test.cpp'
]

get_user_test = executable('get_user_test', 
    [ get_user_test_sources, test_main ], 
    include_directories : includes,
    dependencies : [
        test_deps
    ], 
    link_with : [
        cppversus_lib, test_dep_libs
    ]
)

test('Get User API Test', get_user_test)
project_benchmark_sources += files(get_user_test_sources)