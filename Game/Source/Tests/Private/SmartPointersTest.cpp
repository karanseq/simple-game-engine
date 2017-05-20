// library includes
#include <algorithm>
#include <vector>

// engine includes
#include "GameObject\GameObject.h"
#include "Logger\Logger.h"
#include "Math\Vec3D.h"
#include "Memory\SharedPointer.h"
#include "Memory\UniquePointer.h"
#include "Memory\WeakPointer.h"

void TestSharedPointerConstructorsAndAssignment()
{
    // block that tests the strong pointer constructors and assignment operators
    {
        // standard constructor...strong pointer to nullptr
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr0;
        LOG("StrongPointer default constructor OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr0.GetStrongCount(), strong_ptr0.GetWeakCount());
#endif

        // standard 'nullptr' constructor
        engine::memory::SharedPointer<engine::math::Vec3D> strong_ptr1 = nullptr;
        LOG("StrongPointer nullptr constructor OK");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr1.GetStrongCount(), strong_ptr1.GetWeakCount());
#endif

        // standard constructor...strong pointer to a game object
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr2(engine::gameobject::GameObject::Create());
        LOG("StrongPointer standard constructor object = game object OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr2.GetStrongCount(), strong_ptr2.GetWeakCount());
#endif

        // copy constructor
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr3(strong_ptr2);
        LOG("StrongPointer copy constructor OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr3.GetStrongCount(), strong_ptr3.GetWeakCount());
#endif

        // move constructor
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr4(std::move(strong_ptr3));
        LOG("StrongPointer move constructor OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr4.GetStrongCount(), strong_ptr4.GetWeakCount());
#endif

        // copy assignment
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr5;
        strong_ptr5 = strong_ptr4;
        LOG("StrongPointer copy assignment operator OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr5.GetStrongCount(), strong_ptr5.GetWeakCount());
#endif

        // move assignment
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr6;
        strong_ptr6 = std::move(strong_ptr5);
        LOG("StrongPointer move assignment operator OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr6.GetStrongCount(), strong_ptr6.GetWeakCount());
#endif

        // weak pointer (non-nullptr) constructor
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr1(strong_ptr6);
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr7(weak_ptr1);
        LOG("StrongPointer from weak pointer to non-nullptr constructor OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr7.GetStrongCount(), strong_ptr7.GetWeakCount());
#endif

        // weak pointer (nullptr) constructor
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr2;
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr8(weak_ptr2);
        LOG("StrongPointer from weak pointer to nullptr constructor OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr8.GetStrongCount(), strong_ptr8.GetWeakCount());
#endif

        // expired weak pointer constructor
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr3;
        {
            engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr0(engine::gameobject::GameObject::Create());
            weak_ptr3 = strong_ptr0;
        }
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr9(weak_ptr3);
        LOG("StrongPointer from expired weak pointer %s", (strong_ptr9) ? "NOT OK!" : "OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr9.GetStrongCount(), strong_ptr9.GetWeakCount());
#endif

        // assignment to nullptr
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr10(engine::gameobject::GameObject::Create());
        strong_ptr10 = nullptr;
        LOG("StrongPointer assignment to nullptr OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr10.GetStrongCount(), strong_ptr10.GetWeakCount());
#endif

        // assignment to a pointer
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr11(nullptr);
        strong_ptr11 = engine::gameobject::GameObject::Create();
        LOG("StrongPointer assignment to game object OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr11.GetStrongCount(), strong_ptr11.GetWeakCount());
#endif
    }

    LOG("------------------------------");
}

void TestSharedPointerOperators()
{
    // block that tests remaining strong pointer operators
    {
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr1(engine::gameobject::GameObject::Create());

        // member access operator
        strong_ptr1->SetPosition(strong_ptr1->GetPosition() + engine::math::Vec3D(10.0f, 10.0f, 10.0f));

        // dereference operator
        const engine::math::Vec3D& vec = (*strong_ptr1).GetPosition();
        LOG("Member access operator & dereference operator test vec:%f, %f, %f", vec.x(), vec.y(), vec.z());

        // bool operator
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr2;
        LOG("Strong pointer 2 bool operator:%s", (strong_ptr2) ? "true" : "false");

        // equality check with nullptr
        LOG("Strong pointer 2 == nullptr:%s", (strong_ptr2 == nullptr) ? "true" : "false");

        // inequality check with nullptr
        LOG("Strong pointer 2 != nullptr:%s", (strong_ptr2 != nullptr) ? "true" : "false");

        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr3(engine::gameobject::GameObject::Create());
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr4 = strong_ptr1;

        // equality check with another pointer
        LOG("Strong pointer 3 == Strong pointer 4:%s", (strong_ptr3 == strong_ptr4) ? "true" : "false");

        // inequality check with another pointer
        LOG("Strong pointer 4 == Strong pointer 1:%s", (strong_ptr4 == strong_ptr1) ? "true" : "false");
    }

    LOG("------------------------------");
}

void TestWeakPointer()
{
    // block that tests weak pointer constructors & operators
    {
        // standard constructor
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr0;
        LOG("WeakPointer standard constructor OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr0.GetStrongCount(), weak_ptr0.GetWeakCount());
#endif

        // strong pointer constructor
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr0(engine::gameobject::GameObject::Create());
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr1 = strong_ptr0;
        LOG("WeakPointer strong pointer constructor OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr1.GetStrongCount(), weak_ptr1.GetWeakCount());
#endif

        // copy constructor
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr2 = weak_ptr1;
        LOG("WeakPointer copy constructor OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr2.GetStrongCount(), weak_ptr2.GetWeakCount());
#endif

        // move constructor
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr3(std::move(weak_ptr1));
        LOG("WeakPointer move constructor OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr3.GetStrongCount(), weak_ptr3.GetWeakCount());
#endif

        // copy assignment
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr4;
        weak_ptr4 = weak_ptr3;
        LOG("WeakPointer copy assignment OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr4.GetStrongCount(), weak_ptr4.GetWeakCount());
#endif

        // move assignment
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr5;
        weak_ptr5 = std::move(weak_ptr4);
        LOG("WeakPointer move assignment OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr5.GetStrongCount(), weak_ptr5.GetWeakCount());
#endif

        // strong pointer assignment
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr6;
        weak_ptr6 = strong_ptr0;
        LOG("WeakPointer strong pointer assignment OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", weak_ptr6.GetStrongCount(), weak_ptr6.GetWeakCount());
#endif

        // non-expired weak pointer lock function
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr1 = weak_ptr6.Lock();
        LOG("WeakPointer non-expired Lock function OK!");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr1.GetStrongCount(), strong_ptr1.GetWeakCount());
#endif

        // expired weak pointer lock function
        engine::memory::WeakPointer<engine::gameobject::GameObject> weak_ptr7;
        {
            engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr2(engine::gameobject::GameObject::Create());
            weak_ptr7 = strong_ptr2;
        }
        engine::memory::SharedPointer<engine::gameobject::GameObject> strong_ptr2 = weak_ptr7.Lock();
        LOG("WeakPointer expired Lock function %s", (strong_ptr2) ? "NOT OK!" : "OK");
#ifdef BUILD_DEBUG
        LOG("StrongCount:%ld  WeakCount:%ld", strong_ptr2.GetStrongCount(), strong_ptr2.GetWeakCount());
#endif
    }

    LOG("------------------------------");
}

void TestUniquePointer()
{
    // block that tests unique pointer constructor & operators
    engine::memory::UniquePointer<engine::math::Vec3D> unique_ptr1;
    {
        //engine::memory::UniquePointer<engine::gameobject::GameObject> unique_ptr2(unique_ptr1);   // copy not allowed

        engine::memory::UniquePointer<engine::math::Vec3D> unique_ptr2(new engine::math::Vec3D());
        LOG("UniquePointer standard constructor %s", (unique_ptr2) ? "OK!" : "NOT OK!");

        engine::memory::UniquePointer<engine::math::Vec3D> unique_ptr3(std::move(unique_ptr2));
        LOG("UniquePointer move constructor %s", (!unique_ptr2 && unique_ptr3) ? "OK!" : "NOT OK!");

        unique_ptr1 = std::move(unique_ptr3);
        LOG("UniquePointer move assignment operator %s", (!unique_ptr3 && unique_ptr1) ? "OK!" : "NOT OK!");

        LOG("UniquePointer equality check %s", (unique_ptr2 == unique_ptr3 && unique_ptr1 != unique_ptr2) ? "OK!" : "NOT OK!");
    }

    LOG("------------------------------");
}

void TestSmartPointersWithVector()
{
    // create a vector to hold some shared pointers
    std::vector<engine::memory::SharedPointer<engine::gameobject::GameObject>> vec1;
    std::vector<engine::memory::SharedPointer<engine::gameobject::GameObject>> vec3;

    // fill that vector up
    const size_t rand_count = rand() % 25;
    for (size_t i = 0; i < rand_count; ++i)
    {
        engine::math::Transform transform;
        transform.SetPosition(engine::math::Vec3D(i * 10.0f));
        vec1.push_back(engine::gameobject::GameObject::Create(engine::math::AABB::ZERO, transform));
    }
    LOG("Created the first vector with %zu shared pointers", vec1.size());

    LOG("Beginning a random copy of shared pointers between vectors:");
    const size_t iteration_count = rand() % 20;
    for (size_t i = 0; i < iteration_count; ++i)
    {
        // create another vector
        std::vector<engine::memory::SharedPointer<engine::gameobject::GameObject>> vec2(vec1);

        // shuffle elements
        std::random_shuffle(vec2.begin(), vec2.end());

        const size_t num_elements_to_add = rand_count / 3;
        vec3.resize(vec3.size() + num_elements_to_add);

        // move some of the elements
        std::move_backward(vec2.begin(), vec2.begin() + num_elements_to_add, vec3.end());
    }
    LOG("Finished %zu iterations of random copy", iteration_count);

#ifdef BUILD_DEBUG
    LOG("Displaying the ref count of shared pointers in the first vector:");
    for (auto i : vec1)
    {
        LOG("REF_COUNT:%ld", i.GetStrongCount());
    }
#endif
}

void TestSmartPointers()
{
    LOG("-------------------- Running Smart Pointers Test --------------------");
        
    TestSharedPointerConstructorsAndAssignment();
    TestSharedPointerOperators();
    TestWeakPointer();
    TestUniquePointer();

    TestSmartPointersWithVector();

    LOG("-------------------- Finished Smart Pointers Test --------------------");
}