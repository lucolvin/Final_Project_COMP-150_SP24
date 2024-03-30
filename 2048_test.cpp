
//   const auto bar_pattern_list = {std::make_tuple("┌", "┬", "┐"),
//                                  std::make_tuple("├", "┼", "┤"),
//                                  std::make_tuple("└", "┴", "┘")};

#include<iostream>
using namespace std;

int main()
{
    cout << "Test" << endl;
    cout << "┌" << "─" << "┬" << "┐" << endl;
    cout << "├" << "┼" << "┤" << endl;
    cout << "└" << "┴" << "┘" << endl;
    return 0;
}