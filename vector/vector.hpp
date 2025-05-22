#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <memory>
#include <initializer_list>
#include <iterator>
#include <algorithm>
#include <stdexcept>
#include <utility>
#include <ostream>
#include <istream>

template<typename T, typename Allocator = std::allocator<T>>
class Vector {

public:
	using value_type = T;
	using allocator_type = Allocator;
	using allocator_traits = std::allocator_traits<allocator_type>;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = typename std::allocator_traits<allocator_type>::pointer;
	using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	//---------------------------------------------------------------------------------------------- konstruktoriai

	Vector() noexcept(std::is_nothrow_default_constructible<Allocator>::value) // default
		: allocator_(), data_(nullptr), size_(0), capacity_(0) {
	}

	explicit Vector(size_type count, const T& value, const Allocator& alloc = Allocator())  // count
		: allocator_(alloc), size_(count), capacity_(count)
	{
		data_ = std::allocator_traits<Allocator>::allocate(allocator_, capacity_);
		for (size_type i = 0; i < size_; ++i)
			std::allocator_traits<Allocator>::construct(allocator_, data_ + i, value);
	}


	template <typename InputIt, //range 
		typename = std::enable_if_t<!std::is_integral<InputIt>::value>>  // ne enable_if, kadangi butu dviprasmiskumas jei vartotojas duotu 2 int
		Vector(InputIt first, InputIt last, const Allocator& allocate = Allocator())
		: allocator_(allocate)
	{
		size_ = capacity_ = std::distance(first, last);
		data_ = std::allocator_traits<Allocator>::allocate(allocator_, capacity_);
		size_type i = 0;
		for (InputIt it = first; it != last; ++it, ++i)
			std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *it);
	}

	Vector(const Vector& other) //kopijavimo
		: size_(other.size_), capacity_(other.capacity_), allocator_(other.allocator_) {
		data_ = allocator_traits::allocate(allocator_, capacity_);
		for (size_type i = 0; i < size_; ++i) {
			allocator_traits::construct(allocator_, data_ + i, other.data_[i]);
		}
	}

	Vector(Vector&& other) noexcept //kelimo
		: data_(other.data_), size_(other.size_), capacity_(other.capacity_),
		allocator_(std::move(other.allocator_)) {
		other.data_ = nullptr;
		other.size_ = 0;
		other.capacity_ = 0;
	}

	Vector(std::initializer_list<T> init) //list inicializacija
		: size_(init.size()), capacity_(init.size()), allocator_() {
		data_ = allocator_traits::allocate(allocator_, capacity_);
		std::uninitialized_copy(init.begin(), init.end(), data_);
	}

	Vector(size_t count)  ///bufferiams
	{
		data_ = allocator_.allocate(count);
		for (size_t i = 0; i < count; ++i)
			allocator_.construct(data_ + i);
		size_ = count;
		capacity_ = count;
	}


	~Vector() { //destruktorius
		clear();
		if (data_) {
			allocator_traits::deallocate(allocator_, data_, capacity_);
		}
	}


	//---------------------------------------------------------------------------------------------- priskyrimo operatoriai

	Vector& operator=(const Vector& other) { //kopijavimo
		if (this != &other) {
			Vector tmp(other);
			swap(tmp);
		}
		return *this;
	}

	Vector& operator=(Vector&& other) noexcept { //kelimo
		if (this != &other) {
			clear();
			if (data_) {
				allocator_traits::deallocate(allocator_, data_, capacity_);
			}

			data_ = other.data_;
			size_ = other.size_;
			capacity_ = other.capacity_;
			allocator_ = std::move(other.allocator_);

			other.data_ = nullptr;
			other.size_ = 0;
			other.capacity_ = 0;
		}
		return *this;
	}
	//---------------------------------------------------------------------------------------------- iteratoriai

	iterator begin() noexcept { return data_; }
	const_iterator begin() const noexcept { return data_; }
	const_iterator cbegin() const noexcept { return data_; }

	iterator end() noexcept { return data_ + size_; }
	const_iterator end() const noexcept { return data_ + size_; }
	const_iterator cend() const noexcept { return data_ + size_; }

	reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

	reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

	//---------------------------------------------------------------------------------------------- capacity



	bool empty() const noexcept { return size_ == 0; }
	size_type size() const noexcept { return size_; }
	size_type capacity() const noexcept { return capacity_; }
	size_type max_size() const noexcept {
		return std::allocator_traits<Allocator>::max_size(allocator_);
	}


	void reserve(size_type new_cap) {
		if (new_cap <= capacity_) return;

		pointer newdata = std::allocator_traits<Allocator>::
			allocate(allocator_, new_cap);

		for (size_type i = 0; i < size_; ++i) {
			std::allocator_traits<Allocator>::construct(
				allocator_, newdata + i,
				std::move_if_noexcept(data_[i])
			);
			std::allocator_traits<Allocator>::destroy(
				allocator_, data_ + i
			);
		}

		if (data_) {
			std::allocator_traits<Allocator>::deallocate(
				allocator_, data_, capacity_);
		}

		data_ = newdata;
		capacity_ = new_cap;
	}

	void shrink_to_fit() {
		if (size_ == capacity_)
			return;

		// 1) remember the current size
		size_type old_size = size_;

		// 2) allocate exactly old_size slots
		pointer new_data = std::allocator_traits<Allocator>::
			allocate(allocator_, old_size);

		// 3) move-construct each element into new_data
		size_type i = 0;
		try {
			for (; i < old_size; ++i) {
				std::allocator_traits<Allocator>::construct(
					allocator_,
					new_data + i,
					std::move_if_noexcept(data_[i])
				);
			}
		}
		catch (...) {
			// if any construct throws, roll back what we built so far
			for (size_type j = 0; j < i; ++j) {
				std::allocator_traits<Allocator>::destroy(
					allocator_, new_data + j
				);
			}
			std::allocator_traits<Allocator>::deallocate(
				allocator_, new_data, old_size
			);
			throw;
		}

		// 4) destroy old elements (but do not touch size_ yet)
		for (size_type j = 0; j < old_size; ++j) {
			std::allocator_traits<Allocator>::destroy(
				allocator_, data_ + j
			);
		}
		// 5) free old storage
		std::allocator_traits<Allocator>::deallocate(
			allocator_, data_, capacity_
		);

		// 6) commit the new buffer
		data_ = new_data;
		capacity_ = old_size;
		size_ = old_size;  // restore size
	}


	//---------------------------------------------------------------------------------------------- modifiers


	void clear() noexcept {
		for (size_type i = 0; i < size_; ++i) {
			std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
		}
		size_ = 0;
	}


	void push_back(const T& value) {
		if (size_ == capacity_) {
			reserve(capacity_ ? capacity_ * 10 : 1);
		}
		std::allocator_traits<Allocator>::construct(
			allocator_, data_ + size_, value
		);
		++size_;
	}

	void push_back(T&& value) {
		if (size_ == capacity_) {
			reserve(capacity_ ? capacity_ * 10 : 1);
		}
		std::allocator_traits<Allocator>::construct(
			allocator_, data_ + size_, std::move(value)
		);
		++size_;
	}


	template <class... Args>
	reference emplace_back(Args&&... args) {
		if (size_ == capacity_) {
			reserve(capacity_ ? capacity_ * 10 : 1);
		}
		std::allocator_traits<Allocator>::construct(
			allocator_, data_ + size_, std::forward<Args>(args)...
		);
		return *(data_ + size_++);
	}

	template<class... Args>
	iterator emplace(const_iterator pos, Args&&... args) {
		size_type idx = pos - data_;
		if (size_ == capacity_) {
			reserve(capacity_ ? capacity_ * 10 : 1);
		}
		for (size_type i = size_; i > idx; --i) {
			std::allocator_traits<Allocator>::construct(
				allocator_, data_ + i,
				std::move_if_noexcept(data_[i - 1])
			);
			std::allocator_traits<Allocator>::destroy(allocator_, data_ + i - 1);
		}
		std::allocator_traits<Allocator>::construct(
			allocator_, data_ + idx, std::forward<Args>(args)...
		);
		++size_;
		return data_ + idx;
	}

	void pop_back() noexcept {
		if (size_ > 0) {
			--size_;
			std::allocator_traits<Allocator>::destroy(allocator_, data_ + size_);
		}
	}

	void resize(size_type count, const T& value = T()) {
		if (count < size_) {
			for (size_type i = count; i < size_; ++i) {
				std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
			}
			size_ = count;
		}
		else if (count > size_) {
			if (count > capacity_) reserve(count);
			size_type i = size_;
			try {
				for (; i < count; ++i) {
					std::allocator_traits<Allocator>::construct(
						allocator_, data_ + i, value
					);
				}
			}
			catch (...) {
				for (size_type j = size_; j < i; ++j) {
					std::allocator_traits<Allocator>::destroy(allocator_, data_ + j);
				}
				throw;
			}
			size_ = count;
		}
	}

	void swap(Vector& other) noexcept
	{
		using std::swap;
		swap(data_, other.data_);
		swap(size_, other.size_);
		swap(capacity_, other.capacity_);
		swap(allocator_, other.allocator_);
	}

	iterator erase(const_iterator pos) {
		size_type idx = pos - data_;
		std::allocator_traits<Allocator>::destroy(allocator_, data_ + idx);
		for (size_type i = idx; i + 1 < size_; ++i) {  //sunaikinam elementa ir patraukiam i kaire
			std::allocator_traits<Allocator>::construct(
				allocator_, data_ + i,
				std::move_if_noexcept(data_[i + 1])
			);
			std::allocator_traits<Allocator>::destroy(allocator_, data_ + i + 1);
		}
		--size_;
		return data_ + idx;
	}

	iterator erase(const_iterator first, const_iterator last) {
		size_type idx1 = first - data_;
		size_type idx2 = last - data_;
		size_type n = idx2 - idx1;
		if (n == 0) return data_ + idx1;

		for (size_type i = idx1; i < idx2; ++i) {
			std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
		}
		for (size_type i = idx2; i < size_; ++i) {
			std::allocator_traits<Allocator>::construct(
				allocator_, data_ + (i - n),
				std::move_if_noexcept(data_[i])
			);
			std::allocator_traits<Allocator>::destroy(allocator_, data_ + i);
		}
		size_ -= n;
		return data_ + idx1;
	}

	template<typename InputIt> // leidzia assign dirbti ant bet kokio iteratoriaus range
	void assign(InputIt first, InputIt last) {
		clear();
		reserve(std::distance(first, last));
		for (; first != last; ++first)
			push_back(*first);
	}

	iterator insert(const_iterator pos, T&& value) {
		size_type idx = pos - data_;
		if (size_ == capacity_) {
			reserve(capacity_ ? capacity_ * 2 : 1);
		}
		// patraukiam tada emplacinam
		for (size_type i = size_; i > idx; --i) {
			std::allocator_traits<Allocator>::construct(
				allocator_, data_ + i,
				std::move_if_noexcept(data_[i - 1])
			);
			std::allocator_traits<Allocator>::destroy(allocator_, data_ + i - 1);
		}
		std::allocator_traits<Allocator>::construct(
			allocator_, data_ + idx, std::move(value)
		);
		++size_;
		return data_ + idx;
	}

	// fill insert
	iterator insert(const_iterator pos, size_type n, const T& value) {
		size_type idx = pos - data_;
		if (n == 0) return data_ + idx;
		// uztikrinam del talpos
		if (size_ + n > capacity_) {
			reserve(std::max(capacity_ * 2, size_ + n));
		}
		// patraukiam senus elementus
		for (size_type i = size_ + n - 1; i >= idx + n; --i) {
			std::allocator_traits<Allocator>::construct(
				allocator_, data_ + i,
				std::move_if_noexcept(data_[i - n])
			);
			std::allocator_traits<Allocator>::destroy(allocator_, data_ + i - n);
		}
		for (size_type i = 0; i < n; ++i) { //uzpildom tarpa
			std::allocator_traits<Allocator>::construct(allocator_, data_ + idx + i, value);
		}
		size_ += n;
		return data_ + idx;
	}

	template<class InputIt,
		class = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
	iterator insert(const_iterator pos, InputIt first, InputIt last) {
		size_type idx = pos - data_;
		size_type n = std::distance(first, last);
		if (n == 0) return data_ + idx;
		if (size_ + n > capacity_) {
			reserve(std::max(capacity_ * 2, size_ + n));
		}
		for (size_type i = size_ + n - 1; i >= idx + n; --i) {
			std::allocator_traits<Allocator>::construct(
				allocator_, data_ + i,
				std::move_if_noexcept(data_[i - n])
			);
			std::allocator_traits<Allocator>::destroy(allocator_, data_ + i - n);
		}
		size_type i = idx;
		for (InputIt it = first; it != last; ++it, ++i) {
			std::allocator_traits<Allocator>::construct(allocator_, data_ + i, *it);
		}
		size_ += n;
		return data_ + idx;
	}

	//---------------------------------------------------------------------------------------------- elementu access

// netikrinamas
	reference       operator[](size_type idx) noexcept { return data_[idx]; }
	const_reference operator[](size_type idx) const noexcept { return data_[idx]; }

	//patikrinamas access (meta std::out_of_range ant invalid idx)
	reference at(size_type idx) {
		if (idx >= size_) {
			throw std::out_of_range("Vector::at() index out of range");
		}
		return data_[idx];
	}
	const_reference at(size_type idx) const {
		if (idx >= size_) {
			throw std::out_of_range("Vector::at() index out of range");
		}
		return data_[idx];
	}

	// accessina pirma ir paskutini el (undefined behaviour jei tuscias)
	reference       front() noexcept { return data_[0]; }
	const_reference front() const noexcept { return data_[0]; }

	reference       back() noexcept { return data_[size_ - 1]; }
	const_reference back() const noexcept { return data_[size_ - 1]; }

	// tiesiogine prieiga i masyva ( nullptr jei tuscia)
	pointer         data() noexcept { return data_; }
	const_pointer   data() const noexcept { return data_; }

private:
	Allocator allocator_;
	pointer data_;
	size_type size_;
	size_type capacity_;


};

//---------------------------------------------------------------------------------------------- non member funkcijos
template<typename T, typename Alloc>
void swap(Vector<T, Alloc>& a, Vector<T, Alloc>& b) noexcept(noexcept(a.swap(b))) {
	a.swap(b);
}

template<typename T, typename Alloc>
bool operator==(const Vector<T, Alloc>& a, const Vector<T, Alloc>& b) {
	if (a.size() != b.size()) return false;
	return std::equal(a.begin(), a.end(), b.begin());
}

template<typename T, typename Alloc>
bool operator!=(const Vector<T, Alloc>& a, const Vector<T, Alloc>& b) {
	return !(a == b);
}

template<typename T, typename Alloc>
bool operator<(const Vector<T, Alloc>& a, const Vector<T, Alloc>& b) {
	return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

template<typename T, typename Alloc>
bool operator>(const Vector<T, Alloc>& a, const Vector<T, Alloc>& b) {
	return b < a;
}

template<typename T, typename Alloc>
bool operator<=(const Vector<T, Alloc>& a, const Vector<T, Alloc>& b) {
	return !(b < a);
}

template<typename T, typename Alloc>
bool operator>=(const Vector<T, Alloc>& a, const Vector<T, Alloc>& b) {
	return !(a < b);
}

template<typename T, typename Alloc>
std::ostream& operator<<(std::ostream& os, const Vector<T, Alloc>& vec) {
	os << "[";
	for (size_t i = 0; i < vec.size(); ++i) {
		os << vec[i];
		if (i < vec.size() - 1) os << ", ";
	}
	return os << "]";
}

template<typename T, typename Alloc>
std::istream& operator>>(std::istream& is, Vector<T, Alloc>& vec) {
	vec.clear();

	// Read initial [
	char ch;
	if (!(is >> ch) || ch != '[') {
		is.setstate(std::ios::failbit);
		return is;
	}

	// Now read values until ']' or failure
	for (;;) {
		// Skip whitespace
		is >> std::ws;

		// Peek to see if next is closing bracket
		if (is.peek() == ']') {
			is.get(ch);  // consume ']'
			break;
		}

		// Read one element
		T value;
		if (!(is >> value)) {
			// failed to read T
			return is;
		}
		vec.push_back(std::move(value));

		// After a value, read comma or closing bracket
		if (!(is >> ch)) {
			return is;
		}
		if (ch == ']') {
			break;
		}
		if (ch != ',') {
			is.setstate(std::ios::failbit);
			return is;
		}
		// else loop to read next element
	}

	return is;
}


#endif