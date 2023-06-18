// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"



// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// declare protoytpe of method
bool unload_linked_list(node *cursor);

// TODO: Choose number of buckets in hash table
const unsigned int N = 26*26*26;

//global variable to store number of words in dictionairy
unsigned int words_in_dict = 0;

// Hash table
node *table[N];

// Char array for loading data from dict-file into
char buffer[LENGTH];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //get hash value of word and set cursor to first element of LL
    int index = hash(word);
    node *cursor = table[index];

    //while strings are not equal and current node is not null
    while(cursor != NULL)
    {
        //if strings are equal
        if(strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        //set cursor to next node in LL only if next node isnt null
        else
        {
            if(cursor->next == NULL)
            {
                return false;
            }
            cursor = cursor->next;
        }
    }

    return false;


}

/*
// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    return toupper(word[0]) - 'A';
}
*/

unsigned int hash(const char *word)
{
    int hash_value = 0;
    if(strlen(word) == 1)
    {
        hash_value = (toupper(word[0]) - 'A') * 626;
    }
    else if(strlen(word) == 2)
    {
        hash_value = ((toupper(word[0]) - 'A') * 626) + ((toupper(word[1]) - 'A') * 26);
    }
    else
    {
        hash_value = ((toupper(word[0]) - 'A') * 626) + ((toupper(word[1]) - 'A') * 26) + (toupper(word[2]) - 'A');
    }
    //Hash function
    //according to hash table
    //[0]       AAA
    //[1]       AAB
    //[2]       AAC
    //[26]      ABA
    //[52]      ACA
    //[626]     BAA
    //[1252]    CAA
    //[1271]    CAT -> for testing with dictionaries/small
    return hash_value;

}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    int index = 0;

    //open dict file
    FILE *file = fopen(dictionary, "r");

    //if unable to open file
    if(!file)
    {
        return false;
    }

    while(fscanf(file, "%s", buffer) != EOF)
    {
        //generate new node with word (buffer) from dict file
        node *new_node = malloc(sizeof(node));
        if(new_node == NULL)
        {
            return false;
        }

        //copy word from buffer to new node and terminate end of string in new node
        strcpy(new_node->word, buffer);
        new_node->next = NULL;

        //generate hash value to get correct index for adding node in hash table
        index = hash(new_node->word);

        //SET FIRST ELEMENT OF ARR[INDEX] - HEAD POINTER OF LL
        //set array index pointer to first element of linked list in hash table index position
        if (table[index] == NULL)
        {
            table[index] = new_node;
            words_in_dict++;
        }
        else
        {
            //set pointer from new node to current top position node in linked list, to add new node at first position
            new_node->next = table[index];
            //SET FIRST ELEMENT OF ARR[INDEX] - HEAD POINTER OF LL
            table[index] = new_node;
            words_in_dict++;
        }
    }
    fclose(file);
    return true;

}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // return value of global variable
    return words_in_dict;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    int index = 0;
    node *cursor;

    while(index < N)
    {
        //if linked list at array pos exists
        if(table[index] != 0x0)
        {
            //unload linked list
            if (!unload_linked_list(table[index]))
            {
                return false;
            }
        }

        index++;
    }

    return true;
}

bool unload_linked_list(node *cursor)
{
    //base case if there is no node to free left
    if(cursor == NULL)
    {
        return true;
    }

    unload_linked_list(cursor->next);

    //free node
    free(cursor);

    return true;

}