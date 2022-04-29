
// Copyright (c) 2022 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

// Extended example from https://botan.randombit.net/handbook/api_ref/hash.html

#include <botan/hash.h>
#include <botan/hex.h>
#include <iostream>

int main()
{
    std::unique_ptr<Botan::HashFunction> hash1(Botan::HashFunction::create("SHA-256"));
    std::unique_ptr<Botan::HashFunction> hash2(Botan::HashFunction::create("SHA-384"));
    std::unique_ptr<Botan::HashFunction> hash3(Botan::HashFunction::create("SHA-3"));
    std::vector<uint8_t> buf(2048);


    buf.push_back('a');
    buf.push_back('b');
    buf.push_back('c');
    buf.push_back('d');
    std::cout << "String: '";
    for (char c : buf) std::cout << c;
    std::cout << '\'' << std::endl;
    std::cout << "SHA-256: " << Botan::hex_encode(hash1->final()) << std::endl;
    std::cout << "SHA-384: " << Botan::hex_encode(hash2->final()) << std::endl;
    std::cout << "SHA-3: " << Botan::hex_encode(hash3->final()) << std::endl;
    std::cout << std::endl;


    while (std::cin.good())
    {
        //read STDIN to buffer
        std::cin.read(reinterpret_cast<char*>(buf.data()), buf.size());
        size_t readcount = std::cin.gcount();
        //update hash computations with read data
        hash1->update(buf.data(), readcount);
        hash2->update(buf.data(), readcount);
        hash3->update(buf.data(), readcount);
    }
    std::cout << "String: '";
    for (char c : buf) std::cout << c;
    std::cout << '\'' << std::endl;
    std::cout << "SHA-256: " << Botan::hex_encode(hash1->final()) << std::endl;
    std::cout << "SHA-384: " << Botan::hex_encode(hash2->final()) << std::endl;
    std::cout << "SHA-3: " << Botan::hex_encode(hash3->final()) << std::endl;
    std::cout << std::endl;
    return 0;
}
