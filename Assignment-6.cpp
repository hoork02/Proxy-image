#include<iostream>
#include<algorithm>
#define HEAP_SIZE 100 //256 MB
using namespace std;


template <class T>
class minHeap
{
private:
    T* arr;
    int _size;
   
    
    int getLeftChild(int parent)
    {
        int lc = 2 * parent + 1;
        return(lc < _size) ? lc : -1;
    
    }// return left child of parent
    // return -1 if it doesn't exist
    int getRightChild(int parent)
    {
        int rc = 2 * parent + 2;
        return(rc < _size) ? rc : -1;
    
    }// return right child of parent
    // return -1 if it doesn't exist
    int getParent(int child)
    {
        if (child == 0 || child >= _size) {
            return -1;
        }
        return (child - 1) / 2;
    }// return parent of child
    // return -1 if it doesn't exist
    void heapifyUp(int node)
    {   
        while (node > 0 && arr[node] < arr[getParent(node)]) {
            swap(arr[node], arr[getParent(node)]);
            node = getParent(node);
        }
    
    }// restore the heap property
    // compare children with parent and swap if disruption exists
    void heapifyDown(int node)
    {
        int minInd = node;
        int leftC = getLeftChild(node);
        int rightC = getRightChild(node);

        if (leftC != -1 && arr[leftC] < arr[minInd]) {
            minInd = leftC;
        }

        if (rightC != -1 && arr[rightC] < arr[minInd]) {
            minInd = rightC;
        }

        if (node != minInd) {
            swap(arr[node], arr[minInd]);
            heapifyDown(minInd);
        }
    }// restore the heap property
    // compare parent with children and swap if disruption exists

public:
    const int incapacity = 20;
    int capacity;
    minHeap()
    {
        capacity = incapacity;
        arr = new T[capacity];
        _size = 0;
    
    }// make an empty heap
    int getCapacity() const {
        return capacity;
    }
    void heapify(T* _arr, int _capacity)
    {   
        if (_capacity > capacity) {
            delete[] arr;
            capacity = _capacity;
            arr = new T[capacity];
        }

        _size = _capacity;
        for (int i = 0; i < _capacity; i++) {
            arr[i] = _arr[i];
        }

        for (int i = _size / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    
    }// make a heap from given array
    minHeap(T* _arr, int _capacity)
    {
        capacity = _capacity;
        arr = new T[capacity];
        _size = _capacity;

        for (int i = 0; i < _capacity; i++) {
            arr[i] = _arr[i];
        }

        for (int i = _size / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }// initialize through heapify
    T getMin()
    {
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }
        return arr[0];
    }// return the element with minimum key value
    T extractMin()
    {
        if (empty()) {
            throw std::out_of_range("Heap is empty.");
        }

        T minValue = arr[0];
        arr[0] = arr[_size - 1];
        _size--;
        heapifyDown(0);

        return minValue;
    
    }// return and pop the element with minumum key value
    void insert(T _key)
    {
        if (_size >= capacity) {
            // Handle resizing of the array if needed
            capacity *= 2;
            T* newArr = new T[capacity];
            for (int i = 0; i < _size; i++) {
                newArr[i] = arr[i];
            }
            delete[] arr;
            arr = newArr;
        }

        arr[_size] = _key;
        _size++;
        heapifyUp(_size - 1);
    
    }// insert the data into heap based on key value
    void erase(T _key)
    {
        int index = -1;
        for (int i = 0; i < _size; i++) {
            if (arr[i] == _key) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            throw std::invalid_argument("Element not found in the heap.");
        }

        arr[index] = arr[_size - 1];
        _size--;
        heapifyDown(index);
    
    }// delete the data based on key value
    bool empty()
    {
        return (_size == 0);
    
    
    
    }// tells whether heap is empty or not
    int size()
    {
        return _size;
    }// return number of elements
    void print()
    {
        for (int i = 0; i < _size; i++) {
            std::cout << arr[i] << " ";
        }
        std::cout << std::endl;
    
    }// print the heap in whatever order you like
    // by order it means, either preorder, inorder, postorder
    // or in ascending/descending order
    static void sort(T* _arr, int _capacity)
    {
        minHeap heap;
        heap.heapify(_arr, _capacity);

        for (int i = _capacity - 1; i >= 0; i--) {
            _arr[i] = heap.extractMin();
        }
    
    
    }
    minHeap& operator=(const minHeap& other) {
        if (this != &other) {
            // Perform a deep copy of the data
            delete[] arr;
            capacity = other.capacity;
            _size = other._size;
            arr = new T[capacity];
            for (int i = 0; i < _size; i++) {
                arr[i] = other.arr[i];
            }
        }
        return *this;
    }
    // sort the array in descending order using heap sort
    ~minHeap()
    {
        delete[] arr;
    
    }// deallocate the memory
};

template <class T>
class maxHeap
{
private:
    T* arr;
    int _size;
    int capacity;

    int getLeftChild(int parent)
    {
        int leftC = 2 * parent + 1;
        return (leftC < _size) ? leftC : -1;
    
    
    }// return left child of parent
    // return -1 if it doesn't exist
    int getRightChild(int parent)
    {
        int rc = 2 * parent + 2;
        return (rc < _size) ? rc : -1;
    }
    // return right child of parent
    // return -1 if it doesn't exist
    int getParent(int child)
    {
        if (child == 0 || child >= _size) {
            return -1;
        }
        return (child - 1) / 2;
    }
    // return parent of child
    // return -1 if it doesn't exist
    void heapifyUp(int node)
    {
        while (node > 0 && arr[node] > arr[getParent(node)]) {
            swap(arr[node], arr[getParent(node)]);
            node = getParent(node);
        }
    }// restore the heap property
    // compare children with parent and swap if disruption exists
    void heapifyDown(int node)
    {
        int maxIndex = node;
        int leftChild = getLeftChild(node);
        int rightChild = getRightChild(node);

        if (leftChild != -1 && arr[leftChild] > arr[maxIndex]) {
            maxIndex = leftChild;
        }

        if (rightChild != -1 && arr[rightChild] > arr[maxIndex]) {
            maxIndex = rightChild;
        }

        if (node != maxIndex) {
            swap(arr[node], arr[maxIndex]);
            heapifyDown(maxIndex);
        }

    
    
    }// restore the heap property
    // compare parent with children and swap if disruption exists

public:
    const int ic = 20;//intial capacity
    maxHeap()
    {
        capacity = ic;
        arr = new T[capacity];
        _size = 0;
    
    
    }// make an empty heap
    void heapify(T* _arr, int _capacity)
    {
        if (_capacity > capacity) {
            delete[] arr;
            capacity = _capacity;
            arr = new T[capacity];
        }

        _size = _capacity;
        for (int i = 0; i < _capacity; i++) {
            arr[i] = _arr[i];
        }

        for (int i = _size / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    }// make a heap from given array
    maxHeap(T* _arr, int _capacity)
    {
        capacity = _capacity;
        arr = new T[capacity];
        _size = _capacity;

        for (int i = 0; i < _capacity; i++) {
            arr[i] = _arr[i];
        }

        for (int i = _size / 2 - 1; i >= 0; i--) {
            heapifyDown(i);
        }
    
    
    }// initialize through heapify
    T getMax()
    {
        if (empty()) {
            throw out_of_range("Heap is empty.");
        }
        return arr[0];
    
    }// return the element with maximum key value
    T extractMax()
    {
        if (empty()) {
            throw out_of_range("Heap is empty.");
        }

        T maxValue = arr[0];
        arr[0] = arr[_size - 1];
        _size--;
        heapifyDown(0);

        return maxValue;
    
    }// return and pop the element with maximum key value
    void insert(T _key)
    {
        if (_size >= capacity) {
            // Handle resizing of the array if needed
            capacity *= 2;
            T* newArr = new T[capacity];
            for (int i = 0; i < _size; i++) {
                newArr[i] = arr[i];
            }
            delete[] arr;
            arr = newArr;
        }

        arr[_size] = _key;
        _size++;
        heapifyUp(_size - 1);
    
    }// insert the data into heap based on key value
    void erase(T _key)
    {   
        int index = -1;
        for (int i = 0; i < _size; i++) {
            if (arr[i] == _key) {
                index = i;
                break;
            }
        }

        if (index == -1) {
            throw std::invalid_argument("Element not found in the heap.");
        }

        arr[index] = arr[_size - 1];
        _size--;
        heapifyDown(index);
    
    
    }// delete the data based on key value
    bool empty()
    {
        return(_size == 0);
    
    }// tells whether heap is empty or not
    int size()
    {
        return (_size);
    }// return number of elements
    void print()
    {
        for (int i = 0; i < _size; i++) {
            cout << arr[i] << " ";
        }
        cout << std::endl;
    
    }// print the heap in whatever order you like
    // by order it means, either preorder, inorder, postorder
    // or in ascending/descending order
    static void sort(T* _arr, int _capacity)
    {
        maxHeap heap;
        heap.heapify(_arr, _capacity);

        for (int i = _capacity - 1; i >= 0; i--) {
            _arr[i] = heap.extractMax();
        }
    
    }
    // sort the array in ascending order using heap sort
    ~maxHeap()
    {
        delete[] arr;
    
    }// deallocate the memory
};
int main4() {
    // Create a max heap from an array
    int arr[] = { 4, 10, 3, 5, 1 };
    int arrSize = sizeof(arr) / sizeof(arr[0]);

    maxHeap<int> heap(arr, arrSize);

    // Print the initial max heap
    std::cout << "Max Heap: ";
    heap.print();

    // Insert an element into the max heap
    heap.insert(8);
    std::cout << "After inserting 8: ";
    heap.print();

    // Extract the maximum element
    int maxElement = heap.extractMax();
    std::cout << "Extracted Max Element: " << maxElement << std::endl;
    std::cout << "Max Heap after extraction: ";
    heap.print();

    // Erase an element from the max heap
    int elementToErase = 4;
    heap.erase(elementToErase);
    std::cout << "After erasing " << elementToErase << ": ";
    heap.print();

    // Sort an array using heap sort
    int unsortedArr[] = { 7, 2, 9, 1, 6, 8, 3, 4, 5 };
    int unsortedArrSize = sizeof(unsortedArr) / sizeof(unsortedArr[0]);

    maxHeap<int>::sort(unsortedArr, unsortedArrSize);
    std::cout << "Sorted Array (Ascending Order): ";
    for (int i = 0; i < unsortedArrSize; i++) {
        std::cout << unsortedArr[i] << " ";
    }
    std::cout << std::endl;


    return 0;
}

int main5()
{

   
    // Create a max heap from an array
    int ar[] = { 4, 10, 3, 5, 1 };
    int arrSiz = sizeof(ar) / sizeof(ar[0]);

    minHeap<int> heap(ar, arrSiz);

    // Print the initial max heap
    std::cout << "Min Heap: ";
    heap.print();

    // Insert an element into the max heap
    heap.insert(8);
    std::cout << "After inserting 8: ";
    heap.print();

    // Extract the maximum element
    int maxElement = heap.extractMin();
    std::cout << "Extracted Min Element: " << maxElement << std::endl;
    std::cout << "Max Heap after extraction: ";
    heap.print();

    // Erase an element from the max heap
    int elementToErase = 4;
    heap.erase(elementToErase);
    std::cout << "After erasing " << elementToErase << ": ";
    heap.print();

    // Sort an array using heap sort
    int unsortedArr[] = { 7, 2, 9, 1, 6, 8, 3, 4, 5 };
    int unsortedArrSize = sizeof(unsortedArr) / sizeof(unsortedArr[0]);

    maxHeap<int>::sort(unsortedArr, unsortedArrSize);
    std::cout << "Sorted Array (Ascending Order): ";
    for (int i = 0; i < unsortedArrSize; i++) {
        std::cout << unsortedArr[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}


class Heap
{
private:
    char arr[HEAP_SIZE];
    minHeap<pair<int, char*>> freeMemory;
    void mergeFreeSegments()
    {
        minHeap<std::pair<size_t, char*>> mergedSegments; // Temporary minHeap to store merged segments
        size_t mergedSize = 0;
        char* mergedStart = nullptr;

        while (!freeMemory.empty())
        {
            auto currentSegment = freeMemory.extractMin();
            char* currentStart = currentSegment.second;
            size_t currentSize = currentSegment.first;

            if (mergedStart == nullptr)
            {
                // Initialize the merged segment with the first segment from freeMemory
                mergedStart = currentStart;
                mergedSize = currentSize;
            }
            else if (currentStart == mergedStart + mergedSize)
            {
                // Current segment is contiguous with the merged segment, merge them
                mergedSize += currentSize;
            }
            else
            {
                // Current segment is not contiguous, add the merged segment to the temporary minHeap
                mergedSegments.insert(std::make_pair(mergedSize, mergedStart));

                // Initialize the merged segment with the current segment
                mergedStart = currentStart;
                mergedSize = currentSize;
            }
        }

        if (mergedStart != nullptr)
        {
            // Add the last merged segment to the temporary minHeap
            mergedSegments.insert(std::make_pair(mergedSize, mergedStart));
        }

        // Update freeMemory with the merged segments
        //freeMemory = mergedSegments;
    }
public:
    void* ITUN(size_t size)
    {
        if (size == 0)
            return nullptr;

        // Search the minHeap to find an available segment
        while (!freeMemory.empty())
        {
            auto segment = freeMemory.getMin();
            if (segment.first >= size)
            {
                freeMemory.erase(segment); // Remove the segment from the minHeap

                char* ptr = segment.second;

                if (segment.first > size)
                {
                    // Split the segment and update the freeMemory
                    freeMemory.insert(make_pair(segment.first - size, ptr + size));
                }

                return static_cast<void*>(ptr);
            }
            freeMemory.extractMin(); // Remove the invalid segment and continue searching
        }

        throw std::runtime_error("Out of memory");
    
    
    }
    void  ITUD(void* ptr)
    {
        if (ptr == nullptr)
            return;

        char* charPtr = static_cast<char*>(ptr);

        if (charPtr >= arr && charPtr < (arr + HEAP_SIZE))
        {
            size_t size = freeMemory.capacity;
            char* endPtr = charPtr + size;

            // Iterate through the freeMemory to merge and insert the new free segment
            bool merged = false;
            while (!freeMemory.empty())
            {
                auto segment = freeMemory.getMin();
                char* segmentEnd = segment.second + segment.first;

                if (segmentEnd == charPtr) // Merge with the segment before
                {
                    freeMemory.erase(segment);
                    size += segment.first;
                    charPtr = segment.second;
                    merged = true;
                }
                else if (charPtr + size == segment.second) // Merge with the segment after
                {
                    freeMemory.erase(segment);
                    size += segment.first;
                    endPtr = segmentEnd;
                    merged = true;
                }
                else
                {
                    break; // No more merging can be done
                }
            }

            if (merged)
            {
                freeMemory.insert(std::make_pair(size, charPtr)); // Insert the merged segment
            }
            else
            {
                freeMemory.insert(std::make_pair(size, charPtr));
            }
            mergeFreeSegments();
        }
        else
        {
            throw std::runtime_error("Invalid memory address");
        }
    
    
    }
    void  get_free_mem()
    {
        
            cout << "Free Memory Segments:\n";
            // Iterate through the freeMemory minHeap and print the segments
            while (!freeMemory.empty())
            {
                auto segment = freeMemory.extractMin();
                std::cout << "[" << std::hex << reinterpret_cast<size_t>(segment.second)
                    << " - " << reinterpret_cast<size_t>(segment.second + segment.first) << "] "
                    << "Size: " << std::dec << segment.first << " bytes\n";
            }
            
    }
};

template <class T>
class PriorityQueue
{
    maxHeap<T> PQ;
public:
    PriorityQueue()
    {
        PQ = {};
    }// You know guys...
    void push(T data)
    {
        PQ.insert(data);
    
    
    }// insert data into queue
    void pop()
    {
        // Remove the element with the highest key value from the queue.
        PQ.extractMax();
    
    }// remove highest key value element
    T front()
    {
        // Return the element with the highest key value (the root of the maxHeap).
        return PQ.getMax();
    }// return the element with highest key value
    bool empty()
    {
        // Check if the queue is empty based on the maxHeap size.
        return PQ.empty();
    
    }// tell whether queue is empty or not
    int size()
    {
        return PQ.size();
    }// tell the size of the queue
    ~PriorityQueue()
    {
        delete PQ;
    }// do whatever is necessary...  
};

