#pragma once

#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

// STL INCLUDES
#include <algorithm>
#include <cassert>
#include <chrono>
#include <stdio.h>
#include <io.h>
#include <iomanip>
#include <fcntl.h>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <bitset>
#include <optional>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include <filesystem>

// OpenGL libraries
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// LOGGING LIBRARY
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"


constexpr int GLlogSize{ 512 };
