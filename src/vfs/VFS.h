#ifndef VFS_H
#define VFS_H

#include <functional>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <string>
#include <sstream>
#include <algorithm>
#include <mutex>
#include <memory>
#include <utility>
#include <math.h>
#include <assert.h>
#include <fstream>

namespace xen
{
	#define CLASS_PTR(_class) typedef std::shared_ptr<class _class> _class##Ptr;\
									  typedef std::weak_ptr<class _class> _class##Weak;
};

#endif