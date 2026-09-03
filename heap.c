

#include "heap.h"
#include <stdlib.h>

/*For huffman compression we will be using capacity=256 since we need
to track freq of each byte in the file*/


bool heap_init(heap_t* heap,size_t capacity)
{
    heap_node_t** node=(heap_node_t**)malloc(capacity*sizeof(heap_node_t*));
    if(node==NULL) //init failed
    return false;

    heap->capacity=capacity;
    heap->size=0;
    heap->arr=node;
    
    return true;
    

}



/*Swap the node byte and frequency*/
static void swap_nodes(heap_node_t** node1,heap_node_t** node2)
{
  heap_node_t* temp=*node1;
  *node1=*node2;
  *node2=temp;



}






/*When we add new node*/
static void heapify_up(heap_t* heap)
{
    size_t curr_index=(heap->size-1);
   size_t parent_index;
    
    
    while(curr_index>0)
    {
        parent_index=(curr_index-1)/2; 
    
      if((heap->arr[curr_index]->freq)<(heap->arr[parent_index]->freq))
      {
        
        //swap nodes
        swap_nodes(&heap->arr[curr_index],&heap->arr[parent_index]);
        curr_index=parent_index;

        

      }

      else
      break;  //no need to heapify since the tree already satisifies min heap property

        

    }
    
}



/*      Needs to be called for n>=2 nodes
        called when we do remove_min          
                 */
static void heapify_down(heap_t *heap)
{
    

    size_t curr_index = 0;
    size_t left_child_idx;
    size_t right_child_idx;
    size_t smaller;

  

    while (1)
    {
        left_child_idx = 2 * curr_index + 1;
        right_child_idx = 2 * curr_index + 2;

        /* No left child means no children at all */
        if (left_child_idx >= heap->size)
        {
            break;
        }

        /*
         * Assume left child is smaller.
         * Change to right child if it exists and is smaller.
         */
        smaller = left_child_idx;

        if (right_child_idx < heap->size &&
            heap->arr[right_child_idx]->freq <
            heap->arr[left_child_idx]->freq)
        {
            smaller = right_child_idx;
        }

        /*
         * Current node is already <= smallest child.
         * Heap property is satisfied.
         */
        if (heap->arr[curr_index]->freq <=
            heap->arr[smaller]->freq)
        {
            break;
        }

        /* Move current node down */
        swap_nodes(&heap->arr[curr_index],
             &heap->arr[smaller]);

        curr_index = smaller;
    }
}
    


void free_heap(heap_t* heap)
{
    free(heap->arr);
    heap->capacity=0;
    heap->size=0;
}



bool add_node(heap_t* heap,heap_node_t* node)
{
 if(heap->size==heap->capacity) //capacity full
 return false;

 size_t heap_size=heap->size;
 heap->arr[heap_size]=node;
 heap->size++;
 heapify_up(heap);

 return true;

}






/*
Now we remove the min element i.e root
copy last element to root and heapify down to restore the min heap property

*/

heap_node_t* remove_min(heap_t* heap)
{
    if(heap->size==0) //empty heap
    return NULL;


    if(heap->size==1) //single node heap
    {
        heap->size=0;
        return heap->arr[0];

    }

    //n>=2 nodes in heap


 heap_node_t* root_node=heap->arr[0];

    /*replace root node with last node*/
    heap->arr[0]=heap->arr[heap->size-1];

    heap->size--; /*Since we removed the last node*/
   
 
    heapify_down(heap);

    return root_node;


}

