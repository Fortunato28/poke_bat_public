#include <iostream>
#include <vector>

using namespace std;

void print_vec(std::vector<char> to_print)
{
    for(auto elem: to_print)
    {
        cout << elem << endl;
    }
}

void request(std::vector<char>* vec)
{
    std::vector<char> buf(500, 'y');

    cout << vec->capacity() << endl;

    //vec->insert(vec->end(), buf.begin(), buf.end());
    for(auto elem: buf)
    {
        vec->push_back(elem);
    }

    cout << vec->capacity() << endl;
}

int main()
{
    cout << "Hello, poke_bat!" << endl;
    std::vector<char> vec = {'x', 'x','x'};

    print_vec(vec);
    char * ptr = vec.data();
    cout << &ptr << endl;

    request(&vec);

    //print_vec(vec);
    ptr = vec.data();
    cout << &ptr << endl;
}
