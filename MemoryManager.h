#include "IMemoryManager.h"
#include <map>
#include <vector>
class MemoryManager : public IMemoryManager {
	struct FreeStore {
		FreeStore *next;
	};
	void expandPoolSize(size_t size);
	void cleanUp();
	FreeStore* freeStoreHead;

	size_t chunkSize;
public:
	MemoryManager() {
		freeStoreHead = 0;
	}
	virtual ~MemoryManager() {
		cleanUp();
	}
	// client must set nr of preallocations
	int _NODECOUNT = 0; 

	virtual void  setNumberOfAllocations(size_t);
	virtual void* internalAllocate(size_t);
	virtual void  free(void*);
};

// map for multiple instances
extern std::map<uint32_t, MemoryManager*> g_MemoryMap;

// simplified allocation 
template<class T>
static T* newMemory(size_t size, uint32_t id) {
	return g_MemoryMap[id]->allocate<T>(size);
}

template<class T>
static T* newMemory(uint32_t id) {
	return g_MemoryMap[id]->allocate<T>(sizeof(T));
}

static void freeMemory(void* ptr, uint32_t id) {
	g_MemoryMap[id]->free(ptr);
}