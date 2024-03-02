#include <stdio.h>
#include "SA/memory/mem_alloc.h"
#include "SA/strings/strings.h"
#include "SA/parsing/parser_tree.h"

typedef struct _tree_node {
    char* key;
    char* value;
    struct _tree_node* left_child;
    struct _tree_node* right_child;
} TreeNode;

struct _SA_parser_tree {
    TreeNode* root;
    char* current_key;
    char* current_value;
};


/*
Create a new ParserTree.
If it fails, it returns NULL.
*/
SA_ParserTree* SA_ptree_init()
{
    SA_ParserTree* tree = (SA_ParserTree*) SA_malloc(sizeof(SA_ParserTree));
    if(tree == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
        return NULL;
    }
    
    tree->root = NULL;
    tree->current_key = NULL;
    tree->current_value = NULL;

    return tree;
}

/*
Allocate the space for the partial_key and concatenate it to the older one.
It can be called multiple time if the key you want to store is separated in multiples chunks.
However, because of the realloc, it's quicker if you work with large chunks.
If it succeeded, it returns SA_TRUE.
If it fails, it's a memory error, the operation is aborted and the previously allocated key is freed.
*/
SA_bool SA_ptree_update_key(SA_ParserTree* tree, const char* partial_key, int partial_key_len)
{
    char* writer;
    if(tree->current_key == NULL)
    {
        // New key
        tree->current_key = (char*) SA_malloc((partial_key_len + 1) * sizeof(char));
        writer = tree->current_key;
    }
    else
    {
        int len_old_key = SA_strlen(tree->current_key);
        char* temp = (char*) SA_realloc(tree->current_key, (len_old_key + partial_key_len + 1) * sizeof(char));
        if(temp == NULL)
        {
            SA_free(&(tree->current_key));
        }
        tree->current_key = temp;
        writer = tree->current_key + len_old_key;
    }
    if(tree->current_key == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
        return SA_FALSE;
    }
    
    SA_strncpy(writer, partial_key, partial_key_len);

    return SA_TRUE;
}

/*
Allocate the space for the partial_value and concatenate it to the older one.
It can be called multiple time if the value you want to store is separated in multiples chunks.
However, because of the realloc, it's quicker if you work with large chunks.
If it succeeded, it returns SA_TRUE.
If it fails, it's a memory error, the operation is aborted and the previously allocated value is freed.
*/
SA_bool SA_ptree_update_value(SA_ParserTree* tree, const char* partial_value, int partial_value_len)
{
    char* writer;
    if(tree->current_value == NULL)
    {
        // New key
        tree->current_value = (char*) SA_malloc((partial_value_len + 1) * sizeof(char));
        writer = tree->current_value;
    }
    else
    {
        int len_old_value = SA_strlen(tree->current_value);
        char* temp = (char*) SA_realloc(tree->current_value, (len_old_value + partial_value_len + 1) * sizeof(char));
        if(temp == NULL)
        {
            SA_free(&(tree->current_value));
        }
        tree->current_value = temp;
        writer = tree->current_value + len_old_value;
    }
    if(tree->current_value == NULL)
    {
        _SA_set_error(SA_ERROR_MALLOC);
        return SA_FALSE;
    }
    
    SA_strncpy(writer, partial_value, partial_value_len);

    return SA_TRUE;
}

/*
Abort the saving of current key/value.
This doesn't free the tree and you can continue to fill it after that.
You can use this in case you realize while parsing that the current partial key/values you are saving aren't wanted.
*/
void SA_ptree_abort(SA_ParserTree* tree)
{
    SA_free(&(tree->current_key));
    SA_free(&(tree->current_value));
}

/*
Once you have added your key and your value,
you have to call SA_ptree_push to add this couple to the final tree and start saving something else.

DATA_MODIFICATIONS is a pointer to a function which can operate on the key and the value.
For example, you can decode a base64 or an urlencoded string at this moment.
*/
SA_bool SA_ptree_push(SA_ParserTree* tree, void (*data_modifications)(char*))
{
    TreeNode* node = (TreeNode*) SA_malloc(sizeof(TreeNode));
    if(node == NULL)
        return SA_FALSE;
    node->key = tree->current_key;
    if(data_modifications != NULL)
        (*data_modifications)(node->key);
    node->value = tree->current_value;
    if(data_modifications != NULL)
        (*data_modifications)(node->value);
    node->left_child = NULL;
    node->right_child = NULL;

    tree->current_key = NULL;
    tree->current_value = NULL;

    if(tree->root == NULL)
    {
        tree->root = node;
        return SA_TRUE;
    }

    TreeNode* root = tree->root;

    while(root != NULL)
    {
        int cmp = SA_strcasecmp(node->key, root->key);
        if(cmp == 0)
        {
            // The key already exists, just replace the value
            SA_free(&(root->value));
            root->value = node->value;
            SA_free(&(node->key));
            SA_free(&node);
            return SA_TRUE;
        }
        if(cmp < 0)
        {
            if(root->left_child == NULL)
            {
                root->left_child = node;
                return SA_TRUE;
            }
            root = root->left_child;
        }
        else
        {
            if(root->right_child == NULL)
            {
                root->right_child = node;
                return SA_TRUE;
            }
            root = root->right_child;
        }
    }
    // we are not supposed to go here
    return SA_FALSE;
}

/*
Once the parsing is done, use this to get the value associated with KEY (KEY is case unsensitive).
If you have to modify this value, copy it before.
*/
const char* SA_ptree_get_value(SA_ParserTree* tree, const char* key)
{
    TreeNode* root = tree->root;
    while(root != NULL)
    {
        int cmp = SA_strcasecmp(key, root->key);
        if(cmp == 0)
        {
            return root->value;
        }
        if(cmp < 0)
        {
            root = root->left_child;
        }
        else
        {
            root = root->right_child;
        }
    }
    return NULL;
}

static void _ptree_free(TreeNode* root)
{
    if(root == NULL)
        return;
    
    if(root->left_child != NULL)
        _ptree_free(root->left_child);
    if(root->right_child != NULL)
        _ptree_free(root->right_child);
    
    SA_free(&(root->key));
    SA_free(&(root->value));
    
    SA_free(&root);
}

/*
Take the address of the tree handler.
Free the tree and set the tree handler to NULL.
*/
void SA_ptree_free(SA_ParserTree** tree)
{
    if(*tree != NULL)
    {
        _ptree_free((*tree)->root);
        SA_free(&((*tree)->current_key));
        SA_free(&((*tree)->current_value));
        SA_free(tree);
    }
}

static void _ptree_display(TreeNode* root)
{
    if(root->left_child != NULL)
        _ptree_display(root->left_child);
    if(root->right_child != NULL)
        _ptree_display(root->right_child);
    
    if(root->key != NULL)
        printf("KEY: %s\n", root->key);
    if(root->value != NULL)
        printf("VALUE: %s\n\n", root->value);
}

/*
Display the tree.
Used for debug purpose.
*/
void SA_ptree_display(SA_ParserTree* tree)
{
    _ptree_display(tree->root);
}
