#include<iostream>
using namespace std;

class base
{
    public:
    virtual void fun() const = 0;  
};

class derived1 : public base
{
    public:
        void fun() const override;
};

class derived2 : public base
{
    public:
        void fun() override;
};

