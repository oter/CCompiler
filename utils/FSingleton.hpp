#ifndef _3FENGINE_SRC_UTILS_F_SINGLETON_HPP_
#define _3FENGINE_SRC_UTILS_F_SINGLETON_HPP_

#include <assert.h>
#include <memory>

#include <boost/core/noncopyable.hpp>

template<typename T>
class FSingleton : boost::noncopyable
{
public:

    virtual ~FSingleton() { }

    static std::shared_ptr<T> GetInstance()
    {
        if (!instance)
        {
            instance = std::shared_ptr<T>(new T());
        }
        assert(instance && "Error creating instance.");
        return instance;
    }

    static void Release()
    {
        assert(instance && "Trying to delete non existing instance.");
        instance.reset();
    }

protected:
    FSingleton() { };

private:
    static std::shared_ptr<T> instance;
};

template<typename T>
std::shared_ptr<T> FSingleton<T>::instance = nullptr;


#endif // _3FENGINE_SRC_UTILS_F_SINGLETON_HPP_
