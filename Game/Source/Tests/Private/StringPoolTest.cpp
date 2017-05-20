// library includes
#include <stdlib.h>
#include <vector>

// engine includes
#include "Assert\Assert.h"
#include "Data\HashedString.h"
#include "Data\PooledString.h"
#include "Data\StringPool.h"
#include "Logger\Logger.h"
#include "Memory\BlockAllocator.h"

char* MakeRandomWord(size_t i_length)
{
    ASSERT(i_length > 0);

    char* word = reinterpret_cast<char*>(engine::memory::BlockAllocator::GetDefaultAllocator()->Alloc(i_length + 1));
    for (size_t i = 0; i < i_length; ++i)
    {
        *(word + i) = 65 + rand() % 26;
    }
    *(word + i_length - 1) = '\0';

    return word;
}

void TestStringPool()
{
    LOG("-------------------- Running StringPool Test --------------------");

    // test the pool
    const char* search_string = "sentient";
    const char* added_string = engine::data::StringPool::Get()->Add(search_string);
    const char* found_string = engine::data::StringPool::Get()->Find(search_string);
    found_string = engine::data::StringPool::Get()->Find(added_string);
    ASSERT(found_string == added_string);

    added_string = engine::data::StringPool::Get()->Add("transcendant");
    found_string = engine::data::StringPool::Get()->Find("transcendant");
    found_string = engine::data::StringPool::Get()->Find(added_string);
    ASSERT(found_string == added_string);

    // test pooled string, its constructors & operators
    // default constructor, new string
    engine::data::PooledString ps0("thomas_was_alone");
    // default constructor, existing string
    engine::data::PooledString ps1("transcendant");
    // copy constructor
    engine::data::PooledString ps2(ps1);
    // assignment operator
    ps2 = ps0;
    ASSERT(ps0 == ps2);

    // test hashed string, its constructors and operators
    // default constructor
    engine::data::HashedString hs1("transcendant");
    // copy constructor
    engine::data::HashedString hs2(hs1);
    // pooled string constructor
    engine::data::HashedString hs3(ps1);
    ASSERT(hs1 == hs3);
    // assignment operator
    hs1 = hs3;
    ASSERT(hs1 == hs3);
    
    LOG("-------------------- Finished StringPool Test --------------------");
}