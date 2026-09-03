
#include "heap.h"
#include <stdlib.h>




static void free_tree(heap_node_t *node)
{
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}



/*If wer failed to build the huffman tree free each node of the huffman
tree built so far.
Note that each node can be a huffman subtree so we have free_tree here
*/
static void free_remaining_nodes(heap_t *heap)
{
    heap_node_t *n;
    while ((n = remove_min(heap)) != NULL)
        free_tree(n);
}




static heap_node_t* combine_nodes(heap_node_t* a,heap_node_t* b)
{
    heap_node_t* parent_node=(heap_node_t*)malloc(sizeof(heap_node_t));
    if(parent_node==NULL)
    return NULL;

    parent_node->freq=a->freq+b->freq;
    parent_node->byte=0; //we dont care about internal nodes
    parent_node->left=a;
    parent_node->right=b;

    return parent_node;


}


static heap_node_t* create_node(uint8_t byte,uint32_t freq)
{
    heap_node_t* node=(heap_node_t*)malloc(sizeof(heap_node_t));
   
    if(node==NULL) //err create node
    return NULL;

    node->byte=byte;
    node->freq=freq;

    node->left=node->right=NULL;

    return node; 



}

/*On success return ptr to huffman tree and NULL on failure*/
heap_node_t* build_huffman_tree(uint32_t freq[256])
{
  heap_t heap;

  if(!heap_init(&heap,MAX_HEAP_CAPACITY))
  {
    return NULL; 
  }

  heap_node_t* new_node;

  /*Create a node for each byte*/
  for(size_t i=0;i<256;i++)
  {
    if(freq[i]!=0)
    {
      new_node=create_node(i,freq[i]);
      if(new_node==NULL)
      {
        free_remaining_nodes(&heap);
        free_heap(&heap);
        return NULL;
      }

    /*add node to heap*/
    add_node(&heap,new_node);


    }


  }



  /*Now the min heap is ready*/

    heap_node_t* node1; /*Now we extract two nodes and combine them and add back to heap*/
    heap_node_t* node2;
    heap_node_t* parent_node;

   while(heap.size>1)
   {
    node1=remove_min(&heap);
    node2=remove_min(&heap);

    parent_node=combine_nodes(node1,node2);

    /*At this point node1 and node2 can be huffaman subtree themselves so
    we need to free them and then free the nodes in the remaining huffaman tree so as to avoid memory
    leak of any sort */

    if(parent_node==NULL)
    {
        free_tree(node1);
        free_tree(node2);
     free_remaining_nodes(&heap);
     free_heap(&heap);
     return NULL;

    }



    add_node(&heap,parent_node);


   }


    /*Now we have the huffman tree ready*/

  heap_node_t* huffman_root=remove_min(&heap);

     free_heap(&heap);

   return huffman_root;




}



