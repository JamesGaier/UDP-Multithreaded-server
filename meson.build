project(
    'UDP Multithreaded Server',
    'cpp',
    version: '1.0',
)

include_dir = include_directories('src/')

udp_sock_src = [
    'src/sock.cpp',
    'src/thread_pool.cpp',
    'src/udp_server.cpp',
]

udp_socket_main_src = [

    'src/socket_example.cpp',
]

udp_serv_main_src = [
    'src/udp_server_example.cpp'
]

udp_cli_main_src = [
    'src/socket_client_example.cpp'
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
    'udp-socket',
    udp_socket_main_src,
    udp_sock_src,
    dependencies: [udp_server_dep],
    cpp_args: ['-std=c++20', '-Wall', '-Wextra', '-Wpedantic', '-Werror', '-O3']
)

executable(
    'udp-client',
    udp_cli_main_src,
    udp_sock_src,
    dependencies: [udp_server_dep],
    cpp_args: ['-std=c++20', '-Wall', '-Wextra', '-Wpedantic', '-Werror', '-O3']
)

executable(
    'udp-server',
    udp_serv_main_src,
    udp_sock_src,
    dependencies: [udp_server_dep],
    cpp_args: ['-std=c++20', '-Wall', '-Wextra', '-Wpedantic', '-Werror', '-O3']
)