#pragma once

#include "Core.h"

template<typename T>
size_t StdFunctionAddress(std::function<void(T &)> f) {
	typedef void (fnType)(T &);
	fnType **fnPointer = f.template target<fnType *>();
	return (size_t)*fnPointer;
}

template<typename T>
using Callback = std::function<void(const T &)>;

template<typename T>
using CallbackRef = Ref<Callback<T>>;

template<typename T>
constexpr CallbackRef<T> MakeCallbackRef(Callback<T> arg) { return MakeRef<Callback<T>>(arg); }

template <typename T>
class Event
{
public:
	Event() = default;
	virtual ~Event() = default;

public:
	void Subscribe(Ref<Callback<T>> callback)
	{
		subscribers.insert(callback);
	}

	void Unsubscribe(Ref<Callback<T>> callback)
	{
		subscribers.erase(callback);
	}

	void Invoke(const T &argument) const
	{
		for (const auto &callback : subscribers)
			(*callback.get())(argument);
	}

private:
	std::unordered_set<Ref<Callback<T>>> subscribers;
};
