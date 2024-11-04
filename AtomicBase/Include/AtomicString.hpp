#include <mutex>
#include <shared_mutex>
#include <memory>
#include <string>
#include <algorithm>
#include <iostream>
#include <type_traits>
#include <format>

template <typename T>
class AtomicString
{

public:

    using string_type = std::basic_string<T>;
    using mutex_type = std::shared_mutex;

    AtomicString() = default;
    ~AtomicString() = default;

    AtomicString& operator=(const AtomicString& other) = delete;

    AtomicString(AtomicString&& other) noexcept
    {
        std::unique_lock<std::shared_mutex> lock(other.mutex);
        data = std::move(other.data);
    }

    template <typename U>
    AtomicString(const std::basic_string<U>& str)
    {
        data = Convert<U, T>(str);
    }

    template <typename U>
    AtomicString(const U* str)
    {
        data = Convert<U, T>(std::basic_string<U>(str));
    }

    template <typename T>
    class ThreadSafeIterator 
    {

    public:

        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        class ProxyReference 
        {

        public:

                return *this;
            }

            operator T() const 
            {
                return *it_;
            }

        private:

            AtomicString<T>& container_;
            typename AtomicString<T>::string_type::iterator it_;

        };

        ThreadSafeIterator(AtomicString<T>& container, typename AtomicString<T>::string_type::iterator it) : container_(container), it_(it), lock_(std::make_shared<std::unique_lock<std::shared_mutex>>(container_.mutex)) {}

        ThreadSafeIterator(const ThreadSafeIterator& other) : container_(other.container_), it_(other.it_), lock_(other.lock_) {}

        ThreadSafeIterator& operator=(const ThreadSafeIterator& other) 
        {
            if (this != &other) 
            {
                container_ = other.container_;
                it_ = other.it_;
                lock_ = other.lock_;
            }

            return *this;
        }

        ProxyReference operator*()
        {
            return ProxyReference(container_, it_);
        }

        T operator*() const 
        {
            return *it_;
        }

        ThreadSafeIterator& operator++() 
        {
            ++it_;
            return *this;
        }

        ThreadSafeIterator operator++(int) 
        {
            ThreadSafeIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        ThreadSafeIterator& operator--() 
        {
            --it_;
            return *this;
        }

        ThreadSafeIterator operator--(int)
        {
            ThreadSafeIterator tmp(*this);
            --(*this);

            return tmp;
        }

        bool operator==(const ThreadSafeIterator& other) const 
        {
            if (&container_ != &other.container_)
                return false;

            return it_ == other.it_;
        }

        bool operator!=(const ThreadSafeIterator& other) const 
        {
            return !(*this == other);
        }

    private:

        AtomicString<T>& container_;
        typename AtomicString<T>::string_type::iterator it_;
        std::shared_ptr<std::unique_lock<std::shared_mutex>> lock_;
    };

    AtomicString& operator=(AtomicString&& other) noexcept
    {
        if (this != &other)
        {
            std::scoped_lock lock(mutex, other.mutex);
            data = std::move(other.data);
        }

        return *this;
    }

    template <typename U>
    AtomicString& operator=(const AtomicString<U>& input)
    {
        std::unique_lock<std::shared_mutex> lock(mutex);

        data = Convert<U, T>(input);

        return *this;
    }

    template <typename U>
    AtomicString& operator=(const std::basic_string<U>& input)
    {
        std::unique_lock<std::shared_mutex> lock(mutex);

        data = Convert<U, T>(input);

        return *this;
    }

    template <typename U>
    AtomicString& operator=(const U* str)
    {
        std::unique_lock<std::shared_mutex> lock(mutex);

        data = Convert<U, T>(std::basic_string<U>(str));

        return *this;
    }

    template <typename U>
    bool operator==(const AtomicString<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data == Convert<U, T>(other);
    }

    template <typename U>
    bool operator==(const std::basic_string<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data == Convert<U, T>(other);
    }

    template <typename U>
    bool operator==(const U* other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data == Convert<U, T>(std::basic_string<U>(other));
    }

    template <typename U>
    bool operator!=(const AtomicString<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data != Convert<U, T>(other);
    }

    template <typename U>
    bool operator!=(const std::basic_string<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data != Convert<U, T>(other);
    }

    template <typename U>
    bool operator!=(const U* other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data != Convert<U, T>(std::basic_string<U>(other));
    }

    template <typename U>
    bool operator<(const AtomicString<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data < Convert<U, T>(other);
    }

    template <typename U>
    bool operator<(const std::basic_string<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data < Convert<U, T>(other);
    }

    template <typename U>
    bool operator<(const U* other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data < Convert<U, T>(std::basic_string<U>(other));
    }

    template <typename U>
    bool operator<=(const AtomicString<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data <= Convert<U, T>(other);
    }

    template <typename U>
    bool operator<=(const std::basic_string<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data <= Convert<U, T>(other);
    }

    template <typename U>
    bool operator<=(const U* other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data <= Convert<U, T>(std::basic_string<U>(other));
    }

    template <typename U>
    bool operator>(const AtomicString<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data > Convert<U, T>(other);
    }

    template <typename U>
    bool operator>(const std::basic_string<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data > Convert<U, T>(other);
    }

    template <typename U>
    bool operator>(const U* other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data > Convert<U, T>(std::basic_string<U>(other));
    }

    template <typename U>
    bool operator>=(const AtomicString<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data >= Convert<U, T>(other);
    }

    template <typename U>
    bool operator>=(const std::basic_string<U>& other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data >= Convert<U, T>(other);
    }

    template <typename U>
    bool operator>=(const U* other) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data >= Convert<U, T>(std::basic_string<U>(other));
    }

    template <typename U>
    AtomicString operator+(const AtomicString<U>& other) const
    {
        AtomicString<T> otherConverted(other);

        std::scoped_lock lock(mutex, otherConverted.mutex);

        AtomicString<T> result;

        result.data = this->data + otherConverted.data;

        return result;
    }

    template <typename U>
    AtomicString operator+(const std::basic_string<U>& other) const
    {
        AtomicString<T> otherConverted(other);

        std::scoped_lock lock(mutex, otherConverted.mutex);

        AtomicString<T> result;

        result.data = this->data + otherConverted.data;

        return result;
    }

    template <typename U>
    AtomicString operator+(const U* other) const
    {
        AtomicString<T> otherConverted(other);

        std::scoped_lock lock(mutex, otherConverted.mutex);

        AtomicString<T> result;

        result.data = this->data + otherConverted.data;

        return result;
    }

    template <typename U>
    AtomicString& operator+=(const AtomicString<U>& other)
    {
        AtomicString<T> otherConverted(other);

        std::scoped_lock lock(mutex, otherConverted.mutex);

        this->data += otherConverted.data;

        return *this;
    }

    template <typename U>
    AtomicString& operator+=(const std::basic_string<U>& other)
    {
        AtomicString<T> otherConverted(other);

        std::scoped_lock lock(mutex, otherConverted.mutex);

        this->data += otherConverted.data;

        return *this;
    }

    template <typename U>
    AtomicString& operator+=(const U* other)
    {
        AtomicString<T> otherConverted(other);

        std::scoped_lock lock(mutex, otherConverted.mutex);

        this->data += otherConverted.data;

        return *this;
    }

    template <typename U>
    AtomicString operator-(const AtomicString<U>& other) const
    {
        AtomicString<T> otherConverted(other);

        std::scoped_lock lock(mutex, otherConverted.mutex);

        AtomicString<T> result;

        result.data = this->data;

        size_t position = result.data.find(otherConverted.data);

        if (position != std::basic_string<T>::npos)
            result.data.erase(position, otherConverted.data.length());

        return result;
    }

    template <typename U>
    AtomicString operator-(const std::basic_string<U>& other) const
    {
        AtomicString<T> otherConverted(other);

        std::scoped_lock lock(mutex, otherConverted.mutex);

        AtomicString<T> result;

        result.data = this->data;

        size_t position = result.data.find(otherConverted.data);

        if (position != std::basic_string<T>::npos)
            result.data.erase(position, otherConverted.data.length());

        return result;
    }

    template <typename U>
    AtomicString operator-(const U* other) const
    {
        AtomicString<T> otherConverted(other);

        std::scoped_lock lock(mutex, otherConverted.mutex);

        AtomicString<T> result;

        result.data = this->data;

        size_t position = result.data.find(otherConverted.data);

        if (position != std::basic_string<T>::npos)
            result.data.erase(position, otherConverted.data.length());

        return result;
    }

    template <typename U>
    AtomicString& operator-=(const AtomicString<U>& other)
    {
        AtomicString<T> otherConverted(other);
        std::scoped_lock lock(mutex, otherConverted.mutex);
        size_t position = data.find(otherConverted.data);

        if (position != std::basic_string<T>::npos)
            data.erase(position, otherConverted.data.length());

        return *this;
    }

    template <typename U>
    AtomicString& operator-=(const std::basic_string<U>& other)
    {
        AtomicString<T> otherConverted(other);
        std::scoped_lock lock(mutex, otherConverted.mutex);
        size_t position = data.find(otherConverted.data);

        if (position != std::basic_string<T>::npos)
            data.erase(position, otherConverted.data.length());

        return *this;
    }

    template <typename U>
    AtomicString& operator-=(const U* other)
    {
        AtomicString<T> otherConverted(other);
        std::scoped_lock lock(mutex, otherConverted.mutex);
        size_t position = data.find(otherConverted.data);

        if (position != std::basic_string<T>::npos)
            data.erase(position, otherConverted.data.length());

        return *this;
    }

    T& operator[](size_t index)
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return data[index];
    }

    template <typename F, typename L>
    void FindAndReplace(const AtomicString<F>& find, const AtomicString<L>& replace)
    {
        std::scoped_lock lock(mutex, find.mutex, replace.mutex);

        size_t pos = 0;

        while ((pos = data.find(find.data, pos)) != std::basic_string<T>::npos)
        {
            data.replace(pos, find.data.length(), replace.data);
            pos += replace.data.length();
        }
    }

    template <typename F, typename L>
    void FindAndReplace(const std::basic_string<F>& find, const std::basic_string<L>& replace)
    {
        std::unique_lock<std::shared_mutex> lock(mutex);

        size_t pos = 0;

        while ((pos = data.find(find, pos)) != std::basic_string<T>::npos)
        {
            data.replace(pos, find.length(), replace);
            pos += replace.length();
        }
    }

    template <typename F, typename L>
    void FindAndReplace(const F* find, const L* replace)
    {
        std::unique_lock<std::shared_mutex> lock(mutex);

        size_t pos = 0;

        while ((pos = data.find(find, pos)) != std::basic_string<T>::npos)
        {
            data.replace(pos, std::char_traits<F>::length(find), replace);
            pos += std::char_traits<L>::length(replace);
        }
    }

    void ToUpper()
    {
        std::unique_lock<std::shared_mutex> lock(mutex);
        std::transform(data.begin(), data.end(), data.begin(), ::toupper);
    }

    void ToLower()
    {
        std::unique_lock<std::shared_mutex> lock(mutex);
        std::transform(data.begin(), data.end(), data.begin(), ::tolower);
    }

    ThreadSafeIterator<T> begin()
    {
        return ThreadSafeIterator<T>(*this, data.begin());
    }

    ThreadSafeIterator<T> end()
    {
        return ThreadSafeIterator<T>(*this, data.end());
    }

	size_t Length() const
	{
		std::shared_lock<std::shared_mutex> lock(mutex);
		return data.length();
	}

    void Clear()
    {
        std::unique_lock<std::shared_mutex> lock(mutex);
        data.clear();
    }

    template <typename U>
    operator std::basic_string<U>() const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return Convert<T, U>(data);
    }

    template <typename U>
    operator const U* () const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return Convert<T, U>(data).c_str();
    }

private:

    template <typename F, typename L>
	AtomicString<L> Convert(const AtomicString<F>& from) const
    {
        std::shared_lock<std::shared_mutex> lock(mutex);
        return Convert<F, L>(from.data);
    }

    template <typename F, typename L>
    std::basic_string<L> Convert(const std::basic_string<F>& from) const
    {
        static_assert(
            std::is_same<F, char>::value || std::is_same<F, wchar_t>::value ||
            std::is_same<F, char16_t>::value || std::is_same<F, char32_t>::value,
            "Convert only supports char, wchar_t, char16_t, and char32_t types."
            );

        static_assert(
            std::is_same<L, char>::value || std::is_same<L, wchar_t>::value ||
            std::is_same<L, char16_t>::value || std::is_same<L, char32_t>::value,
            "Convert only supports char, wchar_t, char16_t, and char32_t types."
            );

        if constexpr (std::is_same<F, L>::value)
            return from;
        else if constexpr (std::is_same<F, char>::value && std::is_same<L, wchar_t>::value)
        {
            std::basic_string<L> result(from.size(), L'\0');

            std::mbstowcs(result.data(), from.c_str(), from.size());

            return result;
        }
        else if constexpr (std::is_same<F, wchar_t>::value && std::is_same<L, char>::value)
        {
            std::basic_string<L> result(from.size() * MB_CUR_MAX, '\0');

            std::wcstombs(result.data(), from.c_str(), from.size() * MB_CUR_MAX);

            result.resize(std::strlen(result.c_str()));

            return result;
        }
        else
        {
            std::cout << std::format("Unsupported character conversion. Conversion between '{}' and '{}'.", typeid(F).name(), typeid(L).name()) << std::endl;
            throw std::runtime_error("Unsupported character conversion.");
        }
    }

    template <typename T>
	friend std::basic_ostream<T>& operator<<(std::basic_ostream<T>& stream, const AtomicString<T>& str);

	template <typename T1, typename T2>
	friend auto operator+(const AtomicString<T1>& lhs, const AtomicString<T2>& rhs);

	template <typename T1, typename T2>
	friend auto operator-(const AtomicString<T1>& lhs, const AtomicString<T2>& rhs);

    mutable std::shared_mutex mutex;

    std::basic_string<T> data;

};

template <typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& stream, const AtomicString<T>& str)
{
    static_assert(
        std::is_same<T, char>::value || std::is_same<T, wchar_t>::value ||
        std::is_same<T, char16_t>::value || std::is_same<T, char32_t>::value,
        "T only supports char, wchar_t, char16_t, and char32_t types."
        );

    std::shared_lock<std::shared_mutex> lock(str.mutex);

    stream << str.data;

    return stream;
}

template <typename T1, typename T2>
auto operator+(const AtomicString<T1>& lhs, const AtomicString<T2>& rhs)
{
    static_assert(
        std::is_same<T1, char>::value || std::is_same<T1, wchar_t>::value ||
        std::is_same<T1, char16_t>::value || std::is_same<T1, char32_t>::value,
        "T1 only supports char, wchar_t, char16_t, and char32_t types."
        );

    static_assert(
        std::is_same<T2, char>::value || std::is_same<T2, wchar_t>::value ||
        std::is_same<T2, char16_t>::value || std::is_same<T2, char32_t>::value,
        "T2 only supports char, wchar_t, char16_t, and char32_t types."
        );

    using ReturnType = std::conditional_t<std::is_same_v<T1, wchar_t> || std::is_same_v<T2, wchar_t>, wchar_t, char>;

    AtomicString<ReturnType> lhsConverted = lhs.operator std::basic_string<ReturnType>();
    AtomicString<ReturnType> rhsConverted = rhs.operator std::basic_string<ReturnType>();

    std::scoped_lock lock(lhsConverted.mutex, rhsConverted.mutex);

    AtomicString<ReturnType> result;

    result.data = lhsConverted.data + rhsConverted.data;

    return result;
}

template <typename T1, typename T2>
auto operator-(const AtomicString<T1>& lhs, const AtomicString<T2>& rhs)
{
    static_assert(
        std::is_same<T1, char>::value || std::is_same<T1, wchar_t>::value ||
        std::is_same<T1, char16_t>::value || std::is_same<T1, char32_t>::value,
        "T1 only supports char, wchar_t, char16_t, and char32_t types."
        );

    static_assert(
        std::is_same<T2, char>::value || std::is_same<T2, wchar_t>::value ||
        std::is_same<T2, char16_t>::value || std::is_same<T2, char32_t>::value,
        "T2 only supports char, wchar_t, char16_t, and char32_t types."
        );

    using ReturnType = std::conditional_t<std::is_same_v<T1, wchar_t> || std::is_same_v<T2, wchar_t>, wchar_t, char>;

    AtomicString<ReturnType> lhsConverted = lhs.operator std::basic_string<ReturnType>();
    AtomicString<ReturnType> rhsConverted = rhs.operator std::basic_string<ReturnType>();

    std::scoped_lock lock(lhsConverted.mutex, rhsConverted.mutex);

    AtomicString<ReturnType> result = lhsConverted;

    size_t pos = 0;

    while ((pos = result.data.find(rhsConverted.data, pos)) != std::basic_string<ReturnType>::npos)
        result.data.erase(pos, rhsConverted.data.length());

    return result;
}