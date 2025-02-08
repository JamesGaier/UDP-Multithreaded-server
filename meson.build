project(
    'UDP Multithreaded Server',
    'cpp',
    version: '1.0',
)

include_dir = include_directories('src/')

udp_sock_src = [
    'src/sock.cpp'
]

udp_serv_main_src = [
    'src/main.cpp',
    'src/thread_pool.cpp',
]

udp_cli_main_src = [
    'src/main_client.cpp'
]

udp_server_lib = library(
    'udp-server',
    include_directories: include_dir, 
)

udp_server_dep = declare_dependency(
    include_directories: include_dir,
    link_with: udp_server_lib
)

executable(
    'multithreaded-server',
    udp_serv_main_src,
    udp_sock_src,
    dependencies: [udp_server_dep],
    cpp_args: ['-std=c++17', '-Wall', '-Wextra', '-Wpedantic', '-Werror', '-O3']
)

executable(
    'multithreaded-client',
    udp_cli_main_src,
    udp_sock_src,
    dependencies: [udp_server_dep],
    cpp_args: ['-std=c++17', '-Wall', '-Wextra', '-Wpedantic', '-Werror', '-O3']
)