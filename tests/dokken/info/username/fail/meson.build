info_by_username_fail_test_sources = [
    'username_fail.cpp'
]

info_by_username_fail_test = executable('info_by_username_fail_test', 
    [ info_by_username_fail_test_sources, test_main ], 
    include_directories : includes,
    dependencies : [
        test_deps
    ], 
    link_with : [
        cppversus_lib, test_dep_libs
    ]
)

test('Info by Username Fail Test', info_by_username_fail_test)
project_benchmark_sources += files(info_by_username_fail_test_sources)