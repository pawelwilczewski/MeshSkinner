#pragma once

// single arugment callback helpers
template<typename T> using Callback = std::function<void(const T &)>;
template<typename T> using CallbackRef = Ref<Callback<T>>;
template<typename T> constexpr CallbackRef<T> MakeCallbackRef(Callback<T> arg) { return MakeRef<Callback<T>>(arg); }

// no argument callback helpers
using CallbackNoArg = std::function<void()>;
using CallbackNoArgRef = Ref<CallbackNoArg>;
CallbackNoArgRef MakeCallbackNoArgRef(CallbackNoArg arg);

/// <summary>
/// Events with a single argument.
/// </summary>
/// <typeparam name="T">Type of the argument passed to the callback.</typeparam>
template <typename T>
class Event
{
public:
	Event() = default;
	virtual ~Event() = default;

public:
	void Subscribe(const CallbackRef<T> &callback)
	{
		subscribers.insert(callback);
	}

	void Unsubscribe(const CallbackRef<T> &callback)
	{
		subscribers.erase(callback);
	}

	void Invoke(const T &argument) const
	{
		for (const auto &callback : subscribers)
			(*callback.get())(argument);
	}

private:
	std::unordered_set<CallbackRef<T>> subscribers;
};

/// <summary>
/// Events with no arguments should be initialized as Event<void> OR (see below) EventNoArg
/// </summary>
template <>
class Event<void>
{
public:
	Event() = default;
	virtual ~Event() = default;

public:
	void Subscribe(const CallbackNoArgRef &callback)
	{
		subscribers.insert(callback);
	}

	void Unsubscribe(const CallbackNoArgRef &callback)
	{
		subscribers.erase(callback);
	}

	void Invoke() const
	{
		for (const auto &callback : subscribers)
			(*callback.get())();
	}

private:
	std::unordered_set<CallbackNoArgRef> subscribers;
};

using EventNoArg = Event<void>;
