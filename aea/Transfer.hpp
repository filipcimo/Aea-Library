#ifndef TRANSFER_HPP
#define TRANSFER_HPP

#include "Pointer.hpp"



namespace aea
{
    class Transfer
    {
        public:
            Transfer() = delete;
            Transfer(const Transfer& obj) = delete;
            Transfer(Transfer&& obj) = delete;
            ~Transfer() = delete;

            Transfer& operator=(const Transfer& obj) = delete;
            Transfer& operator=(Transfer&& obj) = delete;

            static void set(const std::uint16_t& size, const std::uint16_t& bufferSize = 65535);
            static void transfer(const std::uint16_t& transfer, const aea::BasicContainer<char>& data);
            static void pick(const std::uint16_t& transfer, aea::BasicContainer<char>& data);
            static std::uint16_t size();
            static std::uint16_t bufferSize();


        private:
            class Buffer
            {
                public:
                    Buffer() = default;
                    ~Buffer();

                    char* buffer = nullptr;
            };


        private:
            static aea::Pointer<Buffer> transfers;
            static std::uint16_t buffersSize;
    };


    aea::Pointer<Transfer::Buffer> Transfer::transfers;
    std::uint16_t Transfer::buffersSize = 0;


    Transfer::Buffer::~Buffer()
    {
        if (buffersSize == 1) { delete buffer; }
        else if (buffersSize > 1) { delete [] buffer; }
    }


    void Transfer::set(const std::uint16_t& size, const std::uint16_t& bufferSize)
    {
        Transfer::buffersSize = bufferSize;

        if (size == 1) { transfers = std::move( aea::Pointer<Buffer>(new Buffer) ); }
        else if (size > 1) { transfers = std::move( aea::Pointer<Buffer>(new Buffer[size], size) ); }

        if (bufferSize == 1)
        {
            for (std::uint16_t i = 0; i < size; ++i) { transfers.at(i).buffer = new char; }
        }

        else if (bufferSize > 1)
        {
            for (std::uint16_t i = 0; i < size; ++i) { transfers.at(i).buffer = new char[buffersSize]; }
        }
    }


    void Transfer::transfer(const std::uint16_t& transfer, const aea::BasicContainer<char>& data)
    {
        if (data.size() >= buffersSize) { strncpy(transfers.at(transfer).buffer, data.get(), buffersSize); }
        else { strncpy(transfers.at(transfer).buffer, data.get(), data.size()); }
    }


    void Transfer::pick(const std::uint16_t& transfer, aea::BasicContainer<char>& data)
    {
        aea::Pointer<char> buffer;

        if (buffersSize == 1) { buffer = std::move( aea::Pointer<char>(new char) ); }
        else if (buffersSize > 1) { buffer = std::move( aea::Pointer<char>(new char[buffersSize], buffersSize) ); }
        
        strncpy(buffer.get(), transfers.at(transfer).buffer, buffersSize);
        data = std::move(buffer);
    }


    std::uint16_t Transfer::size()
    {
        return static_cast<std::uint16_t>(transfers.size());
    }


    std::uint16_t Transfer::bufferSize()
    {
        return buffersSize;
    }
}



#endif  // TRANSFER_HPP
