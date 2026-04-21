#include <algorithm>
#include <cstddef>
#include <cstring>
#include <vector>

class str
{
private:
    char *data_;
    std::size_t size_;
    std::size_t cap_;

    void allocate_and_copy(const char *s, std::size_t n)
    {
        // Allocate n + 1 (for null terminator)
        cap_ = n + 1;
        data_ = new char[cap_];
        if (n)
            std::memcpy(data_, s, n);
        size_ = n;
        data_[size_] = '\0';
    }

public:
    // Default constructor: empty string
    str() : data_(nullptr), size_(0), cap_(0)
    {
        allocate_and_copy("", 0);
    }

    // Construct from single character
    str(const char &c) : data_(nullptr), size_(0), cap_(0)
    {
        char tmp[2] = {c, '\0'};
        allocate_and_copy(tmp, 1);
    }

    // Construct from C string (rvalue reference as per spec)
    str(const char *&& s_) : data_(nullptr), size_(0), cap_(0)
    {
        const char *s = s_ ? s_ : "";
        allocate_and_copy(s, std::strlen(s));
    }

    // Assign from C string (rvalue reference as per spec)
    str &operator=(const char *&& s_)
    {
        const char *s = s_ ? s_ : "";
        std::size_t n = std::strlen(s);
        if (n + 1 > cap_)
        {
            delete[] data_;
            data_ = nullptr;
            cap_ = 0;
            allocate_and_copy(s, n);
        }
        else
        {
            if (n)
                std::memcpy(data_, s, n);
            size_ = n;
            data_[size_] = '\0';
        }
        return *this;
    }

    // Copy constructor
    str(const str &other) : data_(nullptr), size_(0), cap_(0)
    {
        allocate_and_copy(other.data_, other.size_);
    }

    // Copy assignment
    str &operator=(const str &other)
    {
        if (this == &other)
            return *this;
        if (other.size_ + 1 > cap_)
        {
            delete[] data_;
            data_ = nullptr;
            cap_ = 0;
            allocate_and_copy(other.data_, other.size_);
        }
        else
        {
            if (other.size_)
                std::memcpy(data_, other.data_, other.size_);
            size_ = other.size_;
            data_[size_] = '\0';
        }
        return *this;
    }

    // Element access (0-based). No bounds checking as typical operator[]
    char &operator[](size_t pos)
    {
        return data_[pos];
    }

    // Length of the string
    size_t len() const
    {
        return size_;
    }

    // Join: concatenate strs with *this as separator
    str join(const std::vector<str> &strs) const
    {
        if (strs.empty())
        {
            return str();
        }

        std::size_t sep_len = size_;
        std::size_t total_len = 0;
        for (const auto &s : strs)
        {
            total_len += s.size_;
        }
        if (strs.size() > 1)
            total_len += sep_len * (strs.size() - 1);

        str result;
        // Ensure capacity
        if (total_len + 1 > result.cap_)
        {
            delete[] result.data_;
            result.data_ = new char[total_len + 1];
            result.cap_ = total_len + 1;
        }

        std::size_t pos = 0;
        for (std::size_t i = 0; i < strs.size(); ++i)
        {
            const str &part = strs[i];
            if (part.size_)
            {
                std::memcpy(result.data_ + pos, part.data_, part.size_);
                pos += part.size_;
            }
            if (i + 1 < strs.size() && sep_len)
            {
                std::memcpy(result.data_ + pos, data_, sep_len);
                pos += sep_len;
            }
        }
        result.size_ = total_len;
        result.data_[result.size_] = '\0';
        return result;
    }

    // Slice [l, r)
    str slice(size_t l, size_t r) const
    {
        if (l > r)
            std::swap(l, r);
        if (l > size_)
            l = size_;
        if (r > size_)
            r = size_;
        std::size_t n = (r > l) ? (r - l) : 0;
        str result;
        if (n + 1 > result.cap_)
        {
            delete[] result.data_;
            result.data_ = new char[n + 1];
            result.cap_ = n + 1;
        }
        if (n)
            std::memcpy(result.data_ + 0, data_ + l, n);
        result.size_ = n;
        result.data_[result.size_] = '\0';
        return result;
    }

    ~str()
    {
        delete[] data_;
        data_ = nullptr;
        size_ = 0;
        cap_ = 0;
    }
};

