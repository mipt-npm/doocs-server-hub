project('hub',
    [ 'cpp' ],
    default_options : [
        'warning_level=3',
        'cpp_std=c++17',
    ],
)

src = [
    'src/main.cc',
    'src/EqDevice.cc',
    'src/D_prop_wrapper.cc',
]

# Things we install besides the binary
datafiles = [
    'RPC_LIBNO',
    meson.project_name() + '_CONF.TEMPLATE',
]

doocs_json_proj = subproject('doocs-json')
doocs_zmq_dep = doocs_json_proj.get_variable('doocs_zmq_dep')

# Libraries we depend on (usually found via pkg-config)
dep = [
    dependency('doocs-serverlib'),
    dependency('doocs-doocsapi'),
    dependency('doocs-doocsdapi'),
    doocs_zmq_dep
]

incl = include_directories('include')

install_data(datafiles,
    install_dir : get_option('datadir'),
)

# Server executable
executable(meson.project_name() + '_server',
    src,
    include_directories : incl,
    dependencies : dep,
    install : true,
)
