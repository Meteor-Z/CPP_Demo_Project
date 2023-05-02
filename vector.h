//
// Created by meteor on 23-5-2.
//

#ifndef CPP_STL_VECTOR_H
#define CPP_STL_VECTOR_H

#include <iostream>

namespace my_stl
{
    template<typename T>
    class vector
    {
    public:
        explicit vector() noexcept = default;

        // 进行默认初始化
        explicit vector(size_t size) : capacity_{size}, ptr_{alloc(capacity_)}
        {
            for (; size_ < size; size_++)
            {
                construct(ptr_ + size_);
            }
        }

        vector(size_t size, const T& value) : capacity_{size}, ptr_{alloc(capacity_)}
        {
            for (; size_ < size; size_++)
            {
                construct(ptr_ + size_, value); // 万能引用
            }
        }

        vector(const vector& rhs) : capacity_{rhs.size()}, ptr_{alloc(capacity_)}
        {
            for (; size_ < rhs.size(); size_++)
            {
                construct(ptr_ + size_, rhs[size_]);
            }
        }

        // 移动构造
        vector(vector&& rhs)
        {
            capacity_ = rhs.capacity_;
            rhs.capacity_ = 0;

            size_ = rhs.size_;
            rhs.size_ = 0;

            ptr_ = rhs.ptr_;
            rhs.ptr_ = nullptr;
        }

        // 初始化列表
        vector(std::initializer_list<T> list) : capacity_{list.size()}, ptr_{alloc(capacity_)}
        {
            for (auto& item: list)
            {
                construct(ptr_ + size_, item);
                ++size_;
            }
        }

        void swap(vector& rhs) noexcept
        {
            std::swap(size_, rhs.size_);
            std::swap(capacity_, rhs.capacity_);
            std::swap(ptr_, rhs.ptr_);
        }

        void clear()
        {
            for (; size_ > 0; --size_)
            {
                destroy(ptr_ + size_ - 1);

            }
        }

        ~vector() noexcept
        {
            clear();
            dealloc(ptr_);

        }

        vector& operator =(const vector& rhs)
        {
            if (this != &rhs) // 如果不是自己才会进行交换
            {
                vector{rhs}.swap(*this);
            }
            return *this;
        }

        vector& operator =(vector&& rhs) noexcept
        {
            if (this != &rhs)
            {
                vector{rhs}.swap(*this);
            }
            return *this;

        }

        vector& operator =(std::initializer_list<T> list)
        {
            vector{list}.swap(*this);
            return *this;
        }

        void push_back(const T& value)
        {
            emplace_back(value);
        }

        void push_back(T&& value)
        {
            // 注意，这里 value 是右值，但是传入参数的时候还是会变成左值，参数永远是左值
            emplace_back(std::move(value));
        }

        template<typename... Args>
        void emplace_back(Args&& ... args) // 万能引用
        {
            if (size_ == capacity_)
            {
                auto new_capacity = capacity_ ? capacity_ * 2 : 1;
                auto new_ptr = alloc(new_capacity);
                for (size_t new_size = 0; new_size < size_; ++new_size)
                {
                    construct(new_ptr + new_size, std::move_if_noexcept(ptr_[new_size]));
                }
                capacity_ = new_capacity;
                ptr_ = new_ptr;
            }
            construct(ptr_ + size_, std::forward<Args>(args)...);
            ++size_;
        }

        void pop_back() noexcept
        {
            destroy(ptr_ + size_ - 1);
            --size_;
        }

        size_t size() const noexcept
        {
            return size_;
        }

        size_t capacity() const noexcept
        {
            return capacity_;
        }

        bool empty() const noexcept
        {
            return size_ == 0;
        }

        T& operator [](size_t index)
        {
            return ptr_[index];
        }

        const T& operator [](size_t index) const
        {
            return ptr_[index];
        }

        T* begin() noexcept
        {
            return ptr_;
        }

        const T* begin() const
        {
            return ptr_;
        }

        T* end() noexcept
        {
            return ptr_ + size_;
        }


        const T* end() const noexcept
        {
            return ptr_ + size_;
        }

    private:
        // 内存分配， ::operator new()相当于于一个裸的内存分配，就是只是分配一个内存
        T* alloc(size_t size)
        {
            return static_cast<T*>(::operator new(sizeof(T) * size));
        }

        void dealloc(T* ptr)
        {
            ::operator delete(ptr);
        }

        template<typename...Args>
        void construct(T* ptr, Args&& ... args)
        {
            ::new(ptr) T{std::forward<Args>(args)...};
        }

        void destroy(T* ptr) noexcept
        {
            ptr->~T();
        }


        size_t capacity_{0};
        size_t size_{0};
        T* ptr_{nullptr};
    };
} // my space

template<typename T>
void swap(my_stl::vector<T>& a, my_stl::vector<T>& b)
{
    a.swap(b);
}

#endif //CPP_STL_VECTOR_H
