
#include "huffman.h"
#include "bit_writer.h"
#include <stdlib.h>
#include <stddef.h>


 void free_tree(heap_node_t *node)
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





static bool generate_codes_recursive(heap_node_t *node,
                                     uint32_t code,
                                     uint8_t len,
                                   huffman_code_t codes[256])
{
    if (node == NULL)
        return true;

    /* Leaf node */
    if (node->left == NULL && node->right == NULL)
    {
        /*
         * Special case: only one unique symbol in the input.
         * Give it a 1-bit code.
         */
        if (len == 0)
        {
            codes[node->byte].code = 0;
            codes[node->byte].len = 1;
        }
        else
        {
            codes[node->byte].code = code;
            codes[node->byte].len = len;
        }

        return true;
    }

    /*
     * We support Huffman codes up to 32 bits.
     * A child would have code length len + 1.
     */
    if (len == 32)
        return false;

    /* Left = 0 */
    if (!generate_codes_recursive(node->left,
                                  code << 1,
                                  len + 1,
                                  codes))
    {
        return false;
    }

    /* Right = 1 */
    if (!generate_codes_recursive(node->right,
                                  (code << 1) | 1,
                                  len + 1,
                                  codes))
    {
        return false;
    }

    return true;
}

bool generate_codes(heap_node_t *root,
                    huffman_code_t codes[256])
{
    return generate_codes_recursive(root, 0, 0, codes);
}





bool huffman_encode(const uint8_t *data,
                    size_t size,
                    huffman_code_t codes[256],
                    bit_writer_t *writer)
{
    if (data == NULL || codes == NULL || writer == NULL)
        return false;

    for (size_t i = 0; i < size; i++)
    {
        huffman_code_t *code = &codes[data[i]];

        if (code->len == 0)
            return false;

        if (!bit_writer_write_code(writer,
                                   code->code,
                                   code->len))
        {
            return false;
        }
    }

    return true;
}