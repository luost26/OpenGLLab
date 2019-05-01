#ifndef UniformBufferRangeManager_hpp
#define UniformBufferRangeManager_hpp

#include <map>
#include <vector>
#include "../UniformBuffer.hpp"

namespace wglfw {

	class UniformBufferRangeManager {
	private:
		std::map<UniformBuffer*, int> rangeAllocation;
		int nextRange;
		static UniformBufferRangeManager * globalManager;
	
		UniformBufferRangeManager() {
			nextRange = 0;
		}

	public:
		static UniformBufferRangeManager * global() {
			if (globalManager == NULL)
				globalManager = new UniformBufferRangeManager;
			return globalManager;
		}

		int getRange(UniformBuffer * object) {
			if (rangeAllocation.count(object) > 0) {
				return rangeAllocation[object];
			} else {
				int range = nextRange++;
				rangeAllocation[object] = range;
				return range;
			}
		}
	};

	UniformBufferRangeManager * UniformBufferRangeManager::globalManager = NULL;

}

#endif
