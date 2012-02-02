#include <blitz/array.h>
#include <iostream>
#include <vector>
#include <math.h>


// Assigning a matrix to a formula depending on itself
// using tensor
void operatorDiv(unsigned int size){
    using namespace blitz;
    Array<double,2> matrix1(shape(size, size));
    matrix1 = 1.0;
    matrix1 /= 2 * 4;
    std::cout << matrix1; // outputs 2 2 ...
}


// Assigning a matrix to a formula depending on itself
// using tensor
void tensorSelf(unsigned int size){
    using namespace blitz;
    Array<double,2> matrix1(shape(size, size));
    matrix1 = 1;
    matrix1 = matrix1(tensor::i, tensor::j) + matrix1(tensor::i, tensor::j);
    std::cout << matrix1; // outputs 2 2 ...
}

// Combining tensor::i with range
// the tensor::i starts at 0, no matter what the range
void tensorRange(unsigned int size){
    using namespace blitz;
    Array<double,2> matrix1(shape(size, size));
    matrix1 = 1;
    Array<double,2> matrix2(shape(size, size));
    matrix2 = 2;

    matrix2(Range(size/2, size-1), Range::all()) =
            tensor::i;  // Fills with 0, 1, 2, ...

    std::cout << matrix2;
}



// Speeed test of tensor notation
void speedTest(unsigned int size){
    using namespace blitz;
    Array<double,2> matrix(shape(size, size));
    double dKX = 1.1;
    double dKY = 1.2;
    double deltaZ = 0.02;
    double energyTerm = 1;

    matrix = exp(- sqrt(tensor::i * dKX * tensor::i * dKX + tensor::j *dKY * tensor::j * dKY - energyTerm) * deltaZ);
    std::cout << "Finished\n";
    std::cout << matrix(2,3);
}

// Calculate with tensor indices inside parameter, i.e. A(tensor::i * 2)
// This is forbidden.
void indexCalc(unsigned int size){
    using namespace blitz;
    Array<double,1> vecarray(shape(size));
    Array<double,1> vec2array(shape(size*2));
    vec2array = tensor::i;
//  vecarray = vec2array( (tensor::i) * 2);
    std::cout << vecarray;
}

// Fill Array with tensor notation plus questionmark operator
// This does *not* work.
void fillArrayAdvanced(unsigned int size){
    using namespace blitz;
    Array<double,1> vecarray(shape(size));
    //vecarray = (tensor::i % 2 == 0) ? (tensor::i * tensor::i) : (0);
    std::cout << vecarray;
}

// Fill Array with tensor notation
void fillArray(unsigned int size){
    using namespace blitz;
    Array<double,1> vecArray(shape(size));
    vecArray = tensor::i * tensor::i;
    std::cout << vecArray;
}


// Resizing array - works.
// Note: If array deletes data when done you get an error because of 
// double free (from vector and Array)
void resizeArray(unsigned int size) {
    using namespace blitz;
    
    std::vector<double> vec(size,3.4);
    
    Array<double,1> vecArray(&vec[0], shape(size), neverDeleteData);
   
    vecArray(1) = 100; 
    vecArray(0) = 70; 
    cout << vecArray << endl;
    vecArray.resizeAndPreserve(shape(size+1));
    cout << vecArray << endl;
}

// Partially mapping a vector works as well.
void doubleReuseVector(unsigned int size) {
    using namespace blitz;
    
    std::vector<double> vec(size,3.4);
    
    Array<double,1> vecArray(&vec[0], shape(size), neverDeleteData);
    Array<double,1> vecArray2(&vec[size/2], shape(size/2), neverDeleteData);
    vecArray2(1) = 20;
    vec[7]=100;
    cout << vecArray;
}


// This works perfectly. The Array reuses the vector. 
void reuseVector(unsigned int size) {
    using namespace blitz;
    
    std::vector<double> vec(size,3.4);
    Array<double,1> vecArray(&vec[0], shape(size), neverDeleteData);
    vec[7]=100;

    cout << vecArray;
}

void reuseArray(){
   using namespace::blitz; 
   using namespace tensor;
    double data[] = {1,2,3,4,5,6,7,8,9,10,11,12};    
    Array<double,3> dataArray(data, shape(3,2,2), neverDeleteData);
    cout << dataArray;
    std::cout << dataArray(0,0,1);
    cout << dataArray(0,1,0);
    cout << dataArray(1,0,0);

//    Array<double,1> reduced(sum(dataArray(j,i),j));
//    cout << reduced;
}

void reduceTensor() {
    using namespace blitz;
    Array<float,2> A(3,3);
    Array<float,1> B(3);
    A = 0, 1, 2,
    3, 4, 5,
    6, 7, 8;

    cout << A;
    B= sum(A(tensor::i,tensor::j),tensor::j);
    cout << B;
    
    cout << sum(A) << endl          // 36
         << min(A) << endl          // 0
         << count(A >= 4) << endl;  // 5
}

int main() {
    unsigned int size = 20;
//  doubleReuseVector(size);
//  reuseArray();
//  fillArrayAdvanced(size);
//  resizeArray(size);
//  speedTest(270);
//  indexCalc(size);
//  tensorRange(size);
//  tensorSelf(size);
    operatorDiv(size);
    return 0;
}