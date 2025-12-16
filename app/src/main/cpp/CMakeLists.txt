# 设置CMake最低版本  
cmake_minimum_required(VERSION 3.22.1)

# 设置项目名称  
project("ImGui")

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加ImGui库  
add_library(ImGui SHARED)

# 添加ImGui的源文件  
file(GLOB_RECURSE ImGui_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ImGuiInEGL/*.c*"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/ImGuiInput/*.c*"
)
target_sources(ImGui PRIVATE ${ImGui_SRC})

# 添加ImGui的包含目录  
target_include_directories(ImGui PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/ImGuiInEGL"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/ImGuiInput"
)

# 链接预构建的Sea2库  
#target_link_libraries(ImGui Sea2)

# 设置ImGui的编译选项  
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -O3 -s -fvisibility=hidden -Wno-unused-command-line-argument -Wno-error=format-security -fpermissive -w -Werror -s -std=c++17 -fno-rtti -fno-exceptions -fms-extensions -Wno-error=c++17-narrowing")

# 设置链接选项
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -llog -lEGL -lGLESv3 -landroid -lz")
