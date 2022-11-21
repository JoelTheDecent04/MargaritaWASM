#pragma once
#include <assert.h>
#include <vector>

template <typename T>
class List
{
	std::vector<T*> vec;
public:
	struct Iterator
	{
		T** ptr;
		Iterator(T** ptr) { this->ptr = ptr; }
		T& operator*() { return **ptr; }
		T* operator->() { return *ptr; }
		Iterator& operator++() { ptr++; return *this; }
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		friend bool operator==(Iterator& a, Iterator& b) { return a.ptr == b.ptr; }
		friend bool operator!=(Iterator& a, Iterator& b) { return a.ptr != b.ptr; }
	};

	List() {};
	~List()
	{
		for (auto& element : vec)
		{
			delete element;
		}
	}
	List(List&&) = default;
	List& operator=(List&&) = default;

	List(const List&) = delete;
	List& operator=(const List&) = delete;
	
	template <typename S>
	void Add(S* element)
	{
		vec.push_back(element);
	}
	Iterator begin()
	{
		return Iterator(&vec[0]);
	}
	Iterator end()
	{
		return Iterator(&vec[0] + vec.size());
	}
	T& operator[](size_t index)
	{
		assert(index < size());
		return *vec[index];
	}
	size_t size()
	{
		return vec.size();
	}
	void RemoveAt(size_t index)
	{
		delete vec[index];
		vec.erase(vec.begin() + index);
	}
	std::unique_ptr<T> Take(size_t index)
	{
		T* ptr = vec[index];
		vec.erase(vec.begin() + index);
		return std::unique_ptr<T>(ptr);
	}
	void Append(List<T>&& other)
	{
		vec.insert(
			vec.end(),
			std::make_move_iterator(other.vec.begin()),
			std::make_move_iterator(other.vec.end()));
		other.vec.clear();
	}
};

struct Rect
{
	int left, top, right, bottom;

	Rect() = default;
	Rect(int left, int top, int right, int bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
};

float distance(float ax, float ay, float bx, float by);
float randomf();
float random_off_screen();
void GetRelativeMousePos(int* x, int* y);
void DebugOut(const char* fmt, ...);
bool PointInRect(const Rect& rect, int x, int y);
bool KeyState(int key);

#define ErrorAndQuit(n, ...) { SDL_Log(n, ##__VA_ARGS__); SDL_Quit(); }

#define SAFE_DELETE(n) if (n) { delete n; n = nullptr; }

#define assert_not_reached() assert(0)

namespace Mouse
{
	extern int X;
	extern int Y;
	void UpdatePosition();
}