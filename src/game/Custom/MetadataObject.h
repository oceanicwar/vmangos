#ifndef _CUSTOM_METADATA_OBJECT_H
#define _CUSTOM_METADATA_OBJECT_H

#include <memory>
#include <unordered_map>
#include <string>

class MetadataValueBase 
{
public:
    virtual ~MetadataValueBase() {} // Ensures polymorphism by adding a vtable.
};

template <typename T>
class MetadataValueContainer : public MetadataValueBase
{
public:
    MetadataValueContainer(T val) : value(val) { }
    const T* get() const { return &value; }

private:
    T value;
};

class MetadataObject
{
public:
    template <typename T>
    void SetMetadata(const std::string& key, T value)
    {
        metadata[key] = std::make_shared<MetadataValueContainer<T>>(value);
    }

    template <typename T>
    const T* GetMetadata(const std::string& key) const
    {
        auto it = metadata.find(key);
        if (it == metadata.end()) {
            return nullptr;
        }

        MetadataValueContainer<T>* container = dynamic_cast<MetadataValueContainer<T>*>(it->second.get());

        if (!container)
        {
            return nullptr;
        }

        return container->get();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<MetadataValueBase>> metadata;
};

#endif