//
//  parallel_merge_sort.cpp
//  CCIA_Learning
//  并发两路归并
//  Created by lmc on 2022/1/28.
//

#include <iostream>
#include <thread>
#include <vector>
using namespace std;

void merge(vector<int>& nums, int s, int mid, int e){
    vector<int> lnums, rnums; // lnums, rnums为两个子序列，nums用于存放合并后的序列
    for(int i = s; i <= e; i++){
        if(i <=mid){
            lnums.push_back(nums[i]);
        }else{
            rnums.push_back(nums[i]);
        }
    }

    int l = 0, r = 0, k = s;
    // 比较两个指针(l和r)所指向的元素，选择相对小的元素(升序)放入到合并空间，
    // 并移动指针到下一位置，直到其中一个指针超出序列尾
    while(l < lnums.size() && r < rnums.size()){
        if(lnums[l] < rnums[r]){
            nums[k++] = lnums[l++];
        }else{
            nums[k++] = rnums[r++];
        }
    }
    // 将另一序列剩下的所有元素直接复制到合并序列尾
    while(l < lnums.size()){
        nums[k++] = lnums[l++];
    }
    while(r < rnums.size()){
        nums[k++] = rnums[r++];
    }
}

void mergeSort(vector<int>& nums, int start, int end){
    if(start >= end){
        return;
    }
    int mid = (start + end) / 2;
    // 不断地对半拆分数组
    thread t1(mergeSort, std::ref(nums), start, mid);
    thread t2(mergeSort, std::ref(nums), mid + 1, end);
    t1.join();
    t2.join();
    // 合并相邻的子数组
    merge(nums, start, mid, end);
}

vector<int> sortArray(vector<int>& nums) {
    vector<int> arr = nums; // copy
    mergeSort(arr, 0, static_cast<int>(arr.size()-1));
    return arr;
}

int main(){
    vector<int> nums{10,9,8,7,2,3,4,5,6,1,0}, res;
    res = sortArray(nums);
    for(int num : res){
        cout << num << " ";
    }
    cout << endl;
    return 0;
}
