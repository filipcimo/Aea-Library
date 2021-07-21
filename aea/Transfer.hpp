#ifndef TRANSFER_HPP
#define TRANSFER_HPP

#include "BasicContainer.hpp"
#include "DArray.hpp"



namespace aea
{
    class Transfer
    {
        public:
            Transfer() = default;
            Transfer(const Transfer& obj) = delete;
            Transfer(Transfer&& obj) = delete;
            ~Transfer() = default;

            Transfer& operator=(const Transfer& obj) = delete;
            Transfer& operator=(Transfer&& obj) = delete;

            static void set(const std::uint16_t& count, const std::uint16_t& buffersSize = 65535);
            static void transfer(const std::uint16_t& pipe, const aea::BasicContainer<char>& data);
            static void pick(const std::uint16_t& pipe, aea::BasicContainer<char>& data);
            static std::uint16_t size();
            static std::uint16_t buffersSize();


        private:
            static aea::DArray<aea::DArray<char>> pipes;
    };


    aea::DArray<aea::DArray<char>> Transfer::pipes;

    void Transfer::set(const std::uint16_t& count, const std::uint16_t& buffersSize)
    {
        pipes.resize(count);

        for (aea::DArray<char>* pipe = nullptr; 
             aea::iterate_front(&pipe, pipes, pipes.first()) != nullptr;)
        {
            pipe->resize(buffersSize);
        }
    }


    void Transfer::transfer(const std::uint16_t& pipe, const aea::BasicContainer<char>& data)
    {
        *static_cast<BasicContainer<char>*>(&pipes.at(pipe)) = data;
    }


    void Transfer::pick(const std::uint16_t& pipe, aea::BasicContainer<char>& data)
    {
        data = pipes.at(pipe);
    }


    std::uint16_t Transfer::size()
    {
        return pipes.size();
    }

    std::uint16_t Transfer::buffersSize()
    {
        if (pipes.size() > 0) { return pipes.at(0).size(); }
        return 0;
    }
}



#endif  // TRANSFER_HPP