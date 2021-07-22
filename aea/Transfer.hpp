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

            static void set(const std::uint16_t& size, const std::uint16_t& buffersSize = 65535);
            static void transfer(const std::uint16_t& transfer, const aea::BasicContainer<char>& data);
            static void pick(const std::uint16_t& transfer, aea::BasicContainer<char>& data);
            static std::uint16_t size();
            static std::uint16_t buffersSize();


        private:
            static aea::DArray<aea::DArray<char>> transfers;
    };


    aea::DArray<aea::DArray<char>> Transfer::transfers;

    void Transfer::set(const std::uint16_t& size, const std::uint16_t& buffersSize)
    {
        transfers.resize(size);

        for (aea::DArray<char>* transfer = nullptr; 
             aea::iterate_front(&transfer, transfers, transfers.first()) != nullptr;)
        {
            transfer->resize(buffersSize);
        }
    }


    void Transfer::transfer(const std::uint16_t& transfer, const aea::BasicContainer<char>& data)
    {
        if (data.size() >= transfers.at(transfer).size()) { strncpy(transfers.at(transfer).get(), data.get(), transfers.at(transfer).size()); }
        else { strncpy(transfers.at(transfer).get(), data.get(), data.size()); }
    }


    void Transfer::pick(const std::uint16_t& transfer, aea::BasicContainer<char>& data)
    {
        data = transfers.at(transfer);
    }


    std::uint16_t Transfer::size()
    {
        return transfers.size();
    }


    std::uint16_t Transfer::buffersSize()
    {
        if (transfers.size() > 0) { return transfers.at(0).size(); }
        return 0;
    }
}



#endif  // TRANSFER_HPP