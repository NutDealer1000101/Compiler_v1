#pragma once

class ArenaAllocator {
public:
	ArenaAllocator(size_t bytes)
		:
		size(bytes)
	{
		buffer = new char[size];
		offset = buffer;
		end = buffer + size;
	}

	template<typename T>
	T* alloc() {
		T* address = new(reinterpret_cast<T*>(offset)) T();	//Default construct at the address
		offset += sizeof(T);								//Increment the offset pointer

		//Check if offset is out of bounds
		//If we get to the point this is a problem, we should implement a way to allocate more memory at runtime
		if (offset > end) {									
			std::cerr << "[ARENA] Shits too small, need more memory" << std::endl;
			exit(EXIT_FAILURE);
		}

		return address;										//Return address of newly created object
	}

	ArenaAllocator(const ArenaAllocator& other) = delete;
	ArenaAllocator operator=(const ArenaAllocator& other) = delete;

	~ArenaAllocator() {
		delete[] buffer;
	}
private:
	size_t size;
	char* buffer = nullptr;
	char* offset = nullptr;
	char* end = nullptr;
};