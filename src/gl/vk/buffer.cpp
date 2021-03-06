#if TZ_VULKAN
#include "gl/vk/buffer.hpp"
#include <cstring>

namespace tz::gl::vk
{
    Buffer::Buffer(BufferType type, BufferPurpose purpose, const LogicalDevice& device, hardware::MemoryModule resource_memory, std::size_t size_bytes):
    buffer(VK_NULL_HANDLE),
    memory(VK_NULL_HANDLE),
    device(&device),
    type(type)
    {
        VkBufferCreateInfo create{};
        create.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        create.size = static_cast<VkDeviceSize>(size_bytes);
        switch(type)
        {
            case BufferType::Vertex:
                create.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
            break;
            case BufferType::Index:
                create.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
            break;
            case BufferType::Staging:

            break;
            case BufferType::Uniform:
                create.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            break;
            default:
                tz_error("Unrecognised BufferType");
            break;
        }
        switch(purpose)
        {
            case BufferPurpose::TransferSource:
                tz_assert(type == BufferType::Staging, "Buffer with TransferSource purpose (Staging Buffer) is incompatible with all BufferTypes aside from Staging");
                create.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
            break;
            case BufferPurpose::TransferDestination:
                // Any buffer can be a transfer destination
                create.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            break;
            case BufferPurpose::NothingSpecial:

            break;
            default:
                tz_error("Unrecognised BufferPurpose");
            break;
        }
        create.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        auto res = vkCreateBuffer(this->device->native(), &create, nullptr, &this->buffer);
        tz_assert(res == VK_SUCCESS, "Failed to create buffer");

        VkMemoryRequirements mem_reqs;
        vkGetBufferMemoryRequirements(this->device->native(), this->buffer, &mem_reqs);
        
        const hardware::Device& phys_dev = *this->device->get_queue_family().dev;
        hardware::MemoryProperties mem_props = phys_dev.get_memory_properties();
        tz_assert(phys_dev == *resource_memory.device, "Resource memory comes from hardware device \"%s\" but the logical device provided is sourced from hardware device \"%s\"", phys_dev.get_properties().deviceName, phys_dev.get_properties().deviceName);

        // TODO: Not something this stupid. Having resource memory per buffer is ludicrous. We need a custom allocator
        VkMemoryAllocateInfo alloc{};
        alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        alloc.allocationSize = mem_reqs.size;
        alloc.memoryTypeIndex = resource_memory.index;

        res = vkAllocateMemory(this->device->native(), &alloc, nullptr, &this->memory);
        tz_assert(res == VK_SUCCESS, "Failed to allocate device memory for buffer");
        vkBindBufferMemory(this->device->native(), this->buffer, this->memory, 0);
    }

    Buffer::Buffer(Buffer&& move):
    buffer(VK_NULL_HANDLE),
    memory(VK_NULL_HANDLE),
    device(nullptr),
    type()
    {
        *this = std::move(move);
    }

    Buffer::~Buffer()
    {
        if(this->buffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(this->device->native(), this->buffer, nullptr);
            this->buffer = VK_NULL_HANDLE;
        }
        if(this->memory != VK_NULL_HANDLE)
        {
            vkFreeMemory(this->device->native(), this->memory, nullptr);
            this->memory = VK_NULL_HANDLE;
        }
    }

    BufferType Buffer::get_type() const
    {
        return this->type;
    }

    void Buffer::write(const void* addr, std::size_t bytes)
    {
        void* data;
        vkMapMemory(this->device->native(), this->memory, 0, bytes, 0, &data);
        std::memcpy(data, addr, bytes);
        vkUnmapMemory(this->device->native(), this->memory);
    }

    Buffer& Buffer::operator=(Buffer&& rhs)
    {
        std::swap(this->buffer, rhs.buffer);
        std::swap(this->memory, rhs.memory);
        std::swap(this->device, rhs.device);
        std::swap(this->type, rhs.type);
        return *this;
    }

    VkBuffer Buffer::native() const
    {
        return this->buffer;
    }
}

#endif // TZ_VULKAN