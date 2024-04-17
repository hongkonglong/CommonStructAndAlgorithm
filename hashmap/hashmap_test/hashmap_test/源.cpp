#include <iostream>
#include <functional>
#include <unordered_map>

class Person
{
public:
    std::string name;
    int age;
    Person(std::string str, int g)
    {
        name = str;
        age = g;
    }
    bool operator==(const Person& person) const
    {
        return person.name == name && person.age == age;
    }
    friend inline std::ostream& operator<< (std::ostream& os, const Person& person)
    {
        os << "name: " << person.name << ", age: " << person.age;
        return os;
    }
};

class Man
{
public:
    std::string name;
    int age;
    std::string number;
    Man(std::string str, int a, std::string num)
    {
        name = str;
        age = a;
        number = num;
    }
    /*   bool operator==(const Man& man) const
       {
           return man.name == name && man.age == age && man.number == number;
       }*/
    friend inline std::ostream& operator<<(std::ostream& os, const Man& man)
    {
        os << "name: " << man.name << ", age: " << man.age << ", number: " << man.number << std::endl;
        return os;
    }
};

namespace std
{
    template <>
    class hash<Man>
    {
    public:
        size_t operator()(const Man& man) const
        {
            return hash<string>()(man.name) ^ hash<int>()(man.age) ^ hash<string>()(man.number);
        }
    };
    template <>
    class equal_to<Man>
    {
    public:
        bool operator()(const Man& man1, const Man& man2) const
        {
            return man1.age == man2.age && man1.name == man2.name && man1.number == man2.number;
        }
    };
}

size_t hash_person(const Person& person)
{
    return std::hash<std::string>()(person.name) ^ std::hash<int>()(person.age);
}

struct has_person1
{
    size_t operator()(const Person& per) const
    {
        return std::hash<std::string>()(per.name) ^ std::hash<int>()(per.age);
    }
};

int main()
{
    std::unordered_map < Person, int, std::function < size_t(const Person&)>> test_map(10, hash_person);
    test_map[Person("hong", 29)] = 10010;
    test_map[Person("wang", 27)] = 10011;
    for (auto items : test_map)
    {
        std::cout << items.first << ": " << items.second << std::endl;
    }
    std::unordered_map < Person, int, decltype(&hash_person)> test_map2(10, hash_person);
    test_map2[Person("liu", 27)] = 10086;
    test_map2[Person("yu", 28)] = 10001;
    for (auto items : test_map2)
    {
        std::cout << items.first << ": " << items.second << std::endl;
    }
    std::unordered_map<Person, int, has_person1> test_map3;
    test_map3[Person("zhang", 27)] = 10080;
    test_map3[Person("zhao", 27)] = 10090;
    for (auto items : test_map3)
    {
        std::cout << items.first << ": " << items.second << std::endl;
    }
    std::unordered_map<Man, int> test_map4;
    test_map4[Man("zzz", 22, "123")] = 1;
    test_map4[Man("xxx", 23, "456")] = 2;
    test_map4[Man("zzz", 23, "123")] = 3;
    for (auto items : test_map4)
    {
        std::cout << items.first << ": " << items.second << std::endl;
    }
    return 0;
}
