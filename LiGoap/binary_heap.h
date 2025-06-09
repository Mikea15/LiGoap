#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <chrono>
#include <random>
#include <algorithm>

template<typename T>
class BinaryHeap {
public:
	BinaryHeap(int reserveSpace) 
	{
		heap.reserve(reserveSpace);
	}

	void insert(const T& value)
	{
		// push value
		heap.push_back(value);
		// heapify index of value
		heapifyUp(heap.size() - (size_t) 1);
	}

	[[nodiscard]] T extractMin()
	{
		if (heap.empty())
		{
			throw std::runtime_error("Heap is empty");
		}

		T minVal = heap[0];		// grab value
		heap[0] = heap.back();	// update front
		heap.pop_back();		// remove last

		if (!heap.empty())
		{
			// restore heap property
			heapifyDown(0);
		}

		return minVal;
	}

	[[nodiscard]] T getMin() const
	{
		if (heap.empty())
		{
			throw std::runtime_error("Heap is empty");
		}

		return heap[0];
	}

	bool empty() const { return heap.empty(); }
	size_t size() const { return heap.size(); }

	void printHeap() const {
		std::cout << "Heap Array: [";
		for (size_t i = 0; i < heap.size(); ++i) {
			std::cout << heap[i];
			if (i < heap.size() - 1) std::cout << ", ";
		}
		std::cout << "]" << std::endl;

		// Visual tree representation for small heaps
		if (heap.size() <= 15) {
			printTree();
		}
	}

private:
	[[nodiscard]] inline int parent(int i) { return (i - 1) / 2; }
	[[nodiscard]] inline int leftChild(int i) { return 2 * i + 1; }
	[[nodiscard]] inline int rightChild(int i) { return 2 * i + 2; }

	void heapifyUp(int index)
	{
		while (index > 0 && heap[parent(index)] > heap[index])
		{
			std::swap(heap[index], heap[parent(index)]);
			index = parent(index);
		}
	}

	void heapifyDown(int index)
	{
		int minIndex = index;
		int left = leftChild(index);
		int right = rightChild(index);

		if (left < heap.size() && heap[left] < heap[minIndex])
		{
			minIndex = left;
		}

		if (right < heap.size() && heap[right] < heap[minIndex])
		{
			minIndex = right;
		}

		if (index != minIndex)
		{
			std::swap(heap[index], heap[minIndex]);
			heapifyDown(minIndex);
		}
	}

	void printTree() const
	{
		if (heap.empty())
		{
			return;
		}

		std::cout << "Tree Structure:" << std::endl;
		size_t levels = 0;
		size_t temp = heap.size();
		while (temp > 0) 
		{ 
			levels++; temp /= 2; 
		}

		size_t index = 0;
		for (size_t level = 0; level < levels && index < heap.size(); ++level) 
		{
			size_t elementsInLevel = 1 << level;  // 2^level
			size_t spaces = (1 << (levels - level - 1)) * 2;

			// Print leading spaces
			for (size_t i = 0; i < spaces - 1; ++i) std::cout << " ";

			// Print elements in this level
			for (size_t i = 0; i < elementsInLevel && index < heap.size(); ++i) 
			{
				std::cout << heap[index++];
				if (i < elementsInLevel - 1 && index < heap.size()) 
				{
					for (size_t j = 0; j < spaces * 2 - 1; ++j) std::cout << " ";
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

private:
	std::vector<T> heap;
};