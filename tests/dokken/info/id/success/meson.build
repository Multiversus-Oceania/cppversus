info_by_id_success_test_sources = [
    'id_success.cpp'
]

info_by_id_success_test = executable('info_by_id_success_test', 
    [ info_by_id_success_test_sources, test_main ], 
    include_directories : includes,
    dependencies : [
        test_deps
    ], 
    link_with : [
        cppversus_lib, test_dep_libs
    ]
)

test('Info by ID Success Test', info_by_id_success_test)
project_benchmark_sources += files(info_by_id_success_test_sources)