// #include <iostream>
// #include <iomanip>

// int main() {
//   std::cout << std::setw(40) << "Hello, world!" << std::endl;
//   return 0;
// }


#include <iostream> 
#include <unistd.h> // for sleep function
using namespace std; 

// Function to creating loading bar 
void loadingBar() 
{ 
  // Initialize char for printing loading bar 
  char a = 177, b = 219; 

  cout << "\n\n\n\n";
  cout << "\n\n\n\n\t\t\t\t\tLoading...\n\n";
  cout << "\t\t\t\t\t";

  // Print initial loading bar 
  for (int i = 0; i < 26; i++) 
    cout << a;

  // Set the cursor again starting point of loading bar 
  cout << "\r";
  cout << "\t\t\t\t\t";

  // Print loading bar progress 
  for (int i = 0; i < 26; i++) { 
    cout << b;

    // Sleep for 1 second 
    usleep(1000000); // usleep takes microseconds, so 1 second = 1,000,000 microseconds
    cout.flush(); // ensure that the output is printed immediately
  } 
} 

// Driver Code 
int main() 
{ 
  // Function Call 
  loadingBar(); 
  return 0; 
}