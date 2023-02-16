#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <functional>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <string>
#include <memory>

#include "Log.h"


template<typename T>
using Unique = std::unique_ptr<T>;
template<typename T, typename ...Args>
constexpr Unique<T> MakeUnique(Args &&...args) { return std::make_unique<T>(std::forward<Args>(args)...); }

template<typename T>
using Ref = std::shared_ptr<T>;
template<typename T, typename ...Args>
constexpr Ref<T> MakeRef(Args &&...args) { return std::make_shared<T>(std::forward<Args>(args)...); }

template<typename T>
using Weak = std::weak_ptr<T>;
