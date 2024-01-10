/* -----------------------------------------------------------------------------
Description: Generic factory for runtime object creation
			 Creation of objects of a specified Base class based on a Key 
			 and Arguments:
			 
			 BASE: Represents the base class from which objects will be created.
			 KEY: Represents the key used for mapping and object creation.
			 ARGS: Represents the arguments required for object creation
Author: HRD31
Reviewer: Itzik
Version:
    v0.1 - For mentor approval
----------------------------------------------------------------------------- */
#ifndef __HRD31_FACTORY_HPP__
#define __HRD31_FACTORY_HPP__

#include <functional> //std::function
#include <unordered_map> //std::unordered_map

#include "singleton.hpp"

namespace hrd31
{
//Use instructions:
//KEY must support comparison operators

template <typename BASE, typename KEY, typename ARGS>
class Factory
{
public:
    // CreateFunc is a std::function that creates objects of type BASE.
    using CreateFunc = std::function<std::shared_ptr<BASE>(ARGS)>;
    ~Factory() = default;
    Factory(const Factory& other_) = delete;
    Factory& operator=(const Factory& other_) = delete;

    // If KEY exists, create_func_ is updated
    void Add(const KEY& key_, CreateFunc create_func_);

    // key_: The key used to search for a specific CreateFunc
    // args_: The arguments used when invoking the found CreateFunc
    std::shared_ptr<BASE> Create(const KEY& key_, ARGS args_) const;

private:
    explicit Factory() = default;

    friend class Singleton<Factory>; 

    std::unordered_map<KEY, CreateFunc> m_operations;
};



/* -----------------------------------------------------------------------------
								inline header
----------------------------------------------------------------------------- */
//Add
template <typename BASE, typename KEY, typename ARGS>
void Factory<BASE, KEY, ARGS>::Add(const KEY& key_, CreateFunc create_func_) 
{
    m_operations[key_] = create_func_;
}

//Create
template <typename BASE, typename KEY, typename ARGS>
std::shared_ptr<BASE> Factory<BASE, KEY, ARGS>::Create(const KEY& key_, ARGS args_) const
{
    return m_operations.at(key_)(args_);
}

}//namespace hrd31

#endif //__HRD31_FACTORY_HPP__
