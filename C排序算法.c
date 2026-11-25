#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define n 20

/*重要概念
稳定性：值相等的元素排序后相对顺序不变。稳定的排序在处理带有多个关键字的数据时更可靠。
原地排序：只用常数级额外空间，直接在原数组上操作。
*/

/*
 *  各算法复杂度速查（n 为元素个数）：
 *
 *  冒泡排序      最好 O(n)（带提前退出）| 平均/最坏 O(n^2)      | 额外空间 O(1)     | 稳定
 *  选择排序      最好/平均/最坏 O(n^2)                          | 额外空间 O(1)     | 不稳定
 *  插入排序      最好 O(n)               | 平均/最坏 O(n^2)      | 额外空间 O(1)     | 稳定
 *  二分插入排序  比较次数 O(n log n)，但元素移动仍是 O(n^2)      | 额外空间 O(1)     | 稳定
 *  希尔排序      取决于间隔序列（Knuth 序列最坏约 O(n^1.5)）    | 额外空间 O(1)     | 不稳定
 *  归并排序      最好/平均/最坏 O(n log n)                      | 额外空间 O(n)     | 稳定
 *  快速排序      最好/平均 O(n log n)    | 最坏 O(n^2)           | 额外空间 O(log n) | 不稳定
 *
 *  
*/

/*------------------------------------------构建元素------------------------------------------*/

//创建需要被排序元素结构体
typedef struct Item
{
    int id;//稳定性判断依据
    int key;//排序依据
}Item;

//创建范围结构体（用于快排迭代法中的“待处理区间栈”）
typedef struct Range
{
    int start;//区间起点
    int end;//区间终点
}Range;

/*------------------------------------------运行函数------------------------------------------*/

//定义函数指针
typedef void (*SortFunc)(Item[]);

//初始化
void Init(Item arr[]);

//交换顺序
void Swap(Item *a, Item *b);

//打印函数
void PrintArray(const Item arr[]);

//运行函数
void Run(const char Name[], SortFunc Func);

/*------------------------------------------辅助函数------------------------------------------*/


//返回最小值
int Min(int x, int y);

//返回最大值
int Max(int x, int y);

//二分查找：返回第一个 key 大于给定 key 的位置
int UpperBound(const Item arr[], int len, int key);

//创建新的范围
Range NewRange(int s, int e);

//合并相邻的两个有序区间 arr[lo..mid) 与 arr[mid... hi)
void Merge(Item arr[], Item temp[], int lo, int mid, int hi);

//迭代归并的主要函数
void MergeSortRec(Item arr[], Item tmp[], int lo, int hi);

//快速排序时获取 arr [lo...hi]划分，返回下标
int Partition(Item arr[], int lo, int hi);

//递归快排的主要函数
void QuickSortRec(Item arr[], int lo, int hi);

/*------------------------------------------排序函数------------------------------------------*/

//有序性检查：key是否有序
int IsSorted(const Item arr[]);

//稳定性检查：key相同的元素id应递增
//注意，这只能得到对本次这个数组，这个算法的稳定性，不能得到算法的稳定性
int IsStable(const Item arr[]);

//冒泡排序：相邻元素比较不断比较大小，顺序错了就交换
void BubbleSort(Item arr[]);

//选择排序：把不断把最 大/小 值放在 右/左 端
void SelectionSort(Item arr[]);

//插入排序：构建有序序列，如无序就找地方插入到有序中
//非原地插入：(没有实际收益，一般不用)
void InsertionSort_OutPlace(Item arr[]);
//原地插入：
void InsertionSort_InPlace(Item arr[]);
//非原地二分插入：(没有实际收益，一般不用)
//void InsertionSort_Binary_OutPlace(Item arr[]);
//原地二分插入：
void InsertionSort_Binary_InPlace(Item arr[]);

//希尔排序：
//非原地希尔排序：(没有实际收益，一般不用)
//void ShellSort_OutPlace(Item arr[]);
//原地希尔排序：
void ShellSort_InPlace(Item arr[]);

//归并排序：
//迭代法：
void MergeSort_Iterative(Item arr[]);
//递归法：
void MergeSort_Recursive(Item arr[]);

//快速排序：
//迭代法：
void QuickSort_Iterative(Item arr[]);
//递归法：
void QuickSort_Recursive(Item arr[]);


/*------------------------------------------主函数------------------------------------------*/

int main(void)
{
    Item arr[n]={0};
    Init(arr);
    printf("原始数组:\n");
    PrintArray(arr);
    printf("\n");

    
    Run("冒泡", BubbleSort);
    Run("选择", SelectionSort);
    Run("插入非原地", InsertionSort_OutPlace);
    Run("插入原地", InsertionSort_InPlace);
    //Run("二分插入非原地", InsertionSort_Binary_OutPlace);
    Run("二分插入原地", InsertionSort_Binary_InPlace);
    //Run("希尔非原地", ShellSort_OutPlace);
    Run("希尔原地", ShellSort_InPlace);
    Run("归并递归", MergeSort_Recursive);
    Run("归并迭代", MergeSort_Iterative);
    Run("快速递归", QuickSort_Recursive);
    Run("快速迭代", QuickSort_Iterative);

    

    return 0;
}

/*------------------------------------------函数实现------------------------------------------*/

//初始化
void Init(Item arr[])
{
    //准备重复的key来观察是否稳定
    static const int keys[n] = {22, 34, 3, 55, 32, 82, 55, 55, 89, 50, 37, 5, 50, 5, 64, 35, 35, 9, 70, 70};
    for (int i =0; i < n; i++)
    {
        arr[i].id = i;
        arr[i].key =keys[i];
    }
}

//交换顺序
void Swap(Item *a, Item *b)
{
    Item t = *a;
    *a = *b;
    *b = t;
}


//打印函数
void PrintArray(const Item arr[])
{
    printf("id:     :");
    for (int i = 0; i < n; i++)
        printf("%d ",arr[i].id);
    printf("\n");

    printf("key     :");
    for (int i = 0; i < n; i++)
    printf("%d ",arr[i].key);
    printf("\n");
}

void Run(const char Name[], SortFunc Func)
{
    Item arr[n];
    int i = -1;
    int j = -1;

    Init(arr);
    Func(arr);

    printf("%s:\n",Name);
    PrintArray(arr);

    i = IsSorted(arr);
    j = IsStable(arr);
    
    if(i == 0)
        printf("有序\n");
    else if(i == 1)
        printf("无序\n");
    else
        printf("出错\n");
    
    if(j == 0)
        printf("稳定\n");
    else if(j == 1)
        printf("不稳定\n");
    else
        printf("出错\n");
    printf("\n");
}

//有序性检查
//0表示有序，1表示无序
int IsSorted(const Item arr[])
{
    for (int i = 0; i < n-1; i++)
        if(arr[i].key > arr[i+1].key)
            return 1;
    return 0;
}

//稳定性检查
//如果稳定，那么key相同的元素id应递增，不稳定返回 1 稳定返回 0
int IsStable(const Item arr[])
{
    for (int i = 0 ; i < n-1; i++)
        if (arr[i].key == arr[i+1].key)
            if (arr[i].id > arr[i+1].id)
                return 1;
    return 0;
}

//返回最小值
int Min(int x, int y)
{
    return x < y ? x : y;
}

//返回最大值
int Max(int x, int y)
{
    return x > y ? x : y;
}

//二分查找插入位置：返回第一个 key 大于给定 key 的位置
int UpperBound(const Item arr[], int len, int key)
{
    int left = 0;
    int right = len;
    int mid = -1;

    while(left < right)
    {
        mid = left + (right - left) / 2;

        if(arr[mid].key <= key)//如果 中 比指定的小或相等 就让 左 变成 中
            left = mid +1;
        else
            right = mid;//如果大，就让 右 变成 中
    }
    return left;
}

//创建新的范围
Range NewRange(int s, int e)
{
    Range r;
    r.start= s ;
    r.end = e;
    return r;
}

//冒泡排序
void BubbleSort(Item arr[])
{
    for (int i = 0; i < n-1; i++)
    {
        for (int j = 0; j < n-1; j++)
        {
            if(arr[j].key>arr[j+1].key)
                Swap(&arr[j], &arr[j+1]);
        }
    }
}

//选择排序
void SelectionSort(Item arr[])
{
    int MinNum = 0;
    for (int i = 0; i < n; i++)
    {
        MinNum = arr[i].key;
        for (int j = i; j < n; j++)
        {
            if (arr[j].key < MinNum)
                {
                    MinNum = arr[j].key;
                    Swap(&arr[i], &arr[j]);
                }
        }
    }
}

//非原地插入排序
void InsertionSort_OutPlace(Item arr[])
{
    Item temp[n];
    temp[0] = arr[0];

    for (int i = 1; i < n; i++)//遍历无序数组
    {
        int j = 0;
        while((j < i) && (temp[j].key <= arr[i].key))//从头开始便利，直到找到新来元素严格小雨某个元素为止（找不到就是j=i,排在最后）
            j++;
        for(int t = i; t > j; t--)//把插入位置之后的元素都后移一位
            temp[t] = temp[t-1];
        temp[j] = arr[i];//插入
    }
    memcpy(arr, temp, sizeof(temp));//拷贝回去
}

//原地插入排序
void InsertionSort_InPlace(Item arr[])
{
    for(int i = 1; i < n; i++)//遍历
    {
        Item temp = arr[i];
        int j = i;

        while(j > 0 && arr[j-1].key > temp.key)//从后向前找第一个 <= 新元素的地方，这样是为了让新元素插入到 key相同的元素的后面，保证稳定性
        {
            arr[j] = arr[j-1];//只要每到那个特定的需要插入的位置，同意后移，由于已经用temp备份，所以覆盖arr[i]也没问题
            j--;
        }
        arr[j] = temp;//最后把新来的元素插入到正确的位置
    }
}

//原地二分插入排序
void InsertionSort_Binary_InPlace(Item arr[])
{
    Item cur;
    int pos;
    for(int i = 1; i < n; i++)//遍历
    {
        cur = arr[i];//准备插入的元素
        pos = UpperBound(arr, i, cur.key);//用二分查找在已经排好的 arr 中找到位置

        if(pos < i)//如果pos==i,那本身就是有序的，不用动
        {
            memmove(&arr[pos+1], &arr[pos], (i - pos) * sizeof(Item));//整体后移一个元素大小
            arr[pos] = cur;//最后插入
        }
    }
}

//原地希尔排序：把数组分为gap个子序列，每组内用插入排序，然后缩小gap,再排，最终gap==1时排一次
void ShellSort_InPlace(Item arr[])
{
    int gap = 1;
    int j =0;

    while(gap < n/3)//选取合适的 gap
        gap = gap * 3 + 1;
    
    for( ;gap >= 1; gap /=3)//遍历各种大小的 gap
    {
        for (int i = gap; i < n; i++)//进行间隔为 gap 的插入排序
        {
            j = i;
            while(j >= gap && arr[j-gap].key > arr[i].key)//对于同一组内（下标差gap）,比cur大的元素后移 gap 格
            {
                arr[j] = arr[j-gap];
                j -= gap;
            }
            arr[j] = arr[i];//最后插入
        }
    }
}

//合并相邻的两个有序区间
void Merge(Item arr[], Item temp[], int lo, int mid, int hi)
{
    int i = lo;
    int j = mid;
    int k = lo;

    while( i < mid && j < hi)
    {
        //注意这里是 <= 保证取等时赋值给左边来保证稳定性
        if(arr[i].key <= arr[j].key)//因为已经有序，所以temp接收的总是最小的，保证其也有序
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    //现在两个区间已经有一个用完了，剩下的直接接上，二者都有序且一方绝对大于另一方，temp仍有序
    while (i < mid)//如果j的先被用完
        temp[k++] = arr[i++];
    while (j < hi)//如果i的先被用完
        temp[k++] = arr[j++];

    memmove(&arr[lo], &temp[lo], (hi - lo)*sizeof(Item));//拷贝回去
}

//递归法归并排序
/*
长度为 1 本身有序，直接返回
中间切开，分别递归排序
合并
*/
void MergeSortRec(Item arr[], Item temp[], int lo, int hi)
{
    if(hi - lo <= 1)//一个数据本身有序
        return;
    
    int mid = lo + (hi - lo) / 2;

    //分成两端再去归并
    MergeSortRec(arr, temp, lo, mid);
    MergeSortRec(arr, temp, mid, hi);

    //优化：假如一段最大值没另一短最小值大，直接合并
    if(arr[mid-1].key <= arr[mid].key)
        return;
    
    Merge(arr, temp, lo, mid, hi);//按照之前的方法把两个有序列合成一个有序列
}

//最终调用的函数
void MergeSort_Recursive(Item arr[])
{
    Item temp[n];//一个用于中转的临时数组
    MergeSortRec(arr, temp, 0, n);
}

//迭代法归并排序
//不用递归，而是模拟递归来避免递归的缺点
void MergeSort_Iterative(Item arr[])
{
    Item temp[n];//临时存放

    for(int width = 1; width < n; width *=2)//模拟宽度，从1开始逐渐变大
    {
        for (int lo = 0; lo < n; lo += 2*width)//通过调控每次+=的大小来模拟不同尺度下的归并
        {
            //模拟出一个被二分的序列
            int mid = Min(lo + width, n);
            int hi = Min(lo + 2*width, n);

            if(mid < hi)//相等时说明右边是空的，不用合并
                Merge(arr, temp, lo, mid, hi);//合并两个序列
        }
    }
}

//快速排序时用于返回划分下标的函数
int Partition(Item arr[], int lo, int hi)
{
    int mid = lo + (hi - lo) / 2;

    //确保三者顺序
    if (arr[mid].key < arr[lo].key) Swap(&arr[mid], &arr[lo]);
    if (arr[hi].key  < arr[lo].key) Swap(&arr[hi],  &arr[lo]);
    if (arr[hi].key  < arr[mid].key) Swap(&arr[hi], &arr[mid]);

    //设置基准值和左右边界
    int pivot = arr[mid].key;//取三者中位数
    int i = lo - 1;
    int j = hi + 1;

    while(1)
    {
        //do...while确保至少会动一下不会死循环
        //把大小与基准分化不适配的标记出来，记为i j 两边一起探查更快速
        do{i++;} while (arr[i].key < pivot);
        do{j--;} while (arr[j].key > pivot);

        if(i >= j)//指针相遇或交错
            return j;
        
        Swap(&arr[i], &arr[j]);//换边
    }
}

//递归法快速排序
void QuickSortRec(Item arr[], int lo, int hi)
{
    if(lo >= hi)//只有一个元素，本身有序
        return;
    
    int p = Partition(arr, lo, hi);//获取划分

    //分成两端开始递归
    QuickSortRec(arr, lo, p);
    QuickSortRec(arr, p+1, hi);

}

//调用入口
void QuickSort_Recursive(Item arr[])
{
    QuickSortRec(arr, 0, n - 1);//这里用 n-1 因为这里的区间是闭区间，那个数组就到 n-1
}

//迭代快速排序
/*
自己制作一个数组模拟栈，每个元素都是 Range(带排序的区间)
循环：弹出一个区间，划分，得到两个再入栈
栈空时结束

先压大的再压小的，这样小的会先弹出，保证栈内同时存在的区间较少(如果不管顺序栈可能会很大)
*/
void QuickSort_Iterative(Item arr[])
{
    Range stack[64];//手搓一个栈
    int top = 0;// top == 0表示栈空

    stack[top++] = NewRange(0, n-1);//整个数组入栈

    Range r;
    Range left;
    Range right;
    int p;
    while(top > 0)//栈非空
    {
        r = stack[--top];//弹出一个待处理区间
        
        if(r.start >= r.end)//一个元素本身有序
            continue;
        
        p = Partition(arr, r.start, r.end);//获取待处理区间的划分

        //模拟递归中划分出两个
        left = NewRange(r.start, p);
        right = NewRange(p+1, r.end);

        //先压大的，再压小的
        if(left.end-left.start > right.end-right.start)
        {
            stack[top++] = left;
            stack[top++] = right;
        }
        else
        {
            stack[top++] = right;
            stack[top++] = left;
        }
    }
}