#pragma once

// WINDOWS INCLUDES
#if defined(_WINDOWS)
#include "Windows_Includes.h"
#include <shellapi.h>

#if defined(CreateWindow)
#undef CreateWindow
#endif
#endif

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
#include <sstream>
#include <queue>
#include <bitset>
#include <optional>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

// LOGGING LIBRARY
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"